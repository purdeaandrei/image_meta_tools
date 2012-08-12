#ifndef __MPOTOOLS_MARKERS_H
#define __MPOTOOLS_MARKERS_H

/* The following marker values are taken from the ITU-T Recommendation T.81, page 32 */

#define MARKER_SOF_BASE (0xC0)
#define MARKER_SOF_MASK (0xF0)
#define MARKER_SOF_NR_MASK (0x0F)

#define MARKER_SOF0 (0xC0)
#define MARKER_SOF1 (0xC1)
#define MARKER_SOF2 (0xC2)
#define MARKER_SOF3 (0xC3)

#define MARKER_SOF5 (0xC5)
#define MARKER_SOF6 (0xC6)
#define MARKER_SOF7 (0xC7)

#define MARKER_SOF9 (0xC9)
#define MARKER_SOF10 (0xCA)
#define MARKER_SOF11 (0xCB)

#define MARKER_SOF13 (0xCD)
#define MARKER_SOF14 (0xCE)
#define MARKER_SOF15 (0xCF)

#define MARKER_JPG (0xC8)
#define MARKER_DHT (0xC4)
#define MARKER_DAC (0xCC)

#define MARKER_IS_SOF(marker) ((((marker) & MARKER_SOF_MASK) == MARKER_SOF_BASE) && \
                                ((marker) != MARKER_JPG) && \
                                ((marker) != MARKER_DHT) && \
                                ((marker) != MARKER_DAC))

#define MARKER_RST_BASE (0xD0)
#define MARKER_RST_MASK (0xF8)
#define MARKER_RST_NR_MASK (0x07)

#define MARKER_RST0 (0xD0)
#define MARKER_RST1 (0xD1)
#define MARKER_RST2 (0xD2)
#define MARKER_RST3 (0xD3)
#define MARKER_RST4 (0xD4)
#define MARKER_RST5 (0xD5)
#define MARKER_RST6 (0xD6)
#define MARKER_RST7 (0xD7)

#define MARKER_IS_RST(marker) (((marker) & MARKER_RST_MASK) == MARKER_RST_BASE)

#define MARKER_SOI (0xD8)
#define MARKER_EOI (0xD9)
#define MARKER_SOS (0xDA)
#define MARKER_DQT (0xDB)
#define MARKER_DNL (0xDC)
#define MARKER_DRI (0xDD)
#define MARKER_DHP (0xDE)
#define MARKER_EXP (0xDF)

#define MARKER_APP_BASE (0xE0)
#define MARKER_APP_MASK (0xF0)
#define MARKER_APP_NR_MASK (0x0F)
#define MARKER_APP0  (0xE0)
#define MARKER_APP1  (0xE1)
#define MARKER_APP2  (0xE2)
#define MARKER_APP3  (0xE3)
#define MARKER_APP4  (0xE4)
#define MARKER_APP5  (0xE5)
#define MARKER_APP6  (0xE6)
#define MARKER_APP7  (0xE7)
#define MARKER_APP8  (0xE8)
#define MARKER_APP9  (0xE9)
#define MARKER_APP10 (0xEA)
#define MARKER_APP11 (0xEB)
#define MARKER_APP12 (0xEC)
#define MARKER_APP13 (0xED)
#define MARKER_APP14 (0xEE)
#define MARKER_APP15 (0xEF)

#define MARKER_IS_APP(marker) (((marker) & MARKER_APP_MASK) == MARKER_APP_BASE)

#define MARKER_JPG_BASE (0xF0)
#define MARKER_JPG_MASK (0xF0)
#define MARKER_JPG_NR_MASK (0x0F)

#define MARKER_JPG0  (0xF0)
#define MARKER_JPG1  (0xF1)
#define MARKER_JPG2  (0xF2)
#define MARKER_JPG3  (0xF3)
#define MARKER_JPG4  (0xF4)
#define MARKER_JPG5  (0xF5)
#define MARKER_JPG6  (0xF6)
#define MARKER_JPG7  (0xF7)
#define MARKER_JPG8  (0xF8)
#define MARKER_JPG9  (0xF9)
#define MARKER_JPG10 (0xFA)
#define MARKER_JPG11 (0xFB)
#define MARKER_JPG12 (0xFC)
#define MARKER_JPG13 (0xFD)

#define MARKER_IS_JPG(marker) ((((marker) & MARKER_JPG_MASK) == MARKER_JPG_BASE) && \
                                ((marker) != MARKER_COM) && \
                                ((marker) != 0xFF))

#define MARKER_COM (0xFE)
#define MARKER_TEM (0x01)

#define IS_LONE_MARKER(marker) (MARKER_IS_RST((marker)) || \
                                  ((marker) == MARKER_SOI) || \
                                  ((marker) == MARKER_EOI) || \
                                  ((marker) == MARKER_TEM))

#define IS_RESERVED(marker) ((marker) >= 2 && (marker) <= 0xBF)

/* 'get_marker_name()' - returns a string representing the name of the marker.
sometimes the returned string doesn't always stay constant. */
extern char *get_marker_name(unsigned char marker);
extern int get_marker_number(char *name);

#endif
