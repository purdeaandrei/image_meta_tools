#include <stdio.h>
#include <string.h>
#include "writers.h"
#include "common.h"

void file_writebytes(writer_t *wr, const void *buffer, size_t length) {
    FILE *f = (FILE *)wr->private;
    fwrite(buffer, length, 1, f);
    wr->pos += length;
    if (wr->pos > wr->size) wr->size = wr->pos;
}
void file_writebyte(writer_t *wr, unsigned char byte) {
    FILE *f = (FILE *)wr->private;
    fputc(byte, f);
    wr->pos++;
    if (wr->pos > wr->size) wr->size = wr->pos;
}
void file_seek(writer_t *wr, size_t position) {
    FILE *f = (FILE *)wr->private;
    fseek(f, position, SEEK_SET);
    wr->pos = position;
}

void init_file_writer(writer_t *writer, FILE *f) {
    writer->writebytes = file_writebytes;
    writer->writebyte = file_writebyte;
    writer->seek = file_seek;
    writer->pos = ftell(f);
    fseek(f, 0, SEEK_END);
    writer->size = ftell(f);
    fseek(f, writer->pos, SEEK_SET);
    writer->private = f;
}
void free_file_writer(writer_t *writer) { /* nothing to do here */ }

typedef struct mem_priv_s {
    size_t allocation_unit; // number of new free allocated bytes that will remain after a write when not enaugh bytes were allocated
    size_t allocated;       // number of bytes allocated so far
    void   *buffer;         // pointer to the malloc-ed data
} mem_priv_t;

void mem_writebyte(writer_t *wr, unsigned char byte) {
    mem_priv_t *p = (mem_priv_t *) wr->private;
    if (wr->pos + 1 > p->allocated) {
        size_t newalloc = wr->pos + 1 + p->allocation_unit;
        p->buffer = realloc(p->buffer, newalloc);
        p->allocated = newalloc;
    }
    ((unsigned char *)(p->buffer))[wr->pos] = byte;
    wr->pos ++;
    if (wr->pos > wr->size) wr->size = wr->pos;
}

void mem_writebytes(writer_t *wr, const void *append, size_t len) {
    mem_priv_t *p = (mem_priv_t *) wr->private;
    if (wr->pos + len > p->allocated) {
        size_t newalloc = wr->pos + len + p->allocation_unit;
        p->buffer = realloc(p->buffer, newalloc);
        p->allocated = newalloc;
    }
    memcpy((char *)(p->buffer) + wr->pos, append, len);
    wr->pos += len;
    if (wr->pos > wr->size) wr->size = wr->pos;
}

void mem_seek(writer_t *wr, size_t position) {
    wr->pos = position;
}

void init_memory_writer(writer_t *writer, size_t initial_size, size_t allocation_unit) {
    mem_priv_t *p = (mem_priv_t *) malloc(sizeof(mem_priv_t));
    p->allocated = initial_size;
    p->allocation_unit = allocation_unit;
    p->buffer = malloc(initial_size);
    writer->private = p;
    writer->seek = mem_seek;
    writer->writebyte = mem_writebyte;
    writer->writebytes = mem_writebytes;
    writer->pos = 0;
    writer->size = 0;
} 
void *free_memory_writer(writer_t *writer) {
    void *buffer = ((mem_priv_t *) writer->private)->buffer;
    free(writer->private);
    return buffer;
}

void write_uint16_writer(writer_t *wr, uint16_t value, int le) {
    if (le) {
        wr->writebyte(wr, value & 0xff);
        wr->writebyte(wr, (value >> 8) & 0xff);
    } else {
        wr->writebyte(wr, (value >> 8) & 0xff);
        wr->writebyte(wr, value & 0xff);
    }
}

void write_uint32_writer(writer_t *wr, uint32_t value, int le) {
    if (le) {
        wr->writebyte(wr, value & 0xff);
        wr->writebyte(wr, (value >> 8) & 0xff);
        wr->writebyte(wr, (value >> 16) & 0xff);
        wr->writebyte(wr, (value >> 24) & 0xff);
    } else {
        wr->writebyte(wr, (value >> 24) & 0xff);
        wr->writebyte(wr, (value >> 16) & 0xff);
        wr->writebyte(wr, (value >> 8) & 0xff);
        wr->writebyte(wr, value & 0xff);
    }
}

void write_string_writer(writer_t *wr, const char *str) {
    wr->writebytes(wr, str, strlen(str));
}

void *get_mem_writer_pointer_to_current_position(writer_t *writer) {
    return (char *) (((mem_priv_t *)writer->private)->buffer) + writer->pos;
}
