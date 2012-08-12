/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

/* This file contains stuff for reading MPO/JPEG/JPS files and turning them
   into an mxml memory structure */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "image2xmlmem.h"
#include "markers.h"
#include "tags.h"
#include "data_placement.h"
#include "referenced_images.h"
#include "common.h"

#define NOTEOF(i) {if (*pos + (i) >= size) ERROR("Unexpected end of file\n") }
// skip over fill bytes before a marker: (fill bytes are continuous extra 0xff bytes before any marker
// the last 0xff byte is part of the marker of course)
#define SKIPFILL(type) { size_t t = *pos; NOTEOF(1) while (data[*pos] == 0xFF && data[*pos + 1] == 0xFF) { ++*pos; NOTEOF(1) }; if ((*pos)-t>0) STOREFILL((*pos)-t, (type)); }
#define STOREFILL(bytes, type) { mxml_node_t *e = mxmlNewElement(image, "fill"); \
                                 mxmlElementSetAttrf(e, "bytes", "%ld", (bytes)); \
                                 if ((type)!=NULL) mxmlElementSetAttr(e, "type", (char *)(type)); }
#define SOI_MARKER_AT(data, pos)  (((unsigned char *)(data))[pos]==0xFF && ((unsigned char *)(data))[pos+1]==MARKER_SOI)

uint32_t i2xm_read_uint32(unsigned char *data, size_t pos, int le) {
    if (le) {
        return data[pos] | (data[pos+1]<<8) | (data[pos+2]<<16) | (data[pos+3]<<24);
    } else {
        return (data[pos]<<24) | (data[pos+1]<<16) | (data[pos+2]<<8) | (data[pos+3]);
    }
}

uint16_t i2xm_read_uint16(unsigned char *data, size_t pos, int le) {
    if (le) {
        return data[pos] | (data[pos+1]<<8);
    } else {
        return (data[pos]<<8) | (data[pos+1]);
    }
}


void i2xm_read_single_value(unsigned char *data,        // pointer to the mpo/jpg file in memory
                            size_t        *pos,         // the absolute position of the data to be read
                            mxml_node_t   *destination, // the parent node in which to place a "value" attribute
                            uint16_t      type,         // TAG_TYPE_*
                            int           le) {         // Endian (1 = little endian, 0 = big endian)
    switch (type) {
        case TAG_TYPE_ASCII:
            mxmlElementSetAttrf(destination, "value", "%c", data[*pos]);
            (*pos) += 1;
            break;
        case TAG_TYPE_SHORT:
            mxmlElementSetAttrf(destination, "value", "%u", i2xm_read_uint16(data, *pos, le));
            (*pos) += 2;
            break;
        case TAG_TYPE_LONG:
            mxmlElementSetAttrf(destination, "value", "%u", i2xm_read_uint32(data, *pos, le));
            (*pos) += 4;
            break;
        case TAG_TYPE_SLONG:
            mxmlElementSetAttrf(destination, "value", "%d", i2xm_read_uint32(data, *pos, le));
            (*pos) += 4;
            break;
        case TAG_TYPE_RATIONAL: {
            uint32_t num, den;
            num = i2xm_read_uint32(data, *pos, le);
            (*pos) += 4;
            den = i2xm_read_uint32(data, *pos, le);
            (*pos) += 4;
            mxmlElementSetAttrf(destination, "value", "%u/%u", num, den);
            break;
        }
        case TAG_TYPE_SRATIONAL: {
            int32_t num, den;
            num = i2xm_read_int32(data, *pos, le);
            (*pos) += 4;
            den = i2xm_read_int32(data, *pos, le);
            (*pos) += 4;
            mxmlElementSetAttrf(destination, "value", "%d/%d", num, den);
            break;
        }
        case TAG_TYPE_BYTE:
        case TAG_TYPE_UNDEFINED:
        default: // unknown tag type. treat as single byte
            mxmlElementSetAttrf(destination, "value", "%u", data[*pos]);
            (*pos) += 1;
            break;
    }
}

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
                           int           le) {         // Endian (1 = little endian, 0 = big endian)
    if (count==1) i2xm_read_single_value(data, pos, destination, type, le); // modifies pos!
    else {
        if (type == TAG_TYPE_ASCII) {
            // strings get a single value xml attribute.
            if (data[(*pos) + count - 1] != 0) {
                printf("WARNING: ASCII type tags should have their last byte equal \\0. Overwriting...\n");
                data[(*pos) + count - 1] = 0;
            }
            mxmlElementSetAttrf(destination, "value", "%s", data + *pos);
            (*pos) = (*pos) + count;
        } else if (type == TAG_TYPE_UNDEFINED && count > 5) {
            i2xm_read_binary(data, *pos, get_bytes_per_tag_value(type, count), destination);
        } else {
            // now we are going to have each value get its own element.
            uint16_t i;
            for (i=0;i<count;i++) {
                mxml_node_t *v = mxmlNewElement(destination, TAG_TYPE_STR(type));
                i2xm_read_single_value(data, pos, v, type, le); // modifies pos!
            }
        }
    }
}


static void i2xm_parse_tag(unsigned char       *data, // pointer the the mpo/jpg file in memory
                           size_t              epos,  // the absolute position of the 4-byte sequence that represents the current endian
                           size_t              size,  // size of the marker starting from epos
                           size_t              tpos,  // the absolute position of the 12-byte tag
                           mxml_node_t         *ifd,  // output <tag> xml nodes are placed in this parent node
                           char                *full_name, //used to custruct the full tag name, including parent ifds, and ifd pointers
                           data_placement_db_t *dp,
                           custom_tag_t        *tags, // try to interpret the tag as one of this list
                           referenced_images_t *images, // this is where the MP_Entry tags will place referenced images so we can later retrieve them
                           int                 le) { // Endian (1 if little endian, 0 if big-endian)
// tag_i is the 1-based index of the tag that is being parsed.
    uint16_t tag_id = i2xm_read_uint16(data, tpos, le);
    uint16_t tag_type = i2xm_read_uint16(data, tpos+2, le);
    uint32_t tag_count = i2xm_read_uint32(data, tpos+4, le);
    custom_tag_t *ctag = getCustomTag(tags, tag_id);
    int bytes_per_tag_value = get_bytes_per_tag_value(tag_type, tag_count);
    size_t vpos = 0; // contains the position of the value
    if (bytes_per_tag_value <=4 && (ctag==NULL || !ctag->pointer)) {
        // the value itself is at tpos+8
        vpos = tpos + 8;
    } else {
        // the address at tpos+8 points to the data.
        vpos = epos + i2xm_read_uint32(data, tpos+8, le);
    }
    // now parse the data
    mxml_node_t *tag = mxmlNewElement(ifd, "tag");
    if (ctag != NULL)
         mxmlElementSetAttrf(tag, "id", "%s", ctag->name);
    else mxmlElementSetAttrf(tag, "id", "0x%x", tag_id);
    const char *id = mxmlElementGetAttr(tag, "id");
    char *tag_full_name = (char *) malloc(strlen(id) + strlen(full_name) + strlen("/") + 1);
    snprintf(tag_full_name, strlen(id) + strlen(full_name) + strlen("/") + 1, "%s/%s", full_name, id);
    if ((ctag==NULL || !ctag->pointer) && (bytes_per_tag_value>4))
            add_referenced_data(dp, vpos, bytes_per_tag_value, tag_full_name);
    if (ctag != NULL) {
        if (ctag->def_type != 0 && tag_type != ctag->def_type) {
            mxmlElementSetAttrf(tag, "type", "%s", TAG_TYPE_STR(tag_type));
            printf("WARNING: tag %s is using a nonstandard type! (%s instead of %s)\n", ctag->name, TAG_TYPE_STR(tag_type), TAG_TYPE_STR(ctag->def_type));
            i2xm_read_array_value(data, &vpos, tag_count, tag, tag_type, le);
        } else {
            if (ctag->def_count!=0 && tag_count != ctag->def_count) {
                printf("WARNING: tag %s is using a nonstandard count! (%d instead of %d)\n", ctag->name, tag_count, ctag->def_count);
                i2xm_read_array_value(data, &vpos, tag_count, tag, tag_type, le);
            } else {
                if (ctag->customfunction != NULL) {
                    ctag->customfunction(data, vpos, epos, size, tag_type, tag_count, bytes_per_tag_value, tag, dp, full_name, tag_full_name, images, le);
                } else {
                    if (ctag->def_type==0) mxmlElementSetAttrf(tag, "type", "%s", TAG_TYPE_STR(tag_type));
                    i2xm_read_array_value(data, &vpos, tag_count, tag, tag_type, le);
                }
            }
        }
    } else {
        mxmlElementSetAttrf(tag, "type", "%s", TAG_TYPE_STR(tag_type));
        i2xm_read_array_value(data, &vpos, tag_count, tag, tag_type, le);
    }
    free(tag_full_name);
}

void i2xm_read_ifds(unsigned char       *data,        // pointer the the mpo/jpg file in memory
                    size_t              endian_pos,   // the absolute position of the 4-byte sequence that represents the current endian
                    size_t              size,         // size of the marker starting from endian_pos (inclusive)
                    uint32_t            first_ifd_offset, // the relative position of the first ifd (relative to the endian bytes)
                    mxml_node_t         *parent,      // the parent node in the output xml tree
                    custom_tag_t        *tags,        // the possible tags that can appear in these ifds
                    data_placement_db_t *dp,
                    char                *full_name,   // used to construct a full name for each tag, including parent ifds.
                    referenced_images_t *images,      // this is where the MP_Entry tags will place referenced images so we can later retrieve them
                    int                 read_single_ifd, // read only a single ifd, don't follow pointer to next ifd
                    int                 le) { // Endian (1 if little endian, 0 if big-endian)
                    // TODO add out-of-bounds checking?
    uint32_t ifd_offset = first_ifd_offset;
    int ifd_index = 0;
    do {
        char *ifd_full_name = (char *)malloc(strlen(full_name) + 7 + 1); // 7 = "/ifd[0]" 1 = terminating NULL
        snprintf(ifd_full_name, strlen(full_name) + 7 + 1, "%s/ifd[%d]", full_name, ifd_index);
        
        if (ifd_offset+1 >= size) ERROR("pointer for ifd %s points outside of marker\n", ifd_full_name);
        
        uint16_t tag_count = i2xm_read_uint16(data, endian_pos + ifd_offset, le);
        
        if ((ifd_offset + 2 + 12 * tag_count - 1) >=size) ERROR("ifd larger then marker size? (ifd: %s)\n", ifd_full_name);
        if ((ifd_offset + 2 + 12 * tag_count + 4 - 1) >=size) ERROR("next ifd pointer is not in marker? (ifd: %s)\n", ifd_full_name);
        
        add_referenced_data(dp, endian_pos + ifd_offset, 12 * tag_count + 2 + 4, ifd_full_name);
        
        mxml_node_t *ifd = mxmlNewElement(parent, "ifd");
    
        uint32_t start_of_ifd_values = endian_pos + (ifd_offset + 2 + 4) + 12 * tag_count;
        uint32_t next_ifd_offset = i2xm_read_uint32(data, start_of_ifd_values - 4, le);
    
        // allocate array to track the unreferenced data in the value part of the IFD.
        uint16_t tag_i;
        for (tag_i=0;tag_i<tag_count;tag_i++) {
            i2xm_parse_tag(data, endian_pos, size, endian_pos + (ifd_offset + 2) + 12 * tag_i, ifd, ifd_full_name, dp, tags, images, le);
        }
        ifd_offset = next_ifd_offset;
        ifd_index++;
        free(ifd_full_name);
    } while (ifd_offset != 0 && !read_single_ifd);
}

static void i2xm_read_mpf_exif(unsigned char       *data, // pointer to the image file in memory
                               size_t              pos,   // absolute position of the Endian field in the MPF/Exif header.
                               size_t              size,  // remaining size of the APP2 marker from pos to the end of the APP2 marker
                               mxml_node_t         *parent, // parent APP2 xml tag
                               referenced_images_t *images) { // this is where the MP_Entry tags will place referenced images so we can later retrieve them
    int le = 1;
    if (size<8) ERROR("this mpf/exif marker segment seems to be malformed! (it's too small)\n");
    if (data[pos] == 0x49 && data[pos + 1] == 0x49 && data[pos + 2] == 0x2A && data[pos + 3] == 0x00) {
        mxmlElementSetAttr(parent, "endian", "little");
        le = 1;
    } else if (data[pos] == 0x4D && data[pos + 1] == 0x4D && data[pos + 2] == 0x00 && data[pos + 3] == 0x2A) {
        mxmlElementSetAttr(parent, "endian", "big");
        le = 0;
        printf("Warning: Big endian detected. I'm not sure what values are affected and what values aren't so this might be buggy!\n");
    } else {
        printf("Warning: I have no idea what kind of endian this is @%ld!\n", pos);
    }
    uint32_t ifd_offset = i2xm_read_uint32(data, pos + 4, le);
    if (ifd_offset >= size) ERROR("malformed marker segment: first ifd offset points outside of marker!\n");
    if (ifd_offset < 8) ERROR("malformed marker segment: impossible first ifd pointer!\n");
    data_placement_db_t *dp = init_data_placement_db(pos + 8, size - 8);
    i2xm_read_ifds(data, pos /* endian position */, size, ifd_offset, parent, global_tags, dp, "", images, 0, le);
    sort_data_placement_db(dp);
    mxml_node_t *placement = mxmlNewElement(parent, "data-placement");
    int i;
    for (i=0;i<dp->count;i++) {
        if (dp->arr[i].full_name!=NULL) {
            mxml_node_t *ref = mxmlNewElement(placement, "reference");
            mxmlElementSetAttrf(ref, "from", "%s", dp->arr[i].full_name);
            mxmlElementSetAttrf(ref, "start_pos", "%ld", dp->arr[i].start);
            mxmlElementSetAttrf(ref, "length" , "%ld", dp->arr[i].len);
        } else if (dp->arr[i].len>0) {
            // unreferenced data
            i2xm_read_binary(data, dp->arr[i].start, dp->arr[i].len, placement);
        }
    }
    free_data_placement_db(dp);
}

/*
 * 'i2xm_read_sterdesc()' - Read the Stereoscopic Data Descriptor of a JPS file
 */

static void i2xm_read_sterdesc(unsigned char *data, // pointer to the image file in memory
                               size_t        pos,   // absolute position of the byte following the _JPSJPS_ identifier
                               size_t        size,  // size of the descriptor, including lengths, optional additional data, and comments
                               mxml_node_t   *parent) { // the parent node in the output xml tree
    uint16_t desc_length = i2xm_read_uint16(data, pos, 0);
    pos += 2;
    uint32_t descriptor = i2xm_read_uint32(data, pos, 0);
    mxml_node_t *desctag = mxmlNewElement(parent, "StereoscopicDescriptor");
    if ((descriptor & 0xFF) == 0) { // this is a monoscopic image
        mxmlElementSetAttrf(mxmlNewElement(desctag, "media_type"), "value", "monoscopic");
        uint8_t display = (descriptor >> 8) & 0xFF;
        switch (display) {
            case 0x00: mxmlElementSetAttrf(mxmlNewElement(desctag, "display"), "value", "both eyes"); break;
            case 0x01: mxmlElementSetAttrf(mxmlNewElement(desctag, "display"), "value", "left eye"); break;
            case 0x02: mxmlElementSetAttrf(mxmlNewElement(desctag, "display"), "value", "right eye"); break;
            default: mxmlElementSetAttrf(mxmlNewElement(desctag, "display"), "value", "0x%x", display);
        }
        uint8_t flags = (descriptor >> 16) & 0xFF;
        if (flags & 1) mxmlElementSetAttrf(mxmlNewElement(desctag, "height"), "value", "half");
        else mxmlElementSetAttrf(mxmlNewElement(desctag, "height"), "value", "full");
        
        if (flags & 2) mxmlElementSetAttrf(mxmlNewElement(desctag, "width"), "value", "half");
        else mxmlElementSetAttrf(mxmlNewElement(desctag, "width"), "value", "full");
        
        if (flags >> 2) {
            mxmlElementSetAttrf(mxmlNewElement(desctag, "misc_flags"), "value", "0x%x", flags);
        }
        uint8_t reserved = descriptor >> 24;
        if (reserved) mxmlElementSetAttrf(mxmlNewElement(desctag, "reserved"), "value", "0x%x", reserved);
    } else if ((descriptor & 0xFF) == 1) { // this is a stereoscopic image
        mxmlElementSetAttrf(mxmlNewElement(desctag, "media_type"), "value", "stereoscopic");
        uint8_t layout = (descriptor >> 8) & 0xFF;
        switch (layout) {
            case 0x01: mxmlElementSetAttrf(mxmlNewElement(desctag, "layout"), "value", "interleaved"); break;
            case 0x02: mxmlElementSetAttrf(mxmlNewElement(desctag, "layout"), "value", "sidebyside"); break;
            case 0x03: mxmlElementSetAttrf(mxmlNewElement(desctag, "layout"), "value", "overunder"); break;
            case 0x04: mxmlElementSetAttrf(mxmlNewElement(desctag, "layout"), "value", "anaglyph"); break;
            default: mxmlElementSetAttrf(mxmlNewElement(desctag, "layout"), "value", "0x%x", layout); break;
        }
        uint8_t flags = (descriptor >> 16) & 0xFF;
        if (flags & 1) mxmlElementSetAttrf(mxmlNewElement(desctag, "height"), "value", "half");
        else mxmlElementSetAttrf(mxmlNewElement(desctag, "height"), "value", "full");
        
        if (flags & 2) mxmlElementSetAttrf(mxmlNewElement(desctag, "width"), "value", "half");
        else mxmlElementSetAttrf(mxmlNewElement(desctag, "width"), "value", "full");
        
        if (layout != 0x04) { // not anaglyph
            if (flags & 4) mxmlElementSetAttrf(mxmlNewElement(desctag, "field_order"), "value", "left_first");
            else mxmlElementSetAttrf(mxmlNewElement(desctag, "field_order"), "value", "right_first");
        } else {
            if (flags & 4) mxmlElementSetAttrf(mxmlNewElement(desctag, "field_order"), "value", "left_red");
            else mxmlElementSetAttrf(mxmlNewElement(desctag, "field_order"), "value", "right_red");
        }
        if (flags >> 3) {
            mxmlElementSetAttrf(mxmlNewElement(desctag, "misc_flags"), "value", "0x%x", flags);
        }
        uint8_t separation = (descriptor >> 24);
        if (separation || (layout == 2) || (layout == 3)) mxmlElementSetAttrf(mxmlNewElement(desctag, "separation"), "value", "0x%x", separation);
    } else {
        i2xm_read_binary(data, pos, desc_length, desctag);
    }
    desc_length -= 4; pos += 4;
    if (desc_length>0) // let's see if there is any more optional additional data
        i2xm_read_binary(data, pos, desc_length, desctag);
    pos += desc_length;
    uint16_t comment_length = i2xm_read_uint16(data, pos, 0);
    pos += 2;
    if (comment_length>0) {
        unsigned char t = data[pos + comment_length];
        data[pos + comment_length] = 0;
        mxmlElementSetAttrf(mxmlNewElement(parent, "comment"), "value", "%s", data + pos);
        data[pos + comment_length] = t;
    }
}

/*
 * 'i2xm_read_jfif()' - Read the JFIF header
 */

static void i2xm_read_jfif(unsigned char *data, // pointer to the image file in memory
                           size_t        pos,   // absolute position of the byte following the JFIF\0 identifier
                           size_t        size,  // size of the rest of the segment
                           mxml_node_t   *parent) { // the parent node in the output xml tree
    mxmlElementSetAttrf(mxmlNewElement(parent, "majorversion"), "value", "%d", data[pos]);
    mxmlElementSetAttrf(mxmlNewElement(parent, "minorversion"), "value", "%d", data[pos + 1]);
    switch (data[pos + 2]) {
        case 0: mxmlElementSetAttrf(mxmlNewElement(parent, "density_units"), "value", "no_units_aspect_ratio_only"); break;
        case 1: mxmlElementSetAttrf(mxmlNewElement(parent, "density_units"), "value", "pixels_per_inch"); break;
        case 2: mxmlElementSetAttrf(mxmlNewElement(parent, "density_units"), "value", "pixels_per_centimetre"); break;
        default: mxmlElementSetAttrf(mxmlNewElement(parent, "density_units"), "value", "0x%x", data[pos + 2]);
    }
    mxmlElementSetAttrf(mxmlNewElement(parent, "x_density"), "value", "%d", i2xm_read_uint16(data, pos + 3, 0));
    mxmlElementSetAttrf(mxmlNewElement(parent, "y_density"), "value", "%d", i2xm_read_uint16(data, pos + 5, 0));
    unsigned char w, h;
    mxmlElementSetAttrf(mxmlNewElement(parent, "thumbnail_width"), "value", "%d", w = data[pos + 7]);
    mxmlElementSetAttrf(mxmlNewElement(parent, "thumbnail_height"), "value", "%d", h = data[pos + 8]);
    if (w*h>0) {
        mxml_node_t *thumbnail = mxmlNewElement(parent, "thumbnail");
        i2xm_read_binary(data, pos + 9, w*h, thumbnail);
    }
}

/*
 * 'i2xm_read_image()' - Read an image, and recursively read all other images that it refers to
 *
 * This function will also read all the non-parsed binary pieces, until the last referenced 
 * image in the file, but it will not read any non-parsed binary pieces after that.
 */

static void i2xm_read_image(unsigned char       *data,      // pointer to the image file in memory
                            size_t              *pos,       // absolute position of the image in the array
                            size_t              size,       // the size of the whole imagefile (not just this image)
                            mxml_node_t         *parent,    // parent xml file tag
                            int                 referenced, // if the image was not referenced, then a type="unreferenced-concatenated" attribute will be added to it's xml tag
                            referenced_images_t *images) {  // this is where the MP_Entry tags will place referenced images so we can later retrieve them
    mxml_node_t *image = mxmlNewElement(parent, "image");
    SKIPFILL("beforeSOI") // skip over possible fill bytes before the SOI marker
    if ((data[*pos] != 0xFF) || (data[*pos + 1] != MARKER_SOI))
        ERROR("A SOI marker was expected at position %ld\n", (*pos)++)
    if (!referenced) {
        mxmlElementSetAttrf(image, "type", "unreferenced-concatenated");
    }
    *pos += 2;
    while (*pos + 1 < size) { // this while runs once for each marker
        SKIPFILL(NULL)  // skip over potential fill bytes before the marker
        if (data[*pos] != 0xFF) // this is not a marker! all markers start with a 0xff byte!
            ERROR("A marker was expected at position %ld. Instead I found byte %x\n", *pos, data[*pos])
        unsigned char marker = data[*pos + 1]; // the marker code is the byte following 0xff
        if (marker == MARKER_EOI) { // end of image
            *pos += 2;
            break;
        }
        if (IS_LONE_MARKER(marker)) { // skip over all other lone markers
            *pos += 2;
            continue;
        }
        char *marker_name = get_marker_name(marker); 
        mxml_node_t *marker_node = mxmlNewElement(image, marker_name);
        NOTEOF(3)
        size_t segment_size = (data[*pos + 2] << 8) | data[*pos + 3];
        if (segment_size < 2) ERROR("segment size should include the 2 bytes that specify the segment size. Error in marker: %s\n", marker_name);
        *pos += 4;
        NOTEOF(segment_size - 2) // -2 because segment size includes the two bytes that specify segment size
        switch (marker) {
            case MARKER_APP0: 
                if (segment_size-2>=6 && 0==memcmp(data + *pos, "JFIF\0", 5)) {
                    mxmlElementSetAttrf(marker_node, "type", "JFIF");
                    i2xm_read_jfif(data, *pos + 5, segment_size - (2 + 5), marker_node);
                } else if (segment_size-2>0) {
                    i2xm_read_binary(data, *pos, segment_size - 2, marker_node);
                }
                break;
            case MARKER_APP1:
                if (segment_size-2>=6 && 0 == memcmp(data + *pos, "Exif\0\0", 6)) {
                    mxmlElementSetAttrf(marker_node, "type", "Exif");
                    i2xm_read_mpf_exif(data, *pos + 6, segment_size - (2 + 6), marker_node, images);
                } else if (segment_size-2>0) {
                    i2xm_read_binary(data, *pos, segment_size - 2, marker_node);
                }
                break;
            case MARKER_APP2:
                if (segment_size-2>=4 && 0 == memcmp(data + *pos, "MPF\0", 4)) {
                    mxmlElementSetAttrf(marker_node, "type", "MPF");
                    i2xm_read_mpf_exif(data, *pos + 4, segment_size - (2 + 4), marker_node, images);
                } else if (segment_size-2>0) {
                    i2xm_read_binary(data, *pos, segment_size - 2, marker_node);
                }
                break;
            case MARKER_APP3:
                if (segment_size-2>=8 && 0 == memcmp(data + *pos, "_JPSJPS_", 8)) {
                    mxmlElementSetAttrf(marker_node, "type", "JPS");
                    i2xm_read_sterdesc(data, *pos + 8, segment_size - (2 + 8), marker_node);
                } else if (segment_size-2>0) {
                    i2xm_read_binary(data, *pos, segment_size - 2, marker_node);
                }
                break;
            default: if (segment_size-2>0) i2xm_read_binary(data, *pos, segment_size - 2, marker_node);
        }
        *pos += segment_size - 2;
        if (marker == MARKER_SOS) { // compressed data follows
            size_t startpos = *pos;
            NOTEOF(1)
            while (data[*pos] != 0xFF || data[*pos + 1] == 0x00 || MARKER_IS_RST(data[*pos + 1])) *pos += 1;
            if (*pos-startpos>0) i2xm_read_binary(data, startpos, *pos - startpos, image);
        }
    }
}

void i2xm_read_binary(unsigned char *data,     // pointer to the mpo/jpg file in memory
                      size_t        pos,       // absolute position of the data interpreted as raw binary
                      size_t        size,      // size of the data interpreted as raw binary
                      mxml_node_t   *parent) { // In this parent node a new child element will be created, named "binary",
                                               // and the binary node will contain a custom child which contains a binary_t structure.
    if (size<0 || size > (SIZE_MAX >> 1)) {
        ERROR("something has gone quite wrong! binary chunk size shouldn't be negative, or immense!\n");
    }
    mxml_node_t *bin = mxmlNewElement(parent, "binary");
    binary_t *piece = (binary_t *) malloc(size + 2 * sizeof(size_t));
    piece->size = size;
    piece->offset = pos;
    memcpy(piece->data, data + pos, size);
    mxmlNewCustom(bin, piece, free);
}

static void ix2m_read_binary_with_possible_images(unsigned char *data, // pointer to the mpo/jpg file in memory
                                                  size_t        start, // inclusive absolute address
                                                  size_t        stop,  // exclusive absolute address. It could be size of data buffer,
                                                                       // or start of referenced image.
                                                  mxml_node_t   *parent) { // in this parent binary, and image elements will be created.
    size_t pos = start;
    if (stop - start > 10) { // arbitrary constant
        size_t npos = pos;
        while (npos < stop-1) {
            while (npos < stop-1 && !SOI_MARKER_AT(data, npos)) npos++;
            if (npos >= stop-1) break; // no more images found?
            // read binary data from pos to npos
            if (npos-pos>0) i2xm_read_binary(data, pos, npos - pos, parent);
            // read an Unreferenced image!:
            i2xm_read_image(data, &npos, stop, parent, 0, NULL);
            pos = npos;
            if (npos > stop) {
                // oops this overlaps, or overruns! Warning!!
                printf("WARNING: there seems to be an overrun, or overlap with a referenced image!!!\n");
            }
        }
    }
    // read the last  piece of binary data, in which we haven't found a SOI marker.
    if (stop - pos > 0) i2xm_read_binary(data, pos, stop - pos, parent);
}

mxml_node_t *i2xm_load_mem(void   *data,   // pointer to the mpo/jpg file in memory
                           size_t size,    // size of the mpo/jpg file in memory
                           int    interpret_concatenated) { // whether this function should look for unreferenced images
    size_t pos = 0;
    mxml_node_t *xml = mxmlNewXML("1.0");
    mxml_node_t *file = mxmlNewElement(xml, "file");

    referenced_images_t images;
    images.count = 0;
    /* The first image is read. If this image references other images, then they are added to
     * the images variable. This will be used later to find the referenced images.
     */
    i2xm_read_image((unsigned char *) data, &pos, size, file, 1, &images);
    sort_referenced_images(&images);
    int i;
    for (i=0;i<images.count;i++) {
        /* This loop executes once for each referenced image. But before handling the referenced image we have to handle
         * all binary data that is between the end of the last referenced image (pos), and the beginning of the current
         * referenced image (images.arr[i]). If we are given the flag interpret_concatenated flag to look for unreferenced
         * images, then we try to interpret parts of this binary data as images too.
         */
        if (images.arr[i] > 0 && // we have already read the first image, with absolute position 0, so ignore it.
            images.arr[i] >= pos) { // if images.arr[i]<pos then this is a reference to something which has already been parsed.
            if (interpret_concatenated)
                ix2m_read_binary_with_possible_images((unsigned char *) data, pos, min(images.arr[i], size), file);
            else if (images.arr[i] - pos > 0) i2xm_read_binary((unsigned char *) data, pos, images.arr[i] - pos, file);
            pos = images.arr[i];
            /* read a referenced image!
             * note: if this adds any more referenced images that point to smaller addresses in the file,
             * then these are gonna be ignored, as they have alreadybeen interpreted as either raw binary,
             * or unreferenced images. But if it adds referenced images that are in places that have not
             * been parsed yet, then these will be found as referenced.
             */
            i2xm_read_image((unsigned char *) data, &pos, size, file, 1, &images);
            sort_referenced_images_skip(&images, i+1);
        }
    }
    // we are done with all the referenced images. Now handle the rest of the file:
    if (interpret_concatenated)
        ix2m_read_binary_with_possible_images((unsigned char *) data, pos, size, file);
    else if (size - pos > 0) i2xm_read_binary((unsigned char *) data, pos, size - pos, file);
    return xml;
}

mxml_node_t *i2xm_load_file(FILE *fp, int interpret_concatenated) {
    size_t bytes_read = 0;
    char *buffer = read_entire_file(fp, 102400, &bytes_read);
    mxml_node_t *mpo = i2xm_load_mem(buffer, bytes_read, interpret_concatenated);
    free(buffer);
    return mpo;
}

mxml_node_t *i2xm_load_filename(char *fn, int interpret_concatenated) {
    FILE *fp = fopen(fn, "rb");
    if (fp==NULL) {
        printf("Error: Can't open this file: %s\n", fn); 
        return NULL;
    }
    mxml_node_t *mpo = i2xm_load_file(fp, interpret_concatenated);
    fclose(fp);
    return mpo;
}

