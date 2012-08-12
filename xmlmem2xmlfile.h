/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

#ifndef __MPOTOOLS_XMLMEM2XMLFILE_H
#define __MPOTOOLS_XMLMEM2XMLFILE_H

/* this file contains stuff used to write an mxml memory structure into an xml file */

void xm2xf_save_xml_filename(mxml_node_t *xml, char * fn, int inline_binary);
void xm2xf_save_xml_file(mxml_node_t *xml, FILE *f);
char *xm2xf_save_xml_mem(mxml_node_t *xml);


#endif

