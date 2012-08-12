/* this file contains stuff used to read an mxml memory structure from an xml file.
   this is mainly useful because of the special cases involved with binary data.*/


#include "xmlfile2xmlmem.h"
#include "common.h"

static int count_lines(char *s) {
    int i = 0;
    while (*s!=0) { if (*s=='\n') i++; s++; }
    return i;
}

static void parse_hexdump_line(char *line, char *eol, char *data, size_t *size) {
    while (*line!=':' && *line!=0) line++;
    if (*line==0) return;
    line ++;
    // make sure we don't accidentally read part of the text:
    if (line + 3*16<eol) *(line + 3*16) = 0; 
    unsigned int val;
    int chars, i;
    for (i=0;i<16;i++) {
        if (1==sscanf(line, "%x%n", &val, &chars)) {
            line += chars;
            data[*size] = val;
            (*size) ++;
        } else break;
    }
}

static char *parse_hexdump(char *hex, size_t *size, unsigned int *offset) {
    *size = 0;
    int alloc = (count_lines(hex)+1) * 16;
    char *data = malloc(alloc);
    sscanf(hex, "%x", offset);
    while (*hex!=0) {
        char *eol = hex;
        while (*eol!='\n' && *eol!=0) eol++;
        if (*eol==0) {
            parse_hexdump_line(hex, eol, data, size);
            break;
        } else {
            *eol = 0;
            parse_hexdump_line(hex, eol, data, size);
            hex = eol + 1;
        }
    }
    return data;
}

static void delete_all_children(mxml_node_t *node) {
    mxml_node_t *n = node->child;
    while (n!=NULL) {
        mxml_node_t *next = n->next;
        mxmlDelete(n);
        n = next;
    }
}

mxml_node_t *xf2xm_load_filename(char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp==NULL) {
        printf("ERROR: can't open file: %s\n", filename);
        return NULL;
    }
    mxml_node_t *tree = mxmlLoadFile(NULL, fp, MXML_OPAQUE_CALLBACK);
    fclose(fp);
    /* First let's remove all whitespace-only opaque nodes */
    mxml_node_t *node;
    for (node = tree; NULL!=node; node = mxmlWalkNext(node, tree, MXML_DESCEND)) {
        int exitloop = 0;
        while (exitloop == 0) {
            exitloop = 1;
            if (node->type==MXML_OPAQUE) {
                char *t = node->value.opaque;
                while (*t!=0) {
                    if (*t!=' ' && *t!='\t' && *t!='\n' && *t!='\r') break;
                    t++;
                }
                if (*t==0) {
                    mxml_node_t *oldnode = node;
                    node = mxmlWalkNext(node, tree, MXML_DESCEND);
                    mxmlDelete(oldnode);
                    if (node!=NULL) exitloop = 0;
                }
            }
        }
        if (node == NULL) break;
    }
    /* Now let's handle <include> nodes and turn them into custom <binary> nodes */
    for (node = tree; NULL!=node; node = mxmlWalkNext(node, tree, MXML_DESCEND)) {
        if (node->type == MXML_ELEMENT && 0==strcmp(node->value.element.name, "include")) {
            const char *path = mxmlElementGetAttr(node, "path");
            FILE *f = fopen(path, "rb");
            if (f==NULL) {
                printf("ERROR: can't open included file: %s\n", path);
                return NULL;
            }
            size_t size;
            char *data = read_entire_file(f, 102400, &size);
            fclose(f);
            mxmlElementDeleteAttr(node, "path");
            mxmlSetElement(node, "binary");

            binary_t *piece = (binary_t *) malloc(size + 2 * sizeof(size_t));
            piece->size = size;
            piece->offset = 0; // offset information unavailable, but we don't use it anyway
            memcpy(piece->data, data, size);
            mxmlNewCustom(node, piece, free);
            free(data);
        }
    }
    /* Now let's handle <hexdump> nodes and turn them into custom <binary> nodes */
    for (node = tree; NULL!=node; node = mxmlWalkNext(node, tree, MXML_DESCEND)) {
        if (node->type == MXML_OPAQUE && 0==strcmp(node->parent->value.element.name, "hexdump")) {
            size_t size;
            unsigned int offset;
            char *data = parse_hexdump(node->value.opaque, &size, &offset);
            node = node -> parent; // so that mxmlWalkNext can easily continue.
            mxmlSetElement(node, "binary");
            delete_all_children(node);
            binary_t *piece = (binary_t *) malloc(size + 2 * sizeof(size_t));
            piece->size = size;
            piece->offset = offset;
            memcpy(piece->data, data, size);
            mxmlNewCustom(node, piece, free);
            free(data);
        }
    }
    return tree;
}
