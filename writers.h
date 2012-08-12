#ifndef __MPOTOOLS_WRITERS_H
#define __MPOTOOLS_WRITERS_H
#include <stdio.h>
#include <stdint.h>
typedef struct writer_s {
    void (*writebytes)(struct writer_s *self, const void *buffer, size_t length);
    void (*writebyte)(struct writer_s *self, unsigned char byte);
    void (*seek)(struct writer_s *self, size_t position);
    size_t pos; // the position where the cursor currently is
    size_t size; // size of the data written so far
    void *private;
} writer_t;

void init_file_writer(writer_t *writer, FILE *f);
void free_file_writer(writer_t *writer);

void init_memory_writer(writer_t *writer, size_t initial_size, size_t allocation_unit);
void *free_memory_writer(writer_t *writer); // this frees only hidden structures. returns a malloc-d pointer
// size of data is in writer->size

void write_uint16_writer(writer_t *wr, uint16_t value, int le);
void write_uint32_writer(writer_t *wr, uint32_t value, int le);

void write_string_writer(writer_t *wr, const char *str);

void *get_mem_writer_pointer_to_current_position(writer_t *writer);

    
#endif
