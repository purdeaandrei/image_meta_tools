#include <stdio.h>
#include <mxml.h>
#include <jpeglib.h>
#include "image2xmlmem.h"
#include "xmlmem2xmlfile.h"
#include "xmlmem_data_source_manager.h"

void help(int argc, char ** argv) {
    printf("Usage: %s input.mpo output.jps [ flags ]\n", argv[0]);
    printf("-hh: half-height output (pixels are averaged\n");
    printf("-hw: half-width output (pixels are averaged\n");
    printf("-fr: right image first (topmost/leftmost/red)\n");
    printf("-fl: left image first (topmost/leftmost/red) (default!)\n");
    printf("-i: interleaved layout\n");
    printf("-s: side by side layout (default!)\n");
    printf("-o: over-under layout\n");
    printf("-a: anaglyph\n");
    printf("-qNUMBER: JPEG quality (0..100). remember, MPO->JPS conversion reencodes the image!\n");
    printf("NOTE: monoscopic format is not implemented.\n");
}


mxml_node_t *getMPIndividualNumTag(mxml_node_t *image) {
    if (!IS_ELEMENT(image, "image")) { printf("get_individual_image_number: this must be an image!\n"); return NULL; }
    FOR_CHILDREN(marker, image) {
        if (IS_ELEMENT(marker, "APP2") && IS_ELEMENT_ATTR(marker, "type", "MPF")) {
            FOR_CHILDREN(ifd, marker) {
                if (IS_ELEMENT(ifd, "ifd")) {
                    FOR_CHILDREN(tag, ifd) {
                        if (IS_ELEMENT(tag, "tag") && IS_ELEMENT_ATTR(tag, "id", "MPIndividualNum")) {
                            return tag;
                        }
                    }
                }
            }
        }
    }
    return NULL;
}

int getMPIndividualNumValue(mxml_node_t *image) {
     mxml_node_t *tag = getMPIndividualNumTag(image);
     if (tag!=NULL) {
        const char *value = mxmlElementGetAttr(tag, "value");
        if (value==NULL) return 0;
        int t;
        if (1!=sscanf(value, "%d", &t)) return 0;
        return t;
     } else return 0;
}


static void my_concat(JSAMPARRAY out, JSAMPARRAY in1, JSAMPARRAY in2, size_t in1_n, size_t in2_n) {
    int i;
    for (i=0;i<in1_n;i++) {
        out[0][i] = in1[0][i];
    }
    for (;i<in1_n + in2_n; i++) {
        out[0][i] = in2[0][i - in1_n];
    }
}

static void average_two_rows(JSAMPARRAY a, size_t n) {
    int i;
    for (i=0;i<n;i++) {
        a[0][i] = (a[0][i] + a[1][i])/2;
    }
}

static void half_row_length(JSAMPARRAY a, size_t n, size_t components) {
    int i, j;
    for (i=0;i<n/2/components;i++) {
        for (j=0;j<components;j++) {
            a[0][i*components + j] = (a[0][2*i*components + j] + a[0][(2*i+1)*components + j])/2;
        }
    }
}

static void compose_anaglyph(JSAMPARRAY red, JSAMPARRAY bluegreen, size_t n) {
    int i;
    for (i=0;i<n/3;i++) {
        red[0][i*3+1] = bluegreen[0][i*3+1];
        red[0][i*3+2] = bluegreen[0][i*3+2];
    }
}

static void get_left_and_right(char *filename, mxml_node_t **left, mxml_node_t **right) {
    mxml_node_t *xml = i2xm_load_filename(filename,  /* interpret_concatenated true: */ 1);
    if (xml==NULL) {
        printf("ERROR: there was a problem opening the image file.\n");
        exit(-1);
    }
    if (xml->value.element.name[0]=='?') xml = xml->child;
    *left = *right = NULL;
    FOR_CHILDREN(image, xml) {
        if (IS_ELEMENT(image, "image")) {
            if (*left==NULL) *left = image;
            else if (*right==NULL) *right = image;
            else WARNING("there are more then 2 images in this file. only considering the first two");
        }
    }
    if (*left==NULL || *right==NULL) {
        printf("ERROR: there aren't enaugh images in input\n");
        exit(-2);
    }
    int left_n = getMPIndividualNumValue(*left), right_n = getMPIndividualNumValue(*right);
    if (right_n != 0 && left_n != 0 && right_n < left_n) {
        mxml_node_t *t;
        t = *left;
        *left = *right;
        *right = t;
    }
}

int first_right = 0;
int half_height = 0;
int half_width = 0;
char layout = 's';
int quality=-1; // by default, don't set the quality

static void interpret_arguments(int argc, char **argv) {
    if (argc<3) {
        help(argc, argv);
        exit(-1);
    }
    int i;
    for (i=3;i<argc;i++) {
        if (argv[i][0]!='-') {
            printf("argument nr %d (%s) should be a flag (start with -)!\n", i, argv[i]);
            exit(-1);
        }
        switch (argv[i][1]) {
            case 'h': 
                if (argv[i][2] == 'h') half_height = 1;
                else if (argv[i][2] == 'w') half_width = 1;
                else {
                    printf("argument nr %d (%s) in unrecognized!\n", i, argv[i]);
                    exit (-1);
                }
                break;
            case 'f':
                if (argv[i][2] == 'r') first_right = 1;
                if (argv[i][2] == 'l') first_right = 0;
                break;
            case 'i':
            case 's':
            case 'o':
            case 'a':
                layout = argv[i][1];
                break;
            case 'q':
                if (1!=sscanf(argv[i]+2, "%d", &quality)) {
                    printf("invalid argument: unknown quality. correct examples: -q100 -q75\n");
                    exit(-1);
                }
                break;
            default:
                printf("unknown argument nr %d (%s)\n", i, argv[i]);
                exit(-1);
        }
    }
}

int main(int argc, char **argv) {
    interpret_arguments(argc, argv);
    mxml_node_t *left, *right;
    get_left_and_right(argv[1], &left, &right);
    if (first_right) { // ugly , but works
        mxml_node_t *t;
        t = left;
        left = right;
        right = t;
    }
    
    // LEFT:
    struct jpeg_decompress_struct left_cinfo;
    struct jpeg_error_mgr left_jerr;
    left_cinfo.err = jpeg_std_error(&left_jerr);
    jpeg_create_decompress(&left_cinfo);
    jpeg_xmlmem_src(&left_cinfo, left);
    jpeg_read_header(&left_cinfo, TRUE);
    jpeg_start_decompress(&left_cinfo);
    
    if (half_height && left_cinfo.output_height % 2 == 1) {
        printf("error: left image must have a height divisible by two , if half-height output is desired!\n");
    }
    if (half_width && left_cinfo.output_width % 2 == 1) {
        printf("error: left image must have a width divisible by two , if half-width output is desired!\n");
    }
    
    int left_row_stride = left_cinfo.output_width * left_cinfo.output_components;
    JSAMPARRAY left_buffer = (*left_cinfo.mem->alloc_sarray)
		((j_common_ptr) &left_cinfo, JPOOL_IMAGE, left_row_stride, half_height ? 2 : 1);

    // RIGHT:
    struct jpeg_decompress_struct right_cinfo;
    struct jpeg_error_mgr right_jerr;
    right_cinfo.err = jpeg_std_error(&right_jerr);
    jpeg_create_decompress(&right_cinfo);
    jpeg_xmlmem_src(&right_cinfo, right);
    jpeg_read_header(&right_cinfo, TRUE);
    jpeg_start_decompress(&right_cinfo);
    
    if (half_height && right_cinfo.output_height % 2 == 1) {
        printf("error: right image must have a height divisible by two , if half-height output is desired!\n");
    }
    if (half_width && right_cinfo.output_width % 2 == 1) {
        printf("error: right image must have a width divisible by two , if half-width output is desired!\n");
    }
    
    int right_row_stride = right_cinfo.output_width * right_cinfo.output_components;
    JSAMPARRAY right_buffer = (*right_cinfo.mem->alloc_sarray)
		((j_common_ptr) &right_cinfo, JPOOL_IMAGE, right_row_stride, half_height ? 2 : 1);

    if (right_cinfo.output_width != left_cinfo.output_width) {
        printf("left, and right image width doesn't match!\n");
        exit(-100);
    }
    if (right_cinfo.output_height != left_cinfo.output_height) {
        printf("left, and right image height doesn't match!\n");
        exit(-100);
    }
    if (right_cinfo.output_components != left_cinfo.output_components) {
        printf("left, and right image components doesn't match!\n");
        exit(-100);
    }
    if (right_cinfo.out_color_space != left_cinfo.out_color_space) {
        printf("weird, colorspaces should match!\n");
        exit(-100);
    }

    // OUTPUT:
    struct jpeg_compress_struct out_cinfo;
    struct jpeg_error_mgr out_jerr;
    FILE *outfile;
    out_cinfo.err = jpeg_std_error(&out_jerr);
    jpeg_create_compress(&out_cinfo);
    
    if ((outfile = fopen(argv[2], "wb")) == NULL) {
        fprintf(stderr, "can't open output\n");
        exit(1);
    }
    jpeg_stdio_dest(&out_cinfo, outfile);
    
    out_cinfo.input_components = left_cinfo.output_components;
    out_cinfo.in_color_space = left_cinfo.out_color_space;
    
    if (layout=='a' && (out_cinfo.input_components!=3 || out_cinfo.in_color_space!=JCS_RGB)) {
        printf("error: anaglyph works only with rgb\n");
        exit (-55);
    }
    
    switch (layout) {
        case 's': // side by side
            out_cinfo.image_width = (left_cinfo.output_width + right_cinfo.output_width) / (half_width ? 2 : 1);
            out_cinfo.image_height = left_cinfo.output_height / (half_height ? 2 : 1);
            break;
        case 'o': // over/under
        case 'i': // interlaced
            out_cinfo.image_width = left_cinfo.output_width / (half_width ? 2 : 1);
            out_cinfo.image_height = (left_cinfo.output_height + right_cinfo.output_height) / (half_height ? 2 : 1);
            break;
        case 'a': // anaglyph
            out_cinfo.image_width = left_cinfo.output_width / (half_width ? 2 : 1);
            out_cinfo.image_height = left_cinfo.output_height / (half_height ? 2 : 1);
            break;
    }
    jpeg_set_defaults(&out_cinfo);
    if (quality!=-1)
        jpeg_set_quality(&out_cinfo, quality, TRUE);
    
    jpeg_start_compress(&out_cinfo, TRUE);

    JOCTET jpsmarker[] = "_JPSJPS_" /* descriptor length: */ "\x00\x04" /*descriptor:*/ "\x00\x00\x00\x01" /*comment length:*/ "\x00\x00";
    int jpsmarkerlength = 16;
    switch (layout) {
        case 'i': jpsmarker[12] = 0x01; break;
        case 's': jpsmarker[12] = 0x02; break;
        case 'o': jpsmarker[12] = 0x03; break;
        case 'a': jpsmarker[12] = 0x04; break;
    }
    if (half_height) jpsmarker[11] |= 1;
    if (half_width) jpsmarker[11] |= 2;
    if (!first_right) jpsmarker[11] |= 4;

    jpeg_write_marker(&out_cinfo, JPEG_APP0 + 3, jpsmarker, jpsmarkerlength);

    switch (layout) {
        case 'a':
        case 'i':
        case 's': {
            JSAMPARRAY out_buffer = (*right_cinfo.mem->alloc_sarray)
        		((j_common_ptr) &right_cinfo, JPOOL_IMAGE, (left_row_stride + right_row_stride) / (half_width ? 2 : 1), 1);

            while (left_cinfo.output_scanline < left_cinfo.output_height &&
                   right_cinfo.output_scanline < right_cinfo.output_height &&
                   out_cinfo.next_scanline < out_cinfo.image_height) {
                jpeg_read_scanlines(&left_cinfo, left_buffer, 1);
                jpeg_read_scanlines(&right_cinfo, right_buffer, 1);
                if (half_height) {
                    JSAMPROW t[1];
                    t[0] = left_buffer[1];
                    jpeg_read_scanlines(&left_cinfo, t, 1);
                    t[0] = right_buffer[1];
                    jpeg_read_scanlines(&right_cinfo, t, 1);
        
                    average_two_rows(left_buffer, left_row_stride);
                    average_two_rows(right_buffer, right_row_stride);
                }
                if (half_width) {
                    half_row_length(left_buffer, left_row_stride, left_cinfo.output_components);
                    half_row_length(right_buffer, right_row_stride, right_cinfo.output_components);
                }
                if (layout=='s') {
                    my_concat(out_buffer, left_buffer, right_buffer, left_row_stride / (half_width ? 2 : 1), right_row_stride / (half_width ? 2 : 1));
                    jpeg_write_scanlines(&out_cinfo, out_buffer, 1);
                } else if (layout=='i') {
                    jpeg_write_scanlines(&out_cinfo, left_buffer, 1);
                    jpeg_write_scanlines(&out_cinfo, right_buffer, 1);
                } else if (layout=='a') {
                    compose_anaglyph(left_buffer, right_buffer, left_row_stride);
                    jpeg_write_scanlines(&out_cinfo, left_buffer, 1);
                }
            }
            break;
        }
        case 'o': {
            while (left_cinfo.output_scanline < left_cinfo.output_height &&
                   out_cinfo.next_scanline < out_cinfo.image_height) {
                jpeg_read_scanlines(&left_cinfo, left_buffer, 1);
                if (half_height) {
                    JSAMPROW t[1];
                    t[0] = left_buffer[1];
                    jpeg_read_scanlines(&left_cinfo, t, 1);
                    average_two_rows(left_buffer, left_row_stride);
                }
                if (half_width) {
                    half_row_length(left_buffer, left_row_stride, left_cinfo.output_components);
                }
                jpeg_write_scanlines(&out_cinfo, left_buffer, 1);
            }
            while (right_cinfo.output_scanline < right_cinfo.output_height &&
                   out_cinfo.next_scanline < out_cinfo.image_height) {
                jpeg_read_scanlines(&right_cinfo, right_buffer, 1);
                if (half_height) {
                    JSAMPROW t[1];
                    t[0] = right_buffer[1];
                    jpeg_read_scanlines(&right_cinfo, t, 1);
                    average_two_rows(right_buffer, right_row_stride);
                }
                if (half_width) {
                    half_row_length(right_buffer, right_row_stride, right_cinfo.output_components);
                }
                jpeg_write_scanlines(&out_cinfo, right_buffer, 1);
            }
            break;
        }
    }
 
 
    jpeg_finish_decompress(&left_cinfo);
    jpeg_finish_decompress(&right_cinfo);
    jpeg_finish_compress(&out_cinfo);
    fclose(outfile);
    jpeg_destroy_compress(&out_cinfo);
    jpeg_destroy_decompress(&left_cinfo);
    jpeg_destroy_decompress(&right_cinfo);

    return 0;
}
