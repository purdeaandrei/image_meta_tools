#include "xmlmem_data_source_manager.h"
#include "common.h"
#include "markers.h"

typedef struct {
    struct jpeg_source_mgr pub;
    mxml_node_t *imagenode;
    mxml_node_t *markernode;
    int writing_data;
} my_source_mgr;

typedef my_source_mgr *my_src_ptr;

static JOCTET soi_bytes[] = {0xff, 0xd8};
static JOCTET eoi_bytes[] = {0xff, 0xd9};
static JOCTET fill_bytes[] = {0xff, 0xff};

static JOCTET marker_and_size[] = {0xff, 0, 0, 0};


METHODDEF(void)
init_source (j_decompress_ptr cinfo)
{
    my_src_ptr src = (my_src_ptr) cinfo->src;
    src->pub.next_input_byte = soi_bytes;
    src->pub.bytes_in_buffer = 2;
    src->markernode = src->imagenode->child;
    src->writing_data = 0;
}

static int i_dont_care_about(mxml_node_t *node) {
     if (IS_ELEMENT(node, "fill")) return 1;
     if (IS_ELEMENT(node, "binary")) return 0;
     int marker;
     if ((marker = get_marker_number(node->value.element.name))!=-1) {
        if (MARKER_IS_APP(marker)) return 1;
        return 0;
     }
     if (node->type == MXML_ELEMENT) {
        printf("WARNING: unknown element type: %s\n", node->value.element.name);
     }
     return 1;
}


METHODDEF(boolean)
fill_input_buffer (j_decompress_ptr cinfo)
{
    my_src_ptr src = (my_src_ptr) cinfo->src;
    while (src->markernode != NULL && i_dont_care_about(src->markernode)) src->markernode = src->markernode->next;
    if (src->markernode != NULL) {
        if (IS_ELEMENT(src->markernode, "binary")) {
            binary_t *b = (binary_t *) src->markernode->child->value.custom.data;
            src->pub.next_input_byte = b->data;
            src->pub.bytes_in_buffer = b->size;
            src->markernode = src->markernode->next; // advance
        } else {
            mxml_node_t *ch = src->markernode->child;
            if (!IS_ELEMENT(ch, "binary") || ch->next !=NULL || ch->child->type != MXML_CUSTOM) {
                WARNING("I can only interpret marker segments with a single binary child (hexdump/include), skipping...\n");
                src->pub.next_input_byte = fill_bytes;
                src->pub.bytes_in_buffer = 2;
            } else {
                binary_t *b = (binary_t *) ch->child->value.custom.data;
                if (src->writing_data) {
                    src->pub.next_input_byte = b->data;
                    src->pub.bytes_in_buffer = b->size;
                    src->writing_data = 0;
                    src->markernode = src->markernode->next; // advance
                } else {
                    src->pub.next_input_byte = marker_and_size;
                    src->pub.bytes_in_buffer = 4;
                    src->writing_data = 1;
                    marker_and_size[1] = get_marker_number(src->markernode->value.element.name);
                    JOCTET big_end = ((b->size + 2) >> 8) & 0xff;
                    JOCTET little_end = (b->size + 2) & 0xff;
                    marker_and_size[2] = big_end;
                    marker_and_size[3] = little_end;
                    // DON'T ADVANCE yet!
                }
            }
        }
    } else {
        src->pub.next_input_byte = eoi_bytes;
        src->pub.bytes_in_buffer = 2;
    }
    if (src->pub.bytes_in_buffer<=0) printf("WEIRD!\n");
    return TRUE;
}

METHODDEF(void)
term_source (j_decompress_ptr cinfo)
{ /* no work necessary */
}

/* this method is copied over from jdatasrc.c: */
METHODDEF(void)
skip_input_data (j_decompress_ptr cinfo, long num_bytes)
{
  struct jpeg_source_mgr * src = cinfo->src;

  /* Just a dumb implementation for now.  Could use fseek() except
   * it doesn't work on pipes.  Not clear that being smart is worth
   * any trouble anyway --- large skips are infrequent.
   */
  if (num_bytes > 0) {
    while (num_bytes > (long) src->bytes_in_buffer) {
      num_bytes -= (long) src->bytes_in_buffer;
      (void) (*src->fill_input_buffer) (cinfo);
      /* note we assume that fill_input_buffer will never return FALSE,
       * so suspension need not be handled.
       */
    }
    src->next_input_byte += (size_t) num_bytes;
    src->bytes_in_buffer -= (size_t) num_bytes;
  }
}

void jpeg_xmlmem_src (j_decompress_ptr cinfo, mxml_node_t *imagenode) {
    if (!IS_ELEMENT(imagenode, "image")) ERROR_DUMPNODE(imagenode, "this must be an image!\n");
    my_src_ptr src;
    if (cinfo->src == NULL) {
        cinfo->src = (struct jpeg_source_mgr *) (*cinfo->mem->alloc_small) ((j_common_ptr) cinfo, JPOOL_PERMANENT, sizeof(my_source_mgr));
    }
    src = (my_src_ptr) cinfo->src;
    src->pub.init_source = init_source;
    src->pub.fill_input_buffer = fill_input_buffer;
    src->pub.skip_input_data = skip_input_data;
    src->pub.resync_to_restart = jpeg_resync_to_restart;
    src->pub.term_source = term_source;
    
    src->pub.bytes_in_buffer = 0;
    src->pub.next_input_byte = NULL;
    
    src->imagenode = imagenode;
    src->markernode = NULL;
}
