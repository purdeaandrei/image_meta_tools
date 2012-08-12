#ifndef __MPOTOOLS_COMMON_H
#define __MPOTOOLS_COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "writers.h"

struct binary_s {
    size_t size;
    size_t offset;
    unsigned char data[];
};
typedef struct binary_s binary_t;

int get_bytes_per_tag_value(uint16_t tag_type,    // TAG_TYPE_*
                            uint32_t tag_count);

char *read_entire_file(FILE *fp, size_t unit, size_t *bytes_read);

#define ERROR(...) { printf( "ERROR: " __VA_ARGS__); return; }
#define WARNING(...) { printf( "WARNING: " __VA_ARGS__); }

#define min(a, b) (((a)>(b))?(b):(a))
#define max(a, b) (((a)<(b))?(b):(a))

#define FOR_CHILDREN(variable, parent) mxml_node_t *variable = parent->child; \
                                       for( ; variable != NULL; variable = variable -> next)
#define IS_ELEMENT(node, namep) ((node->type == MXML_ELEMENT) && (strcmp(node->value.element.name, namep)==0))
#define IS_ELEMENT_ATTR(node, attrname, attrvalue) ((node->type == MXML_ELEMENT) && (mxmlElementGetAttr(node, attrname)!=NULL) && (strcmp(mxmlElementGetAttr(node, attrname), attrvalue)==0))
#define IS_ELEMENT_ATTR_NULL(node, attrname) ((node->type == MXML_ELEMENT) && (mxmlElementGetAttr(node, attrname)==NULL))
#define ERROR_DUMPNODE(node, ...) { printf( "ERROR: " __VA_ARGS__);                                      \
                                    if (node->type==MXML_ELEMENT) {                                      \
                                        printf(" (current element: %s)\n", node->value.element.name);    \
                                    } else if (node->type==MXML_TEXT) {                                  \
                                        printf(" (current string node: %s)\n", node->value.text.string); \
                                    } else if (node->type==MXML_OPAQUE) {                                \
                                        printf(" (current opaque node: %s)\n", node->value.opaque);      \
                                    } else if (node->type==MXML_INTEGER) {                               \
                                        printf(" (current integer node: %d)\n", node->value.integer);    \
                                    } else if (node->type==MXML_CUSTOM) {                                \
                                        printf(" (I found a custom node)\n");                            \
                                    } else printf(" (node type: %d)\n", node->type);                     \
                                    return;                                                              \
                                  }

#define CONCP(dest, pat, s, ...) {                   \
    int maxb = strlen(pat) + strlen(s) + 1 + 10;     \
    (dest) = (char *) malloc(maxb);                  \
    snprintf((dest), maxb, (pat), (s), __VA_ARGS__); \
}
#define CONCS(dest, a, b) {                    \
    int maxb = strlen(a) + strlen(b) + 1 + 1;  \
    (dest) = (char *) malloc(maxb);            \
    snprintf((dest), maxb, "%s/%s", a, b);     \
}


typedef struct pointer_s {
    enum pointertype_e {
        p_writer,
        p_memory
    } type;
    union pointer_u {
        struct writer_and_pos_s {
            writer_t *writer;
            size_t pos;
        } writer;
        void *memory;
    } value;
} pointer_t;

void set_pointer(pointer_t *pointer, uint32_t set_to, int le);
pointer_t *mem_pointer(void *p);
pointer_t *writer_pointer(writer_t *wr, size_t pos);


void write_uint32(void *dest, uint32_t value, int le);
void write_uint16(void *dest, uint16_t value, int le);

typedef struct name_and_offset_s {
	char *full_name;
	int offset;
} name_and_offset_t;

typedef struct data_object_s {
    char *full_name;    // full name of the object
    int written;        // has it been written to an outside storage?
    pointer_t *pointer; // where is the pointer that points to this data object? (the parent)
    size_t size;        // number of bytes in buffer
    void *buffer;       // the in-memory buffer that stores the given data.
    int nr_my_offsets;
    name_and_offset_t *my_offsets; // will contain later-modifiable data offsets
    int nr_my_pointers;
    pointer_t *my_pointers[]; // these are the pointers in the data object to other children
} data_object_t;

void add_offset(data_object_t *o, char *full_name, int offset);

#define STORE_OBJECT(objdb, object) objdb->writebytes(objdb, &object, sizeof(data_object_t *))
#define FREE_OBJECT(object) {      \
        free((object)->buffer);    \
        free((object)->full_name); \
        free((object)->pointer);   \
        free((object));            \
        while ((object)->nr_my_offsets>0) {  \
            (object)->nr_my_offsets --;      \
            free((object)->my_offsets[(object)->nr_my_offsets].full_name); \
        } \
        }

#endif
