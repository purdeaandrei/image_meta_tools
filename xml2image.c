#include <stdio.h>
#include "xmlfile2xmlmem.h"
#include "xmlmem2xmlfile.h"
#include "xmlmem2image.h"

void help(int argc, char ** argv) {
    printf("Usage: %s input.xml output.mpo/output.jpg [ output_offsets.h ] [ prefix for defintions in .h file ]\n", argv[0]);
    //printf("-a: force alignment\n"); // TODO
}

int main(int argc, char ** argv) {
    if (argc<3) {
        help(argc, argv);
        return -1;
    }
    char *offsets_fn = NULL;
    writer_t ofs; FILE *f_ofs = NULL;
    if (argc>=4) {
        offsets_fn = argv[3];
        f_ofs = fopen(offsets_fn, "w");
        init_file_writer(&ofs, f_ofs);
        write_offsets = &ofs;
        if (argc>=5) {
            write_offsets_prefix = argv[4];
        } else {
            write_offsets_prefix = "";
        }
    } else {
        write_offsets = NULL;
    }
    int i;
    for (i=5;i<argc;i++) {
        if (argv[i][0]!='-') {
            printf("argument nr %d (%s) should be a flag (start with -)!\n", i, argv[i]);
            return -1;
        }
        switch (argv[i][1]) {
            case 'a': break; // TODO
            default: break;
        }
    }
    mxml_node_t *tree = xf2xm_load_filename(argv[1]);
    if (tree==NULL) {
        printf("ERROR: there was an error opening the xml file\n");
        return -1;
    }
    xm2i_save_image_filename(tree, argv[2]);
    if (f_ofs!=NULL && write_offsets!=NULL) {
        free_file_writer(write_offsets);
        write_offsets = NULL;
        fclose(f_ofs);
    }
    return 0;
}
