#include <stdio.h>
#include "image2xmlmem.h"
#include "xmlmem2xmlfile.h"

void help(int argc, char ** argv) {
    printf("Usage: %s input.mpo/input.jpg output.xml [ flags ]\n", argv[0]);
    printf("-i: non-parsed data: include data inline\n");
    printf("-f: non-parsed data: generate binary files, and refer to the from XML (default)\n");
    printf("-c: try to find unreferenced concatenated images (default)\n");
    printf("-n: don't try to find unreferenced images (they remain binary data) (recommended)\n");
}

int main(int argc, char ** argv) {
    if (argc<3) {
        help(argc, argv);
        return -1;
    }
    int i, interpret_concatenated = 1, inline_binary = 0;
    for (i=3;i<argc;i++) {
        if (argv[i][0]!='-') {
            printf("argument nr %d (%s) should be a flag (start with -)!\n", i, argv[i]);
            return -1;
        }
        switch (argv[i][1]) {
            case 'f': break; // ignore, default!
            case 'i': inline_binary = 1; break;
            case 'c': break; // ignore, default!
            case 'n': interpret_concatenated = 0; break;
        }
    }
    mxml_node_t *image = i2xm_load_filename(argv[1], interpret_concatenated);
    if (image==NULL) {
        printf("ERROR: there was a problem opening the image file.\n");
        return -1;
    }
    xm2xf_save_xml_filename(image, argv[2], inline_binary);
    return 0;
}
