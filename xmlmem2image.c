/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

#include <string.h>
#include "mxml.h"
#include "xmlmem2image.h"
#include "common.h"
#include "markers.h"
#include "writers.h"
#include "tags.h"

void xm2i_write_binary(writer_t *wr, mxml_node_t *image) {
    if (image->child->type != MXML_CUSTOM) ERROR_DUMPNODE(image, "this node should have a custom child!\n");
    binary_t *b = (binary_t *)image->child->value.custom.data;
    wr->writebytes(wr, b->data, b->size);
}

int make_dobj_from_buffer(void *buf, size_t size, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le) {
    data_object_t *dobj = (data_object_t *) malloc(sizeof(data_object_t));
    dobj->full_name = strdup(full_name);
    dobj->written = 0;
    dobj->pointer = pointer;
    dobj->nr_my_pointers = 0;
    dobj->nr_my_offsets = 0;
    dobj->size = size;
    dobj->buffer = buf;
    *dobjd = dobj;
    return 1;
}

int make_dobj_from_mem_writer(writer_t *wr, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le) {
    size_t size = wr->size;
    return make_dobj_from_buffer(free_memory_writer(wr), size, pointer, full_name, dobjd, le);
}

int make_long_obj_with_ptr(uint32_t value, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le) {
    data_object_t *dobj = (data_object_t *) malloc(sizeof(data_object_t) + sizeof(pointer_t));
    dobj->full_name = strdup(full_name);
    dobj->written = 0;
    dobj->pointer = pointer;
    dobj->size = 4;
    unsigned char *buf = (unsigned char *) malloc(dobj->size);
    write_uint32(buf, value, le);
    dobj->buffer = buf;
    dobj->nr_my_pointers = 1;
    dobj->nr_my_offsets = 0;
    dobj->my_pointers[0] = mem_pointer(buf);
    *dobjd = dobj;
    return 1;
}


int make_long_obj(uint32_t value, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le) {
    data_object_t *dobj = (data_object_t *) malloc(sizeof(data_object_t));
    dobj->full_name = strdup(full_name);
    dobj->written = 0;
    dobj->pointer = pointer;
    dobj->nr_my_pointers = 0;
    dobj->nr_my_offsets = 0;
    dobj->size = 4;
    unsigned char *buf = (unsigned char *) malloc(dobj->size);
    write_uint32(buf, value, le);
    dobj->buffer = buf;
    *dobjd = dobj;
    return 1;
}

int make_short_obj(uint16_t value, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le) {
    data_object_t *dobj = (data_object_t *) malloc(sizeof(data_object_t));
    dobj->full_name = strdup(full_name);
    dobj->written = 0;
    dobj->pointer = pointer;
    dobj->nr_my_pointers = 0;
    dobj->nr_my_offsets = 0;
    dobj->size = 2;
    unsigned char *buf = (unsigned char *) malloc(dobj->size);
    write_uint16(buf, value, le);
    dobj->buffer = buf;
    *dobjd = dobj;
    return 1;
}

int make_2_short_obj(uint16_t value, uint16_t value2, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le) {
    data_object_t *dobj = (data_object_t *) malloc(sizeof(data_object_t));
    dobj->full_name = strdup(full_name);
    dobj->written = 0;
    dobj->pointer = pointer;
    dobj->nr_my_pointers = 0;
    dobj->nr_my_offsets = 0;
    dobj->size = 4;
    unsigned char *buf = (unsigned char *) malloc(dobj->size);
    write_uint16(buf, value, le);
    write_uint16(buf+2, value2, le);
    dobj->buffer = buf;
    *dobjd = dobj;
    return 1;
}

int make_3_short_obj(uint16_t value, uint16_t value2, uint16_t value3, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le) {
    data_object_t *dobj = (data_object_t *) malloc(sizeof(data_object_t));
    dobj->full_name = strdup(full_name);
    dobj->written = 0;
    dobj->pointer = pointer;
    dobj->nr_my_pointers = 0;
    dobj->nr_my_offsets = 0;
    dobj->size = 6;
    unsigned char *buf = (unsigned char *) malloc(dobj->size);
    write_uint16(buf, value, le);
    write_uint16(buf+2, value2, le);
    write_uint16(buf+4, value3, le);
    dobj->buffer = buf;
    *dobjd = dobj;
    return 1;
}

int make_4_short_obj(uint16_t value, uint16_t value2, uint16_t value3, uint16_t value4, pointer_t *pointer, const char *full_name, data_object_t **dobjd, int le) {
    data_object_t *dobj = (data_object_t *) malloc(sizeof(data_object_t));
    dobj->full_name = strdup(full_name);
    dobj->written = 0;
    dobj->pointer = pointer;
    dobj->nr_my_pointers = 0;
    dobj->nr_my_offsets = 0;
    dobj->size = 8;
    unsigned char *buf = (unsigned char *) malloc(dobj->size);
    write_uint16(buf, value, le);
    write_uint16(buf+2, value2, le);
    write_uint16(buf+4, value3, le);
    write_uint16(buf+6, value4, le);
    dobj->buffer = buf;
    *dobjd = dobj;
    return 1;
}


void interpret_single_tag_value(writer_t *wr, const char *val_attr, int tagtype, uint32_t *count, int le) {
    uint32_t utemp, utemp2;
    int32_t stemp, stemp2;
    (*count) ++;
    switch (tagtype) {
        case TAG_TYPE_ASCII:
            wr->writebytes(wr, val_attr, strlen(val_attr) + 1);
            *count += strlen(val_attr);
            break;
        case TAG_TYPE_SHORT:
            if (1!=sscanf(val_attr, "%u", &utemp) || (utemp>>16)) {
                WARNING("Could not interpret value '%s' as a SHORT, skipping.\n", val_attr); return;
            }
            write_uint16_writer(wr, utemp, le);
            break;
        case TAG_TYPE_LONG:
            if (1!=sscanf(val_attr, "%u", &utemp)) {
                WARNING("Could not interpret value '%s' as a LONG, skipping.\n", val_attr); return;
            }
            write_uint32_writer(wr, utemp, le);
            break;
        case TAG_TYPE_SLONG:
            if (1!=sscanf(val_attr, "%d", &stemp)) {
                WARNING("Could not interpret value '%s' as a SLONG, skipping.\n", val_attr); return;
            }
            write_uint32_writer(wr, (uint32_t) stemp, le);
            break;
        case TAG_TYPE_RATIONAL:
            if (2!=sscanf(val_attr, "%u/%u", &utemp, &utemp2)) utemp2 = 1;
            if (1!=sscanf(val_attr, "%u", &utemp)) {
                WARNING("Could not interpret value '%s' as a RATIONAL, skipping.\n", val_attr); return;
            }
            write_uint32_writer(wr, utemp, le); write_uint32_writer(wr, utemp2, le);
            break;
        case TAG_TYPE_SRATIONAL:
            if (2!=sscanf(val_attr, "%d/%d", &stemp, &stemp2)) stemp2 = 1;
            if (1!=sscanf(val_attr, "%d", &stemp)) {
                WARNING("Could not interpret value '%s' as a SRATIONAL, skipping.\n", val_attr); return;
            }
            write_uint32_writer(wr, (uint32_t) stemp, le); write_uint32_writer(wr, (uint32_t) stemp2, le);
            break;
        case TAG_TYPE_BYTE:
        case TAG_TYPE_UNDEFINED:
        default:
            if (1!=sscanf(val_attr, "%u", &utemp) || (utemp>>8)) {
                WARNING("Could not interpret value '%s' as a BYTE/UNDEFINED/UNKNOWN, skipping.\n", val_attr); return;
            }
            wr->writebyte(wr, utemp);
            break;
    }
}

static void interpret_noncustom_tag(mxml_node_t *tag, int tagtype, void *count_pointer, const char *full_name, pointer_t *pointer, data_object_t **dobj, int le) {
    const char *val_attr = mxmlElementGetAttr(tag, "value");
    if (val_attr!=NULL && tag->child!=NULL) {
        ERROR("something's wrong. this tag with id '%s' should not have both a value attribute, and children!\n", tag->value.element.name);
    }
    write_uint32(count_pointer, 0, le);
    uint32_t cnt = 0;
    writer_t wr;
    init_memory_writer(&wr, 100, 100);
    if (val_attr != NULL) {
        interpret_single_tag_value(&wr, val_attr, tagtype, &cnt, le);
    } else {
        FOR_CHILDREN(node, tag) {
            if (IS_ELEMENT(node, TAG_TYPE_STR(tagtype))) {
                const char *sub_val_attr = mxmlElementGetAttr(node, "value");
                if (sub_val_attr!=NULL) {
                    interpret_single_tag_value(&wr, sub_val_attr, tagtype, &cnt, le);
                } else {
                    printf("WARNING, element '%s' of parent id '%s' should have a value attribute! (skipping)\n",
                            node->value.element.name, mxmlElementGetAttr(tag, "id"));
                }
            } else if (IS_ELEMENT(node, "binary")) {
                xm2i_write_binary(&wr, node);
            } else {
                printf("WARNING. tag with id '%s' should only have children of type '%s', not '%s' (skipping)\n",
                        mxmlElementGetAttr(tag, "id"), TAG_TYPE_STR(tagtype), node->value.element.name);
            }
        }
    }
    if (tagtype==TAG_TYPE_UNDEFINED) cnt = wr.size;
    write_uint32(count_pointer, cnt, le);
    make_dobj_from_mem_writer(&wr, pointer, full_name, dobj, le);
}

static void interpret_tag(mxml_node_t *tag, data_object_t *parent, const char *full_name, writer_t *objdb, int tag_index, custom_tag_t *tags, size_t endian_pos, int le) {
    unsigned char *tag_addr = ((unsigned char *)parent->buffer) + 2 + 12 * tag_index;
    const char *tagidattr = mxmlElementGetAttr(tag, "id");
    if (tagidattr==NULL) ERROR("all tag elements must have an id attribute\n");
    char *my_full_name;
    CONCS(my_full_name, full_name, tagidattr);
    custom_tag_t *ctag = getCustomTagFromName(tags, tagidattr);
    uint16_t tagid;
    if (ctag!=NULL) tagid = ctag->id;
    else {
        int val;
        if (1==sscanf(tagidattr, "0x%x", &val)) {
            tagid = val;
        } else ERROR("unknown tag: %s\n", tagidattr);
    }
    const char *tagtype_s = mxmlElementGetAttr(tag, "type");
    int tagtype = TAG_TYPE_FROM_STR(tagtype_s);
    if (tagtype == -1 && (ctag == NULL || ctag->def_type==0)) ERROR("no type was specified, and this tag has no default type (id: %s)\n", tagidattr);
    if (tagtype == -1) tagtype = ctag->def_type;
    write_uint16(tag_addr, tagid, le);
    write_uint16(tag_addr + 2, tagtype, le);

    data_object_t *dobj = NULL;
    pointer_t *this_pointer = mem_pointer(tag_addr+8);
    size_t oldobjdbsize = objdb->size;
    if (ctag != NULL && ctag->def_type != 0 && tagtype != -1 && tagtype != ctag->def_type) {
        interpret_noncustom_tag(tag, tagtype, tag_addr + 4, my_full_name, this_pointer, &dobj, le);
        WARNING("xml is using a nonstandard tag type for tag: %s\n", tagidattr);
    } else if (ctag==NULL) {
        interpret_noncustom_tag(tag, tagtype, tag_addr + 4, my_full_name, this_pointer, &dobj, le);
    } else {
        if (ctag->custominterpret!=NULL && ctag->custominterpret(tag, this_pointer, my_full_name, tag_addr + 4, &dobj, parent, objdb, endian_pos, le)) {
            // nothing to do , everything's in the if condition
        } else {
            interpret_noncustom_tag(tag, tagtype, tag_addr + 4, my_full_name, this_pointer, &dobj, le);
        }
    }
    free(my_full_name); // it should have already been dup-ed
    if (dobj==NULL && (objdb->size > oldobjdbsize)) {
        parent->my_pointers[parent->nr_my_pointers++] = this_pointer;
        return;
    }
    if (dobj==NULL) ERROR("these shouldn't be true at the same time: dobj is null; objdb didn't grow\n");
    size_t valbytes = dobj->size;
    unsigned char *valbuffer = (unsigned char *) dobj->buffer;
    if (valbytes<=4) {
        int i;
        for (i=0;i<valbytes;i++) tag_addr[8 + i] = valbuffer[i];
        for (;i<4;i++) tag_addr[8+i] = 0;
        if (dobj!=NULL) {
            if (dobj->nr_my_pointers>1) ERROR("weird, <= 4 bytes of tagvalue shouldn't contain more then one pointer\n");
            if (dobj->nr_my_pointers==1) {
                pointer_t *p = dobj->my_pointers[0];
                if (p->type!=p_memory) ERROR("weird, <= 4 bytes of tagvalue should have only a memory type pointer\n");
                if (p->value.memory!=dobj->buffer) ERROR("weird, <= 4 bytes tagvalue, and contains a pointer != data buffer\n");
                p->value.memory = tag_addr + 8; // TODO , fix memory leak?
            }
            add_offset(parent, strdup(dobj->full_name), 2 + 12 * tag_index + 8);
            FREE_OBJECT(dobj);
        }
    } else {
        parent->my_pointers[parent->nr_my_pointers++] = this_pointer;
        STORE_OBJECT(objdb, dobj);
        //printf("adding tag object with name: %s\n", dobj->full_name); // TODO remove
    }
}

void interpret_ifd_chain(mxml_node_t *marker_node, pointer_t *prev_pointer, char *full_name, writer_t *objdb, custom_tag_t *tags, size_t endian_pos, int le) {
    int ifd_index = 0;
    data_object_t *dobj = NULL;
    FOR_CHILDREN(node, marker_node) {
        if (IS_ELEMENT(node, "ifd")) {
            resetTags();
            char *ifd_full_name;
            
            //(char *)malloc(strlen(full_name) + 7 + 1); // 7 = "/ifd[0]" 1 = terminating NULL // TODO remove
            //snprintf(ifd_full_name, strlen(full_name) + 7 + 1, "%s/ifd[%d]", full_name, ifd_index);
            CONCP(ifd_full_name, "%s/ifd[%d]", full_name, ifd_index);
            
            // count tags
            int nr_tags = 0;
            FOR_CHILDREN(tag1, node) {
                if (IS_ELEMENT(tag1, "tag")) nr_tags++;
                else WARNING("unknown node in ifd '%s'. There are supposed to be only tag elements here!\n", ifd_full_name);
            }
            if (nr_tags>>16) ERROR("too many tags in ifd '%s'\n", ifd_full_name);
            //printf("found %d tags\n", nr_tags); // TODO remove...
            dobj = malloc(sizeof(data_object_t) + sizeof(pointer_t) * (nr_tags + 1));
                 // each tag _could_ in principle point somewhere else + there is the next ifd pointer
            dobj->full_name = ifd_full_name;
            dobj->written = 0;
            dobj->pointer = prev_pointer;
            dobj->nr_my_pointers = 0;
            dobj->nr_my_offsets = 0;
            dobj->size = 12 * nr_tags + 2 + 4; // 12 bytes/tag + 2 bytes=nr. of tags + 4 bytes=next ifd pointer.
            void *ifd_buffer = malloc(dobj->size);
            dobj->buffer = ifd_buffer;
            write_uint16(ifd_buffer, nr_tags, le);
            int tag_index = 0;
            FOR_CHILDREN(tag2, node) {
                if (IS_ELEMENT(tag2, "tag")) {
                    interpret_tag(tag2, dobj, ifd_full_name, objdb, tag_index++, tags, endian_pos, le); // TODO
                }
            }
            void *next_t = ((char *)ifd_buffer) + 12 * nr_tags + 2;
            write_uint16(next_t, 0, le); // assume it's the last one
            prev_pointer = mem_pointer(next_t);
            dobj->my_pointers[dobj->nr_my_pointers++] = prev_pointer;
            STORE_OBJECT(objdb, dobj);
            ifd_index++;
        }
    }
    // now we kill the last pointer of the last ifd, because it's not really pointing to anything.
    if (dobj!=NULL) {
        dobj->nr_my_pointers--;
        set_pointer(dobj->my_pointers[dobj->nr_my_pointers], 0, le);
        free(dobj->my_pointers[dobj->nr_my_pointers]);
    }
}

// UGLY, todo, clean up:
writer_t *write_offsets = NULL;
char *write_offsets_prefix = "";
static const char *write_offsets_second_prefix = "";

static void write_offset(char *full_name, int offset) {
   if (write_offsets != NULL) {
        write_string_writer(write_offsets, "#define ");
        write_string_writer(write_offsets, write_offsets_prefix);
        write_string_writer(write_offsets, write_offsets_second_prefix);
        char *t = full_name;
        char s[50];
        for (;t[0]!=0;t++) {
            if ((t[0]>='a' && t[0]<='z') ||
                (t[0]>='A' && t[0]<='Z') ||
                (t[0]>='0' && t[0]<='9'))
                {
                    write_offsets->writebyte(write_offsets, t[0]);
                } else {
                    snprintf(s, 50, "_"); // "_%d_", t[0]); // let's favor readability
                    write_string_writer(write_offsets, s);
                }
        }
        snprintf(s, 50, " %d\n", offset);
        write_string_writer(write_offsets, s);
    }
}


/* this writes the object out. It then updates the pointer that points to it. It then turns all the child
 * pointers from memory pointers to writer pointers.
 */
static void place_object(writer_t *wr, data_object_t *o, size_t endian_pos, int le) {
    o->written = 1;
    size_t object_pos = wr->pos;
    size_t object_pos_relative_to_endian = object_pos - endian_pos;
    wr->writebytes(wr, o->buffer, o->size);
    set_pointer(o->pointer, object_pos_relative_to_endian, le);
    int j;
    for (j=0;j<o->nr_my_pointers;j++) { // turn all the child pointers into writer-pointers.
        pointer_t *p = o->my_pointers[j];
        if (p->type==p_writer) ERROR("hmm, how did this turn into a writer?\n");
        size_t offset = ((char *) p->value.memory) - ((char *)o->buffer);
        p->type = p_writer;
        p->value.writer.writer = wr;
        p->value.writer.pos = object_pos + offset;
    }
    for (j=0;j<o->nr_my_offsets;j++) {
        write_offset(o->my_offsets[j].full_name, object_pos + o->my_offsets[j].offset);
    }
    write_offset(o->full_name, object_pos);
}

static void xm2i_write_mpf_exif(writer_t *wr, mxml_node_t *marker_node) {
    int le = 1; // default little-endian.
    size_t endian_pos = wr->pos;
    write_offsets_second_prefix = mxmlElementGetAttr(marker_node, "type");
    if (IS_ELEMENT_ATTR(marker_node, "endian", "big")) {
        write_offset("_endian", wr->pos);
        write_offset("_endian_little", 0);
        wr->writebyte(wr, 0x4D); wr->writebyte(wr, 0x4D); wr->writebyte(wr, 0x00); wr->writebyte(wr, 0x2A); 
        le = 0;
    } else if (IS_ELEMENT_ATTR(marker_node, "endian", "little")) {
        write_offset("_endian", wr->pos);
        write_offset("_endian_little", 1);
        wr->writebyte(wr, 0x49); wr->writebyte(wr, 0x49); wr->writebyte(wr, 0x2A); wr->writebyte(wr, 0x00); 
    } else if (mxmlElementGetAttr(marker_node, "endian")!=NULL) {
        ERROR_DUMPNODE(marker_node, "unknown endian: %s", mxmlElementGetAttr(marker_node, "endian"));
    } else {
        WARNING("Assuming little-endian.\n");
        write_offset("_endian", wr->pos);
        write_offset("_endian_little", 1);
        wr->writebyte(wr, 0x49); wr->writebyte(wr, 0x49); wr->writebyte(wr, 0x2A); wr->writebyte(wr, 0x00); 
    }
    pointer_t *first_ifd_pointer = writer_pointer(wr, wr->pos);
    wr->writebyte(wr, 0); wr->writebyte(wr, 0); wr->writebyte(wr, 0); wr->writebyte(wr, 0); // dummy first ifd pointer
    writer_t objdb; // TODO change into a C++ data structure
    init_memory_writer(&objdb, sizeof(data_object_t *) * 30, sizeof(data_object_t *) * 10);

    interpret_ifd_chain(marker_node, first_ifd_pointer, "", &objdb, global_tags, endian_pos, le);
    
    int nr_objects = objdb.size / (sizeof(data_object_t *));
    data_object_t **objects = (data_object_t **) free_memory_writer(&objdb);
    
    // and now let's handle the <data-placement> tag!
    FOR_CHILDREN(node, marker_node) {
        if (IS_ELEMENT(node, "data-placement")) {
            FOR_CHILDREN(data, node) {
                if (IS_ELEMENT(data, "binary")) {
                    xm2i_write_binary(wr, data);
                } else if (IS_ELEMENT(data, "reference")){
                    const char *from = mxmlElementGetAttr(data, "from");
                    if (from==NULL) ERROR_DUMPNODE(data, "reference element should have a 'from' attribute!");
                    int i;
                    for (i=0;i<nr_objects;i++) // TODO change to faster (logarithmic) object database lookup, with c++ data structure (make sure add-order is preserved too)
                        if (0==strcmp(objects[i]->full_name, from) && !objects[i]->written) {
                            place_object(wr, objects[i], endian_pos, le);
                            break;
                        }
                    if (i>=nr_objects) WARNING("reference from %s not found!\n", from);
                } else {
                    WARNING("data-placement should contain reference, or binary(include/hexdump) nodes only!\n");
                }
            }
        }
    }
    int i;
    // and now we are going to write all the objects that have not been written so far.
    for (i=0;i<nr_objects;i++)
        if (!objects[i]->written) {
            place_object(wr, objects[i], endian_pos, le);
        }
    for (i=0;i<nr_objects;i++) {
        // TODO maybe check if objects[i]->pointer has been set to a non-zero value or not
        FREE_OBJECT(objects[i]);
    }
    free(objects);
    write_offsets_second_prefix = "";
}

static void xm2i_write_jps(writer_t *wr, mxml_node_t *marker_node) {
    FOR_CHILDREN(node, marker_node) {
        if (IS_ELEMENT(node, "StereoscopicDescriptor")) {
            int media = 0, byte2 = 0, byte3 = 0, byte4 = 0;
            FOR_CHILDREN(ch, node) {
                if (ch->type!=MXML_ELEMENT) continue;
                const char *val_attr = mxmlElementGetAttr(ch, "value");
                if (val_attr==NULL) {
                    WARNING("element %s must have a value attribute!\n", ch->value.element.name);
                    continue;
                }
                if (IS_ELEMENT(ch, "media_type")) {
                    if (0==strcmp("monoscopic", val_attr)) {
                        media = 0;
                    } else if (0==strcmp("stereoscopic", val_attr)) {
                        media = 1;
                    } else {
                        WARNING("unknown JPS media type!\n");
                    }
                } else if (IS_ELEMENT(ch, "display")) {
                    if (0==strcmp("both eyes", val_attr)) {
                        byte2 = 0;
                    } else if (0==strcmp("left eye", val_attr)) {
                        byte2 = 1;
                    } else if (0==strcmp("right eye", val_attr)) {
                        byte2 = 2;
                    } else if (1!=sscanf(val_attr, "0x%x", &byte2)) {
                        WARNING("display node has a wrong value: %s\n", val_attr);
                        continue;
                    }
                } else if (IS_ELEMENT(ch, "layout")) {
                    if (0==strcmp("interleaved", val_attr)) {
                        byte2 = 1;
                    } else if (0==strcmp("sidebyside", val_attr)) {
                        byte2 = 2;
                    } else if (0==strcmp("overunder", val_attr)) {
                        byte2 = 3;
                    } else if (0==strcmp("anaglyph", val_attr)) {
                        byte2 = 4;
                    } else if (1!=sscanf(val_attr, "0x%x", &byte2)) {
                        WARNING("layout node has a wrong value: %s\n", val_attr);
                        continue;
                    }
                } else if (IS_ELEMENT(ch, "height")) {
                    if (0==strcmp("half", val_attr)) {
                        byte3 |= 1;
                    } else if (0==strcmp("full", val_attr)) {
                        byte3 &= ~1;
                    } else {
                        WARNING("height must be either half, or full, NOT: %s\n", val_attr);
                        continue;
                    }
                } else if (IS_ELEMENT(ch, "width")) {
                    if (0==strcmp("half", val_attr)) {
                        byte3 |= 2;
                    } else if (0==strcmp("full", val_attr)) {
                        byte3 &= ~2;
                    } else {
                        WARNING("width must be either half, or full, NOT: %s\n", val_attr);
                        continue;
                    }
                } else if (IS_ELEMENT(ch, "field_order")) {
                    if (0==strcmp("left_first", val_attr) || 0==strcmp("left_red", val_attr)) {
                        byte3 |= 4;
                    } else if (0==strcmp("right_first", val_attr) || 0==strcmp("right_red", val_attr)) {
                        byte3 &= ~4;
                    } else {
                        WARNING("width must be either half, or full, NOT: %s\n", val_attr);
                        continue;
                    }
                } else if (IS_ELEMENT(ch, "misc_flags")) {
                    int t;
                    if (1!=sscanf(val_attr, "0x%x", &t)) {
                        byte3 |= t;
                    }
                } else if (IS_ELEMENT(ch, "reserved")) {
                    int t;
                    if (1!=sscanf(val_attr, "0x%x", &t)) {
                        byte4 = t;
                    }
                } else if (IS_ELEMENT(ch, "separation")) {
                    int t;
                    if (1!=sscanf(val_attr, "0x%x", &t)) {
                        byte4 = t;
                    }
                }
            }
            size_t sizepos = wr->pos;
            write_uint16_writer(wr, 0, 0); // dummy size
            size_t oldpos = wr->pos;
            wr->writebyte(wr, byte4);
            wr->writebyte(wr, byte3);
            wr->writebyte(wr, byte2);
            wr->writebyte(wr, media);
            FOR_CHILDREN(ch2, node) {
                if (IS_ELEMENT(ch2, "binary")) {
                    xm2i_write_binary(wr, ch2);
                }
            }
            size_t newpos = wr->pos;
            wr->seek(wr, sizepos);
            write_uint16_writer(wr, newpos-oldpos, 0);
            wr->seek(wr, newpos);
        } else if (IS_ELEMENT(node, "comment")) {
            if (node->type!=MXML_ELEMENT) continue;
            const char *val_attr = mxmlElementGetAttr(node, "value");
            if (val_attr==NULL) {
                WARNING("element %s must have a value attribute!\n", node->value.element.name);
                continue;
            }
            write_uint16_writer(wr, strlen(val_attr), 0);
            wr->writebytes(wr, val_attr, strlen(val_attr));
        } else if (IS_ELEMENT(node, "binary")) {
            xm2i_write_binary(wr, node);
        }
    }
}

static void xm2i_write_jfif(writer_t *wr, mxml_node_t *marker_node) {
    unsigned char data[8]; int t;
    FOR_CHILDREN(ch, marker_node) {
        if (ch->type != MXML_ELEMENT) continue;
        const char *val_attr = mxmlElementGetAttr(ch, "value");
        if (val_attr==NULL) {
            WARNING("xml node %s must have a value attribute\n", ch->value.element.name);
            continue;
        }
        if (IS_ELEMENT(ch, "density_units")) {
            if      (IS_ELEMENT_ATTR(ch, "value", "no_units_aspect_ratio_only")) data[2] = 0;
            else if (IS_ELEMENT_ATTR(ch, "value", "pixels_per_inch"))            data[2] = 1;
            else if (IS_ELEMENT_ATTR(ch, "value", "pixels_per_centimetre"))      data[2] = 2;
            else {
                if (1!=sscanf(val_attr, "0x%x", &t)) {
                    WARNING ("density_units contains something which doesn't belong here (%s)\n", val_attr);
                    continue;
                }
                if (t>=0 && t<=255) data[2] = t; else WARNING("density_units code must fit in a byte\n");
            }
        } else {
            if (1!=sscanf(val_attr, "%d", &t)) {
                WARNING("value must be an integer, in tag %s\n", ch->value.element.name);
                continue;
            }
            if        (IS_ELEMENT(ch, "majorversion")) {
                data[0] = t;
            } else if (IS_ELEMENT(ch, "minorversion")) {
                data[1] = t;
            } else if (IS_ELEMENT(ch, "x_density")) {
                write_uint16(&data[3], t, 0);
            } else if (IS_ELEMENT(ch, "y_density")) {
                write_uint16(&data[5], t, 0);
            } else if (IS_ELEMENT(ch, "thumbnail_width")) {
                data[7] = t;
            } else if (IS_ELEMENT(ch, "thumbnail_height")) {
                data[8] = t;
            }
        }
    }
    wr->writebytes(wr, data, 9);
    FOR_CHILDREN(node, marker_node) {
        if (IS_ELEMENT(node, "binary")) {
            xm2i_write_binary(wr, node);
        }
    }
}

/* This function will write the data contained in a marker.
 * This will NOT write the marker length, 0xff or the marker number itself
 * how much the position of the writer is advanced by this function can be used to
 * calculate the length of the marker, and write it out, by a different function:
 * xm2i_write_marker()
 */
static void xm2i_write_marker_inside(writer_t *wr, mxml_node_t *marker_node, int marker_number) {
    switch(marker_number) {
        case MARKER_APP0:
            if (IS_ELEMENT_ATTR(marker_node, "type", "JFIF")) {
                wr->writebytes(wr, "JFIF\0", 5);
                xm2i_write_jfif(wr, marker_node);
                return;
            }
        case MARKER_APP1:
            if (IS_ELEMENT_ATTR(marker_node, "type", "Exif")) {
                wr->writebytes(wr, "Exif\0\0", 6);
                xm2i_write_mpf_exif(wr, marker_node);
                return;
            }
            break;
        case MARKER_APP2:
            if (IS_ELEMENT_ATTR(marker_node, "type", "MPF")) {
                wr->writebytes(wr, "MPF\0", 4);
                xm2i_write_mpf_exif(wr, marker_node);
                return;
            }
            break;
        case MARKER_APP3:
            if (IS_ELEMENT_ATTR(marker_node, "type", "JPS")) {
                wr->writebytes(wr, "_JPSJPS_", 8);
                xm2i_write_jps(wr, marker_node);
                return;
            }
        default: 
            break;
    }
    // Now let's try just binary.
    FOR_CHILDREN(node, marker_node) {
        if (IS_ELEMENT(node, "binary")) {
            xm2i_write_binary(wr, node);
        } else {
            // i don't know what node this is.
            wr->writebyte(wr, 0xFF);// TODO just for testing, remove!
            ERROR_DUMPNODE(node, "WARNING: unknown node inside %s marker!\n", marker_node->value.element.name);
        }
    }
}

static void xm2i_write_marker(writer_t *wr, mxml_node_t *marker_node, int marker_number) {
    wr->writebyte(wr, 0xff);
    wr->writebyte(wr, marker_number);
    if (!IS_LONE_MARKER(marker_number)) {
        wr->writebyte(wr, 0); wr->writebyte(wr, 0); // these will contain the length of this marker
        size_t before_position = wr->pos;
        xm2i_write_marker_inside(wr, marker_node, marker_number);
        size_t after_position = wr->pos;
        size_t bytes = after_position - before_position;
        bytes += 2; // the size includes the size bytes.
        if (bytes >> 16) ERROR_DUMPNODE(marker_node, "this marker became larger then 2^16!");
        unsigned char big_end = (bytes >> 8) & 0xff;
        unsigned char little_end = bytes & 0xff;
        wr->seek(wr, before_position - 2);
        wr->writebyte(wr, big_end);
        wr->writebyte(wr, little_end);
        wr->seek(wr, after_position);
    }
}

static void xm2i_write_fill_bytes(writer_t *wr, mxml_node_t *fill) {
    const char *bytes_s = mxmlElementGetAttr(fill, "bytes");
    int bytes;
    if (bytes_s==NULL) ERROR_DUMPNODE(fill, "there must be a bytes attribute");
    if (1!=sscanf(bytes_s, "%d", &bytes)) ERROR_DUMPNODE(fill, "number of bytes can't be read: %s", bytes_s);
    int i;
    for (i=0;i<bytes;i++) wr->writebyte(wr, 0xff);
}

mp_entry_db_t mp_entry_db;

/* the second parameter is an <image> node */
static void xm2i_write_image_element(writer_t *wr, mxml_node_t *image) {
    if (IS_ELEMENT(image->child, "fill") && IS_ELEMENT_ATTR(image->child, "type", "beforeSOI")) {
        xm2i_write_fill_bytes(wr, image->child);
    }
    xm2i_write_marker(wr, NULL, MARKER_SOI);
    FOR_CHILDREN(node, image) {
        int marker;
        if (IS_ELEMENT(node, "fill") && IS_ELEMENT_ATTR(node, "type", "beforeSOI")) continue;
        if (IS_ELEMENT(node, "fill")) {
            xm2i_write_fill_bytes(wr, node);
        } else if (IS_ELEMENT(node, "binary")) {
            xm2i_write_binary(wr, node);
        } else if ((marker = get_marker_number(node->value.element.name))!=-1) {
            xm2i_write_marker(wr, node, marker);
        } else {
            ERROR_DUMPNODE(node, "this node is invalid in an image element. It should be either fill/binary(include/hexdump)/a_marker.");
        }
    }
    write_offset("size_without_eoi", wr->pos);
    xm2i_write_marker(wr, NULL, MARKER_EOI);
    write_offset("size_with_eoi", wr->pos);
}

/* This function handles a complete xml tree, with a <file> root. including (or not) the <?xml...> tag too */
static void xm2i_write_image(writer_t *wr, mxml_node_t *xml) {
    if (xml->value.element.name[0]=='?') xml = xml->child; // skip <?xml...> if there
    if (!IS_ELEMENT(xml, "file")) {
        ERROR("root node must be a <file> node, (instead of '%s')!\n", xml->value.element.name);
    }
    mp_entry_db.count = 0;
    int image_index = 0;
    size_t start = wr->pos;
    FOR_CHILDREN(node, xml) {
        if (IS_ELEMENT(node, "image")) {
            size_t stop = wr->pos;
            if (image_index!=0) {
                sort_mp_entry_db(&mp_entry_db);
                if (mp_entry_db.count!=0) {
                    if (image_index-1 >= mp_entry_db.count) {
                        printf("WARNING: this image (%d) has not been referenced!, not enough MP_Entry entry tags (count = %d)!\n", image_index-1, mp_entry_db.count);
                    } else {
                        set_pointer(mp_entry_db.arr[image_index-1].size_pointer, stop-start, mp_entry_db.arr[image_index-1].le);
                    }
                }
            }
            start = wr->pos;
            xm2i_write_image_element(wr, node);

            if (IS_ELEMENT_ATTR(node, "type", "unreferenced-concatenated")) {
                // do not add a reference
            } else if (!IS_ELEMENT_ATTR_NULL(node, "type")) {
                ERROR("unknown image type: %s\n", mxmlElementGetAttr(node, "type"));
            } else {
                if (mp_entry_db.count!=0) {
                    if (image_index >= mp_entry_db.count) {
                        printf("WARNING: this image (%d) has not been referenced!, not enough MP_Entry entry tags (count = %d)!\n", image_index, mp_entry_db.count);
                    } else {
                        if (start==0)
                            set_pointer(mp_entry_db.arr[image_index].offset_pointer, 0, mp_entry_db.arr[image_index].le);
                        else 
                            set_pointer(mp_entry_db.arr[image_index].offset_pointer, start - mp_entry_db.arr[image_index].end_pos, mp_entry_db.arr[image_index].le);
                    }
                }
            }
            image_index += 1;
        } else if (IS_ELEMENT(node, "binary")) {
             xm2i_write_binary(wr, node);
        } else {
            ERROR_DUMPNODE(node, "2nd level nodes must be either image/binary (include/hexdump).");
        }
    }
    size_t stop = wr->pos;
    sort_mp_entry_db(&mp_entry_db);
    if (mp_entry_db.count!=0) {
        if (image_index-1 >= mp_entry_db.count) {
            printf("WARNING: this image (%d) has not been referenced!, not enough MP_Entry entry tags (count = %d)!\n", image_index-1, mp_entry_db.count);
        } else {
            set_pointer(mp_entry_db.arr[image_index-1].size_pointer, stop-start, mp_entry_db.arr[image_index-1].le);
        }
    }
}


void xm2i_save_image_filename(mxml_node_t *xml, char * fn) {
    FILE *f = fopen(fn, "wb");
    if (f==NULL) {
        ERROR("Can't open file %s for writing!\n", fn);
    }
    xm2i_save_image_file(xml, f);
    fclose(f);
}
void xm2i_save_image_file(mxml_node_t *xml, FILE *f) {
    writer_t file_writer;
    init_file_writer(&file_writer, f);
    xm2i_write_image(&file_writer, xml);
    free_file_writer(&file_writer);
}
void *xm2i_save_image_mem(mxml_node_t *xml) { // TODO actually test this!
    writer_t memory_writer;
    init_memory_writer(&memory_writer, 300 * 1024, 50 * 1024);
    xm2i_write_image(&memory_writer, xml);
    return free_memory_writer(&memory_writer);
}
