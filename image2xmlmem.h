/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

#ifndef __MPOTOOLS_IMAGE2XMLMEM_H
#define __MPOTOOLS_IMAGE2XMLMEM_H

/* This file contains stuff for reading MPO/JPEG files and turning them
   into an mxml memory structure */

#include <mxml.h>
#include <stdint.h>
#include "markers.h"
#include "tags.h"

mxml_node_t *i2xm_load_mem(void   *data,   // pointer to the mpo/jpg file in memory
                           size_t size,    // size of the mpo/jpg file in memory
                           int    interpret_concatenated); // whether this function should look for unreferenced images
mxml_node_t *i2xm_load_file(FILE * fp, int interpret_concatenated); // same as i2x_load_mem, but read from FILE *
mxml_node_t *i2xm_load_filename(char * fn, int interpret_concatenated); // same as i2x_load_mem, but read from filename

void i2xm_read_single_value(unsigned char *data,        // pointer to the mpo/jpg file in memory
                            size_t        *pos,         // the absolute position of the data to be read
                            mxml_node_t   *destination, // the parent node in which to place a "value" attribute
                            uint16_t      type,         // TAG_TYPE_*
                            int           le);          // Endian (1 = little endian, 0 = big endian)

void i2xm_read_array_value(unsigned char *data,        // pointer to the mpo/jpg file in memory
                           size_t        *pos,         // the absolute position of the data to be read
                           uint32_t      count,        // number of elements to read
                           mxml_node_t   *destination, // the parent node in which to place a "value" attribute
                           uint16_t      type,         // TAG_TYPE_*
                           /* the destination node will get sub-elements for each array element,
                              and this sub-element will have a "value" attribute.
                              Exceptions:
                               -- TAG_TYPE_ASCII     - will be read as a single string
                               -- TAG_TYPE_UNDEFINED - will be read as a custom binary node. */
                           int           le);          // Endian (1 = little endian, 0 = big endian)

uint32_t i2xm_read_uint32(unsigned char *data, size_t pos, int le);
uint16_t i2xm_read_uint16(unsigned char *data, size_t pos, int le);
#define i2xm_read_int16(a,b,c) ((int16_t) i2xm_read_uint16((a),(b),(c)))
#define i2xm_read_int32(a,b,c) ((int32_t) i2xm_read_uint32((a),(b),(c)))

void i2xm_read_ifds(unsigned char       *data,        // pointer to the mpo/jpg file in memory
                    size_t              endian_pos,   // the absolute position of the 4-byte sequence that represents the current endian
                    size_t              size,         // size of the marker starting from endian_pos (inclusive)
                    uint32_t            first_ifd_offset, // the relative position of the first ifs (relative to the endian bytes)
                    mxml_node_t         *parent,      // the parent node in the output xml tree
                    custom_tag_t        *tags,        // the possible tags that can appear in these ifds
                    data_placement_db_t *dp,
                    char                *full_name,   // used to construct a full name for each tag, including parent ifds.
                    referenced_images_t *images,      // this is where the MP_Entry tags will place referenced images so we can later retrieve them
                    int                 read_single_ifd, // read only a single ifd, don't follow pointer to next ifd
                    int                 le) ; // Endian (1 = little endian, 0 = big endian)

void i2xm_read_binary(unsigned char *data,     // pointer to the mpo/jpg file in memory
                      size_t        pos,       // absolute position of the data interpreted as raw binary
                      size_t        size,      // size of the data interpreted as raw binary
                      mxml_node_t   *parent);  // In this parent node a new child element will be created, named "binary",
                                               // and the binary node will contain a custom child which contains a binary_t structure.

#endif
