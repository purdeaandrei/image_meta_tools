#include "mp_entry.h"


static int compare_mp_entry(const void *a, const void *b) {
    return (((mp_entry_t *)a)->offset) - (((mp_entry_t *)b)->offset);
}


void add_mp_entry(mp_entry_db_t *db, uint32_t old_offset, pointer_t *offset, pointer_t *size, uint32_t end_pos, int le) {
    db->arr[db->count].offset = old_offset;
    db->arr[db->count].offset_pointer = offset;
    db->arr[db->count].size_pointer = size;
    db->arr[db->count].end_pos = end_pos;
    db->arr[db->count].le = le;
    db->count = db->count + 1;
}


void sort_mp_entry_db(mp_entry_db_t *db) {
    qsort(db->arr, db->count, sizeof(mp_entry_t), compare_mp_entry);
}
