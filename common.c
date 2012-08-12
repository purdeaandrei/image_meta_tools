/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

#include "common.h"
#include "tags.h"

int get_bytes_per_tag_value(uint16_t tag_type, uint32_t tag_count) {
    int bytes_per_item = 1;
    switch (tag_type) {
        case TAG_TYPE_SHORT:     bytes_per_item = 2; break;
        case TAG_TYPE_LONG:
        case TAG_TYPE_SLONG:     bytes_per_item = 4; break;
        case TAG_TYPE_RATIONAL:
        case TAG_TYPE_SRATIONAL: bytes_per_item = 8; break;
    }
    return bytes_per_item * tag_count;
}

char *read_entire_file(FILE *fp, size_t unit, size_t *bytes_read) {
    char *buffer = (char *) malloc(unit);
    *bytes_read = 0;
    while (1) {
        size_t read_now = fread(buffer + *bytes_read, 1, unit, fp);
        (* bytes_read) += read_now;
        if (read_now == unit)
            buffer = realloc(buffer, (*bytes_read) + unit);
        else
            break;
    }
    return buffer;
}

void set_pointer(pointer_t *pointer, uint32_t set_to, int le) {
    if (pointer->type == p_memory) write_uint32(pointer->value.memory, set_to, le);
    else {
        writer_t *wr = pointer->value.writer.writer;
        size_t set_pos = pointer->value.writer.pos;
        size_t original_pos = wr->pos;
        wr->seek(wr, set_pos);
        if (le) {
            wr->writebyte(wr, set_to & 0xff);
            wr->writebyte(wr, (set_to >> 8) & 0xff);
            wr->writebyte(wr, (set_to >> 16) & 0xff);
            wr->writebyte(wr, (set_to >> 24) & 0xff);
        } else {
            wr->writebyte(wr, (set_to >> 24) & 0xff);
            wr->writebyte(wr, (set_to >> 16) & 0xff);
            wr->writebyte(wr, (set_to >> 8) & 0xff);
            wr->writebyte(wr, set_to & 0xff);
        }
        wr->seek(wr, original_pos); // restore original position of the writer
    }
}

pointer_t *mem_pointer(void *p) {
    pointer_t *t = (pointer_t *)malloc(sizeof(pointer_t));
    t->type = p_memory;
    t->value.memory = p;
    return t;
}

pointer_t *writer_pointer(writer_t *wr, size_t pos) {
    pointer_t *t = (pointer_t *)malloc(sizeof(pointer_t));
    t->type = p_writer;
    t->value.writer.writer = wr;
    t->value.writer.pos = pos;
    return t;
}

void write_uint32(void *dest, uint32_t value, int le) {
    unsigned char *d = (unsigned char *)dest;
    if (le) {
        d[0] = value & 0xff;
        d[1] = (value >> 8) & 0xff;
        d[2] = (value >> 16) & 0xff;
        d[3] = (value >> 24) & 0xff;
    } else {
        d[3] = value & 0xff;
        d[2] = (value >> 8) & 0xff;
        d[1] = (value >> 16) & 0xff;
        d[0] = (value >> 24) & 0xff;
    }
}
void write_uint16(void *dest, uint16_t value, int le) {
    unsigned char *d = (unsigned char *)dest;
    if (le) {
        d[0] = value & 0xff;
        d[1] = (value >> 8) & 0xff;
    } else {
        d[1] = value & 0xff;
        d[0] = (value >> 8) & 0xff;
    }
}

void add_offset(data_object_t *o, char *full_name, int offset) {
    if (o->nr_my_offsets==0) {
        o->my_offsets = malloc(sizeof(name_and_offset_t));
        o->nr_my_offsets = 1;
    } else {
        (o->nr_my_offsets) ++;
        o->my_offsets = realloc(o->my_offsets, sizeof(name_and_offset_t) * o->nr_my_offsets);
    }
    o->my_offsets[o->nr_my_offsets-1].full_name = full_name;
    o->my_offsets[o->nr_my_offsets-1].offset = offset;
}

