/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

#ifndef __MPOTOOLS_XMLFILE2XMLMEM_H
#define __MPOTOOLS_XMLFILE2XMLMEM_H

/* this file contains stuff used to read an mxml memory structure from an xml file.
   this is mainly useful because of the special cases involved with binary data.*/

#include <mxml.h>

mxml_node_t *xf2xm_load_filename(char *filename);

#endif
