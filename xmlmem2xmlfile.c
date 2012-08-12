/* this file contains stuff used to write an mxml memory structure into an xml file */

#include <mxml.h>
#include "xmlmem2xmlfile.h"
#include "common.h"

static char *custom_save_hexdump(mxml_node_t *node) {
    if (strcmp(node->parent->value.element.name, "hexdump")==0) {
        binary_t *t = (binary_t *)(node->value.custom.data);
        size_t size = t->size;
        size_t offset = t->offset;
        unsigned char *data = t->data;
        char *encoded = (char *) malloc(((size + 15) >> 4) * (8 + 1 + 3 * 16 + 1 + 16 + 1) + 1);
        char *pos = encoded;
        pos[0] = 0; // if it's empty
        size_t i;
        for (i=0;i<size;i++) {
            if ((i & 0x0F) == 0) {
                pos += snprintf(pos, 10, "%08lx:", offset + i);
            }
            pos += snprintf(pos, 4, " %02x", data[i]);
            if (i == size - 1) {
                size_t j = (i & 0x0F);
                for(;j<15;j++) pos += snprintf(pos, 4, "   "); // pad the last line with spaces
            }
            if (((i & 0x0F) == 0x0F) || (i == size - 1)) {
                char strpr[17];
                size_t j;
                for (j=(i & 0x0F);j<16;j--) {
                    strpr[j] = data[i - ((i & 0x0F) - j)];
                    if (!isprint(strpr[j])) strpr[j]='.';
                    switch (strpr[j]) {
                        case '>':
                        case '<':
                        case '"':
                        case '&':
                            strpr[j] = '.';
                    }
                }
                strpr[(i & 0x0f) + 1] = 0;
                pos+=snprintf(pos, 16 + 3, " %s\n", strpr);
            }
        }
        return encoded;
    }
    return strdup("Error: unknown parent for custom node (looking for: hexdump)\n");
}

static char *custom_save_include(mxml_node_t *node) {
    if (strcmp(node->parent->value.element.name, "include")==0) {
        const char *bin_filename = mxmlElementGetAttr(node->parent, "path");
        FILE *f = fopen(bin_filename, "wb");
        if (f==NULL) printf("Error opening for writing: %s\n", bin_filename);
        binary_t *cdata = (binary_t *) node->value.custom.data;
        if (cdata->size > 0) {
            if (1 != fwrite(cdata->data, cdata->size, 1, f)) {
                printf("Error writing %ld bytes to file '%s'\n", cdata->size, bin_filename);
            }
        } else printf("Warning: weird binary data size: %ld\n", cdata->size);
        fclose(f);
        return strdup("");
    }
    return strdup("Error: unknown parent for custom node (looking for: include)\n");
}

// this is the callback function used by mxml to add white space for readability:
static const char * whitespace_cb(mxml_node_t *node, int where) {
    if (node->type == MXML_ELEMENT && 0==strcmp(node->value.element.name, "include")) {
        // make sure includes only occuppy one line
        if (where == MXML_WS_AFTER_OPEN || where == MXML_WS_BEFORE_CLOSE) return "";
    }
    if (where == MXML_WS_AFTER_CLOSE || where == MXML_WS_AFTER_OPEN) return "\n";
    static char spaces[100];
    int depth = 0;
    while (node->parent!=NULL) {
        if (depth < 99) {
            spaces[depth] = '\t';
            spaces[depth + 1] = 0;
        }
        node = node->parent;
        depth++;
    }
    return spaces + 1;
}

void convert_hexdump(mxml_node_t *xml) {
    mxml_node_t *node = xml;
    do {
        if (node->type==MXML_ELEMENT && 0==strcmp(node->value.element.name, "binary"))
            mxmlSetElement(node, "hexdump");
    } while (NULL!=(node = mxmlWalkNext(node, xml, MXML_DESCEND)));
}

void revert_hexdump(mxml_node_t *xml) {
    mxml_node_t *node = xml;
    do {
        if (node->type==MXML_ELEMENT && 0==strcmp(node->value.element.name, "hexdump"))
            mxmlSetElement(node, "binary");
    } while (NULL!=(node = mxmlWalkNext(node, xml, MXML_DESCEND)));
}

void convert_include(mxml_node_t *xml, char *output_filename) {
    mxml_node_t *node = xml;
    int last_file_index = 0;
    do {
        if (node->type==MXML_ELEMENT && 0==strcmp(node->value.element.name, "binary")) {
            mxmlSetElement(node, "include");
            char *bin_filename = (char *) malloc(strlen(output_filename) + strlen(".0000.bin") + 1);
            snprintf(bin_filename, strlen(output_filename) + strlen(".0000.bin") + 1, "%s.%04u.bin", output_filename, last_file_index++);
            mxmlElementSetAttrf(node, "path", "%s", bin_filename);
            free(bin_filename);
        }
    } while (NULL!=(node = mxmlWalkNext(node, xml, MXML_DESCEND)));
}

void revert_include(mxml_node_t *xml) {
    mxml_node_t *node = xml;
    do {
        if (node->type==MXML_ELEMENT && 0==strcmp(node->value.element.name, "include")) {
            mxmlSetElement(node, "binary");
            mxmlElementDeleteAttr(node, "path");
        }
    } while (NULL!=(node = mxmlWalkNext(node, xml, MXML_DESCEND)));
}

void xm2xf_save_xml_filename(mxml_node_t *xml, char * fn, int inline_binary) {
    FILE *f = fopen(fn, "w");
    if (f==NULL) {
        ERROR("Can't open file %s for writing!\n", fn);
    }
    mxmlSetWrapMargin(0);
    if (inline_binary) {
        mxmlSetCustomHandlers(NULL, custom_save_hexdump);
        convert_hexdump(xml);
    } else {
        mxmlSetCustomHandlers(NULL, custom_save_include);
        convert_include(xml, fn);
    }
    mxmlSaveFile(xml, f, whitespace_cb);
    if (inline_binary) {
        revert_hexdump(xml);
    } else {
        revert_include(xml);
    }
    fclose(f);
}
void xm2xf_save_xml_file(mxml_node_t *xml, FILE *f) {
    mxmlSetWrapMargin(0);
    mxmlSetCustomHandlers(NULL, custom_save_hexdump);
    convert_hexdump(xml);
    mxmlSaveFile(xml, f, whitespace_cb);
    revert_hexdump(xml);
}
char *xm2xf_save_xml_mem(mxml_node_t *xml) {
    mxmlSetWrapMargin(0);
    mxmlSetCustomHandlers(NULL, custom_save_hexdump);
    convert_hexdump(xml);
    char *data = mxmlSaveAllocString(xml, whitespace_cb);
    revert_hexdump(xml);
    return data;
}






