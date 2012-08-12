#ifndef XMLMEM_DATA_SOURCE_MANAGER_H
#define XMLMEM_DATA_SOURCE_MANAGER_H
#include <stdio.h>
#include <jpeglib.h>
#include <mxml.h>

void jpeg_xmlmem_src(j_decompress_ptr cinfo, mxml_node_t *imagenode);

#endif
