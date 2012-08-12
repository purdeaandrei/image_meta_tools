#ifndef __MPOTOOLS_XMLFILE2XMLMEM_H
#define __MPOTOOLS_XMLFILE2XMLMEM_H

/* this file contains stuff used to read an mxml memory structure from an xml file.
   this is mainly useful because of the special cases involved with binary data.*/

#include <mxml.h>

mxml_node_t *xf2xm_load_filename(char *filename);

#endif
