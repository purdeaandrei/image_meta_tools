/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

#ifndef __MPOTOOLS_XMLMEM2IMAGE_H
#define __MPOTOOLS_XMLMEM2IMAGE_H

#include <stdio.h>
#include <stdint.h>
#include <mxml.h>
#include "common.h"
#include "tags.h"
#include "mp_entry.h"

/* this file contains stuff used to write an mxml memory structure into an image file */

void xm2i_save_image_filename(mxml_node_t *xml, char * fn);
void xm2i_save_image_file(mxml_node_t *xml, FILE *f);
void *xm2i_save_image_mem(mxml_node_t *xml);

void xm2i_write_binary(writer_t *wr, mxml_node_t *image);

int make_long_obj(uint32_t value, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le);
int make_long_obj_with_ptr(uint32_t value, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le);
int make_short_obj(uint16_t value, pointer_t *pointer, const char *full_name, data_object_t **dobj, int le);
int make_2_short_obj(uint16_t value, uint16_t value2, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le);
int make_3_short_obj(uint16_t value, uint16_t value2, uint16_t value3, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le);
int make_4_short_obj(uint16_t value, uint16_t value2, uint16_t value3, uint16_t value4, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le);
int make_dobj_from_buffer(void *buf, size_t size, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le);
int make_dobj_from_mem_writer(writer_t *wr, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le);
void interpret_single_tag_value(writer_t *wr, const char *val_attr, int tagtype, uint32_t *count, int le);
void interpret_ifd_chain(mxml_node_t *marker_node, pointer_t *prev_pointer, char *full_name, writer_t *objdb, custom_tag_t *tags, size_t endian_pos, int le);

extern mp_entry_db_t mp_entry_db;
extern writer_t *write_offsets;
extern char *write_offsets_prefix;

#endif

