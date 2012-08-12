/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

#ifndef __MPOTOOLS_DATA_PLACEMENT_H
#define __MPOTOOLS_DATA_PLACEMENT_H
#include <stdlib.h>

// This is used to keep track of where data is placed in an mpo/jpeg file.
// Terminology:
//     Referenced data: pieces of a file from an APP segment, that can be read
//         accessed by following IFD pointers, and tag value pointers, while
//         respecting length information. This includes IFD tag lists, tag
//         values that are larger then 4 bytes, private IFDs that are pointed to
//         by private tags with a LONG data type, etc. If there is a private tag
//         pointing to a private IFD, and it is not implemented in this
//         application, then it is considered Unreferenced data.
//     Unreferenced data: anything that can not be accessd by only following
//         IFD pointers, and tag value pointers. This includes unknown private
//         IFDs, and padding!


typedef struct data_placement_s {
   // This is a single record of referenced/unreferenced data.
    size_t start; // absolute start address
    size_t len;   // length in bytes
    char   *full_name;  // a string for referenced,
                        // or NULL for unreferenced/free space
} data_placement_t;

typedef struct data_placement_db_s {
   // This is the whole database cointaining all of the referenced/unreferenced
    data_placement_t *arr;    // The array of records
    int count;                // number of records
    int allocated;            // in number of records
    int allocation_unit;      // in number of records
    int allocation_threshold; // in number of records.
    // this should be smaller then allocation_unit
    // (if count mod allocation_unit is >= allocation_threshold then a new
    // allocation unit is allocated)
} data_placement_db_t;

int compare_data_placement(const void *a, const void *b);
   // used to sort an array of data placements
void sort_data_placement_db(data_placement_db_t *db);
   // sorts the records based on the starting position of each record.
void add_referenced_data(data_placement_db_t *db,
                           // the database containing records for all the
                           // referenced and unreferenced data in an APP segment
                         size_t              pos,
                           // absolute position of the referenced data
                         size_t              size,
                           // size of the referenced data in bytes
                         char                *full_name);
data_placement_db_t *init_data_placement_db(
   // this initializes the data_placement database.
   // At first the database contains a single record of unreferenced data:
                            size_t pos,   // starting position (usually 8)
                            size_t length // length in bytes
                         );
void free_data_placement_db(data_placement_db_t *db);

#endif
