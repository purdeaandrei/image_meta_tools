#include "data_placement.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define ERROR(...) { printf( "ERROR: " __VA_ARGS__); return; }
#define WARNING(...) { printf( "WARNING: " __VA_ARGS__); }

#define NO_NULL(val) ((val==NULL)?"NULL":val)

#define MIN(a,b) ((a)<(b)?(a):(b))

static inline void resize_data_placement_db_if_needed(data_placement_db_t *db) {
    int needed = (db->count - db->allocation_threshold) / db->allocation_unit + 2;
    if (db->allocated < needed * db->allocation_unit) {
        db->arr = realloc(db->arr, needed * db->allocation_unit * sizeof(data_placement_t));
        if ((db->arr == NULL) && (needed * db->allocation_unit != 0)) ERROR("resize_data_placement_db_if_needed: realloc failed\n");
        db->allocated = needed * db->allocation_unit;
    }
}

void add_referenced_data(data_placement_db_t *db,
                           // the database containing records for all the
                           // referenced and unreferenced data in an APP segment
                         size_t              pos,
                           // absolute position of the referenced data
                         size_t              size,
                           // size of the referenced data in bytes
                         char                *full_name) {
    int i;
    resize_data_placement_db_if_needed(db);
    if (db == NULL) ERROR("data placement database is NULL\n");
    if (db->count <= 0) ERROR("data placement database is empty\n");
    if (db->arr == NULL) ERROR("data placement database array is NULL\n");
    for (i=0;i < db->count;i++) {
        if (db->arr[i].start <= pos && db->arr[i].start + db->arr[i].len > pos){
        // if this piece of data overlaps (or is inside) an existing record
            if (db->arr[i].full_name!=NULL) {
                WARNING("Tag with full_name %s (starting at %ld) overlaps with tag "
                      "with full_name %s (start %ld, length %ld). \n", NO_NULL(full_name), pos,
                      NO_NULL(db->arr[i].full_name), db->arr[i].start, db->arr[i].len);
            } else if (db->arr[i].len + db->arr[i].start < pos + size) {
                WARNING("This tag with full_name %s overlaps with another tag?? "
                      "(free space: %ld, tagsize: %ld)\n", NO_NULL(full_name),
                      db->arr[i].len - (pos - db->arr[i].start), size)
            }
            /* First we are going to create a reference to the new data, and
               append it to the end of the array */
            db->arr[db->count].start = pos;
            db->arr[db->count].len = size;
            db->arr[db->count].full_name = strdup(full_name);
            db->count++;
            if (db->arr[i].start + db->arr[i].len > pos + size) {
                /* If there is unreferenced space at the end of the current
                   unreferenced space, after the newly created referenced record
                   then we need to create a new record for this space. */
                db->arr[db->count].len = db->arr[i].start + db->arr[i].len -
                                                                  (pos + size);
                db->arr[db->count].start = pos + size;
                db->arr[db->count].full_name = NULL; // this is unreferenced space yet
                db->count++;
            }
            /* And now we truncate the original unreferenced space so that it
            ends just before the referenced data starts. Here this record
            might become a 0-length record.  */
            if (db->arr[i].full_name == NULL) // don't truncate overlapping stuff
                db->arr[i].len = pos - db->arr[i].start;
            /* and now we need to delete other unreferenced spaces
               (this should only happen when there are overlapping tags, and it
               is useful to eliminate data from being presented as unreferenced,
               while it is really referenced) */
            for (i=0;i<db->count;i++) if (db->arr[i].full_name==NULL) {
                if (db->arr[i].start <= pos + size-1 && db->arr[i].start + db->arr[i].len > pos + size + 1) {
                    if (pos>=db->arr[i].start) WARNING("Weird: another piece of unref. space found?"
                                               "start: %ld len: %ld tag name: %s\n",
                                               db->arr[i].start, db->arr[i].len, NO_NULL(full_name));
                    size_t newstart = pos + size;
                    size_t newlength = db->arr[i].len - (newstart - db->arr[i].start);
                    if (newlength<0) newlength = 0;
                    db->arr[i].start = newstart;
                    db->arr[i].len = newlength;
                }
            }
            return; // all is fine, return.
        }
    }
    /* if we reach this point then the tag points to an address
       that is out of the ifd value space. */
    printf("count: %d\n", db->count);
    for (i=0;i < db->count;i++) printf("%ld %ld\n", db->arr[i].start, db->arr[i].len);
    ERROR("tag points to a place that is outside of the IFD value space! (%ld) \n", pos)
}

int compare_data_placement(const void *a, const void *b) {
    return ((data_placement_t *)a)->start - ((data_placement_t *)b)->start;
}

void sort_data_placement_db(data_placement_db_t *db) {
    qsort(db->arr, db->count, sizeof(data_placement_t), compare_data_placement);
}

data_placement_db_t *init_data_placement_db(
   // this initializes the data_placement database.
   // At first the database contains a single record of unreferenced data:
                            size_t pos,   // starting position (usually 8)
                            size_t length) { // length in bytes
    data_placement_db_t *db = (data_placement_db_t *)malloc(sizeof(data_placement_db_t));
    db->allocation_unit = 100;
    db->allocation_threshold = 50;
    db->allocated = 0;
    db->count = 0;
    db->arr = NULL;
    resize_data_placement_db_if_needed(db);
    db->count = 1;
    db->arr[0].start = pos;
    db->arr[0].len = length;
    db->arr[0].full_name = NULL;
    return db;
}

void free_data_placement_db(data_placement_db_t *db) {
    if (db->arr != NULL) {
        int i;
        for (i=0;i < db->count;i++) {
            free(db->arr[i].full_name);
        }
        free(db -> arr);
    }
    free(db);
}


