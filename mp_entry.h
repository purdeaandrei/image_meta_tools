#ifndef __MPOTOOLS_MP_ENTRY_H
#define __MPOTOOLS_MP_ENTRY_H

#include "common.h"
#include "referenced_images.h"

typedef struct mp_entry_s {
    uint32_t offset; // sort by this
    pointer_t *offset_pointer;
    pointer_t *size_pointer;
    uint32_t end_pos; // endian position.
    int le; // little endian??
} mp_entry_t;

typedef struct mp_entry_db_s {
    int count;
    mp_entry_t arr[MAX_IMAGES]; // contains absolute positions!!!
} mp_entry_db_t;


void add_mp_entry(mp_entry_db_t *db, uint32_t old_offset, pointer_t *offset, pointer_t *size, uint32_t end_pos, int le);
void sort_mp_entry_db(mp_entry_db_t *db);


#endif
