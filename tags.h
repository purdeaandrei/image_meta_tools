/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

#ifndef __MPOTOOLS_TAGS_H
#define __MPOTOOLS_TAGS_H

#include <stdint.h>
#include <mxml.h>
#include "data_placement.h"
#include "referenced_images.h"
#include "writers.h"
#include "common.h"

// from EXIF spec:

#define TAG_TYPE_BYTE        1
#define TAG_TYPE_ASCII       2
#define TAG_TYPE_SHORT       3
#define TAG_TYPE_LONG        4
#define TAG_TYPE_RATIONAL    5
#define TAG_TYPE_UNDEFINED   7
#define TAG_TYPE_SLONG       9
#define TAG_TYPE_SRATIONAL  10

#define TAG_TYPE_STR(ID) ((ID)==TAG_TYPE_BYTE)?      "BYTE"      :( \
                         ((ID)==TAG_TYPE_ASCII)?     "ASCII"     :( \
                         ((ID)==TAG_TYPE_SHORT)?     "SHORT"     :( \
                         ((ID)==TAG_TYPE_LONG)?      "LONG"      :( \
                         ((ID)==TAG_TYPE_RATIONAL)?  "RATIONAL"  :( \
                         ((ID)==TAG_TYPE_UNDEFINED)? "UNDEFINED" :( \
                         ((ID)==TAG_TYPE_SLONG)?     "SLONG"     :( \
                         ((ID)==TAG_TYPE_SRATIONAL)? "SRATIONAL" :( \
                            "UNKNOWN" ))))))))
#define TAG_TYPE_FROM_STR(ID) ((ID)==NULL)?   -1  : ( \
                         (0==strcmp((ID),"BYTE"))?      TAG_TYPE_BYTE      :( \
                         (0==strcmp((ID),"ASCII"))?     TAG_TYPE_ASCII     :( \
                         (0==strcmp((ID),"SHORT"))?     TAG_TYPE_SHORT     :( \
                         (0==strcmp((ID),"LONG"))?      TAG_TYPE_LONG      :( \
                         (0==strcmp((ID),"RATIONAL"))?  TAG_TYPE_RATIONAL  :( \
                         (0==strcmp((ID),"UNDEFINED"))? TAG_TYPE_UNDEFINED :( \
                         (0==strcmp((ID),"SLONG"))?     TAG_TYPE_SLONG     :( \
                         (0==strcmp((ID),"SRATIONAL"))? TAG_TYPE_SRATIONAL :( \
                            -1 )))))))))

// From MPO spec:
// Index IFD tags:
#define TAG_MPFVersion           0xB000
#define TAG_NumberOfImages       0xB001
#define TAG_MPEntry              0xB002
#define TAG_ImageUIDList         0xB003
#define TAG_TotalFrames          0xB004
// Attribute IFD tags:
// Common Tags:
//#define TAG_MPFVersion           0xB000 // also an index IFD tag
#define TAG_MPIndividualNum      0xB101
// Multi-View Tags:
#define TAG_PanOrientation       0xB201
#define TAG_PanOverlap_H         0xB202
#define TAG_PanOverlap_V         0xB203
#define TAG_BaseViewpointNum     0xB204
#define TAG_ConvergenceAngle     0xB205
#define TAG_BaselineLength       0xB206
#define TAG_VerticalDivergence   0xB207
#define TAG_AxisDistance_X       0xB208
#define TAG_AxisDistance_Y       0xB209
#define TAG_AxisDistance_Z       0xB20A
#define TAG_YawAngle             0xB20B
#define TAG_PitchAngle           0xB20C
#define TAG_RollAngle            0xB20D

// From Exif spec:
#define TAG_Exif_IFD_Pointer     0x8769
#define TAG_GPS_Info_IFD_Pointer 0x8825
#define TAG_Interoperability_IFD_Pointer 0xA005
// TIFF tags used by Exif:
// tags relating to image data structure
#define TAG_ImageWidth           0x100
#define TAG_ImageLength          0x101 // TODO check this is TIFF spec! (should this be height?)
#define TAG_BitsPerSample        0x102
#define TAG_Compression          0x103
#define TAG_PhotometricInterpretation 0x106
#define TAG_Orientation          0x112
#define TAG_SamplesPerPixel      0x115
#define TAG_PlanarConfiguration  0x11C
#define TAG_YCbCrSubSampling     0x212
#define TAG_YCbCrPositioning     0x213
#define TAG_XResolution          0x11A
#define TAG_YResolution          0x11B
#define TAG_ResolutionUnit       0x128
// Tags relating to recording offset:
#define TAG_StripOffsets         0x111
#define TAG_RowsPerStrip         0x116
#define TAG_StripByteCounts      0x117
#define TAG_JPEGInterchangeFormat 0x201
#define TAG_JPEGInterchangeFormatLength 0x202
// Tags relating to image data characteristics:
#define TAG_TransferFunction     0x12D
#define TAG_WhitePoint           0x13E
#define TAG_PrimaryChromaticities 0x13F
#define TAG_YCbCrCoefficients    0x211
#define TAG_ReferenceBlackWhite  0x214
// Other tags:
#define TAG_DateTime             0x132
#define TAG_ImageDescription     0x10E
#define TAG_Make                 0x10F
#define TAG_Model                0x110
#define TAG_Software             0x131
#define TAG_Artist               0x13B
#define TAG_Copyright            0x8298
// tags relating to version
#define TAG_ExifVersion          0x9000
#define TAG_FlashpixVersion      0xA000
// tags relating to colorspace
#define TAG_ColorSpace           0xA001
// tags relating to image configuration
#define TAG_ComponentsConfiguration 0x9101
#define TAG_CompressedBitsPerPixel  0x9102
#define TAG_PixelXDimension         0xA002
#define TAG_PixelYDimension         0xA003
// tags relating to user information
#define TAG_MakerNote               0x927C
#define TAG_UserComment             0x9286
// tag relating to related file information
#define TAG_RelatedSoundFile        0xA004
// tags relating to date and time
#define TAG_DateTimeOriginal        0x9003
#define TAG_DateTimeDigitized       0x9004
#define TAG_SubsecTime              0x9290
#define TAG_SubsecTimeOriginal      0x9291
#define TAG_SubsecTimeDigitized     0x9292
//other tags
#define TAG_ImageUniqueID           0xA420
// tags relating to picture-taking conditions
#define TAG_ExposureTime             0x829A
#define TAG_FNumber                  0x829D
#define TAG_ExposureProgram          0x8822
#define TAG_SpectralSensitivity      0x8824
#define TAG_ISOSpeedRatings          0x8827
#define TAG_OECF                     0x8828
#define TAG_ShutterSpeedValue        0x9201
#define TAG_ApertureValue            0x9202
#define TAG_BrightnessValue          0x9203
#define TAG_ExposureBiasValue        0x9204
#define TAG_MaxApertureValue         0x9205
#define TAG_SubjectDistance          0x9206
#define TAG_MeteringMode             0x9207
#define TAG_LightSource              0x9208
#define TAG_Flash                    0x9209
#define TAG_FocalLength              0x920A
#define TAG_SubjectArea              0x9214
#define TAG_FlashEnergy              0xA20B
#define TAG_SpatialFrequencyResponse 0xA20C
#define TAG_FocalPlaneXResolution    0xA20E
#define TAG_FocalPlaneYResolution    0xA20F
#define TAG_FocalPlaneResolutionUnit 0xA210
#define TAG_SubjectLocation          0xA214
#define TAG_ExposureIndex            0xA215
#define TAG_SensingMethod            0xA217
#define TAG_FileSource               0xA300
#define TAG_SceneType                0xA301
#define TAG_CFAPattern               0xA302
#define TAG_CustomRendered           0xA401
#define TAG_ExposureMode             0xA402
#define TAG_WhiteBalance             0xA403
#define TAG_DigitalZoomRatio         0xA404
#define TAG_FocalLengthIn35mmFilm    0xA405
#define TAG_SceneCaptureType         0xA406
#define TAG_GainControl              0xA407
#define TAG_Contrast                 0xA408
#define TAG_Saturation               0xA409
#define TAG_Sharpness                0xA40A
#define TAG_DeviceSettingDescription 0xA40B
#define TAG_SubjectDistanceRange     0xA40C
// GPS Attribute information
#define TAG_GPSVersionID             0x00
#define TAG_GPSLatitudeRef           0x01
#define TAG_GPSLatitude              0x02
#define TAG_GPSLongitudeRef          0x03
#define TAG_GPSLongitude             0x04
#define TAG_GPSAltitudeRef           0x05
#define TAG_GPSAltitude              0x06
#define TAG_GPSTimeStamp             0x07
#define TAG_GPSSatellites            0x08
#define TAG_GPSStatus                0x09
#define TAG_GPSMeasureMode           0x0A
#define TAG_GPSDOP                   0x0B
#define TAG_GPSSpeedRef              0x0C
#define TAG_GPSSpeed                 0x0D
#define TAG_GPSTrackRef              0x0E
#define TAG_GPSTrack                 0x0F
#define TAG_GPSImgDirectionRef       0x10
#define TAG_GPSImgDirection          0x11
#define TAG_GPSMapDatum              0x12
#define TAG_GPSDestLatitudeRef       0x13
#define TAG_GPSDestLatitude          0x14
#define TAG_GPSDestLongitudeRef      0x15
#define TAG_GPSDestLongitude         0x16
#define TAG_GPSDestBearingRef        0x17
#define TAG_GPSDestBearing           0x18
#define TAG_GPSDestDistanceRef       0x19
#define TAG_GPSDestDistance          0x1A
#define TAG_GPSProcessingMethod      0x1B
#define TAG_GPSAreaInformation       0x1C
#define TAG_GPSDateStamp             0x1D
#define TAG_GPSDifferential          0x1E
//Interoperability information
#define TAG_InteroperabilityIndex    0x01

// tags from other sources:
#define TAG_InteroperabilityVersion  0x02 // dcf spec.
#define TAG_PrintImageMatching   0xC4A5 // libexif
#define TAG_SensitivityType      0x8830 // exiftool

#define CUSTOMFUNCTION_BASE(NAME) void  NAME (                           \
    unsigned char *data,        /* the mpo/jpeg file in memory */        \
    size_t        absolute_pos, /* the absolute position of the value */ \
    size_t        endian_pos,   /* the position of the endian marker */  \
    size_t        size,         /* the size of the marker where this is, starting from endian_pos */ \
    uint16_t      type,         /* the TAG_TYPE_* of the tag */          \
    uint32_t      count,        /* the number of elements in this tag */ \
    uint32_t      bytespertag,  /* the size of the value of this tag */  \
    mxml_node_t   *tag,         /* the xml node for this tag */          \
    data_placement_db_t *dp,                                             \
    char          *parent_full_name,   /* full name of the parent, so that i can calculate other full_names */ \
    char          *full_name,   /* full name of tag including parent ifds */ \
    referenced_images_t *images, /* this is where the MP_Entry tags will place referenced images so we can later retrieve them */ \
    int           le)  /* Endian setting. 1 = little endian, 0 = big endian. */

#define CUSTOMINTERPRET_BASE(NAME) int  NAME ( \
    mxml_node_t *node, \
    pointer_t *pointer, \
    char *full_name, \
    void *count, \
    data_object_t **dobj, \
    data_object_t *parent, \
    writer_t *objdb, \
    size_t endian_pos, \
    int le )
    

typedef struct custom_tag_s {
    uint16_t id;
    char *name;
    uint16_t def_type; // 0 means that this has not been defined in a standard, or it can be one of multiple types (don't confuse with TAG_TYPE_UNDEFINED, which means it has been defined as undefined)
    uint32_t def_count; // 0 means it is not an obligatory fixed value
    CUSTOMFUNCTION_BASE( (*customfunction) );
    CUSTOMINTERPRET_BASE( (*custominterpret) );
    // NULL if standard format
    int pointer; // used for custom functions where the value is a pointer and it is not detected as a pointer by standard logic, or the data size is unknown
} custom_tag_t;

// here come the identification bits:
#define GPSTAG   0x00010000
#define INTERTAG 0x00020000

custom_tag_t *getCustomTag(custom_tag_t *tags, uint16_t tag_id);
custom_tag_t *getCustomTagFromName(custom_tag_t *tags, const char *tag_name);

#define CUSTOMFUNCTION(NAME) CUSTOMFUNCTION_BASE(TAG_customfunction_ ## NAME)
#define CUSTOMINTERPRET(NAME) CUSTOMINTERPRET_BASE(TAG_custominterpret_ ## NAME)
CUSTOMFUNCTION(UndefinedVersion);
  CUSTOMINTERPRET(UndefinedVersion);

CUSTOMFUNCTION(Compression);
  CUSTOMINTERPRET(Compression);
CUSTOMFUNCTION(PhotometricInterpretation);
  CUSTOMINTERPRET(PhotometricInterpretation);
CUSTOMFUNCTION(Orientation);
  CUSTOMINTERPRET(Orientation);

CUSTOMFUNCTION(PlanarConfiguration);
  CUSTOMINTERPRET(PlanarConfiguration);
CUSTOMFUNCTION(YCbCrSubsampling);
  CUSTOMINTERPRET(YCbCrSubsampling);
CUSTOMFUNCTION(YCbCrPositioning);
  CUSTOMINTERPRET(YCbCrPositioning);
CUSTOMFUNCTION(ResolutionUnit);
  CUSTOMINTERPRET(ResolutionUnit);


CUSTOMFUNCTION(Exif_IFD_Pointer);
  CUSTOMINTERPRET(Exif_IFD_Pointer);
CUSTOMFUNCTION(Interoperability_IFD_Pointer);
  CUSTOMINTERPRET(Interoperability_IFD_Pointer);
CUSTOMFUNCTION(GPS_Info_IFD_Pointer);
  CUSTOMINTERPRET(GPS_Info_IFD_Pointer);

CUSTOMFUNCTION(ColorSpace);
  CUSTOMINTERPRET(ColorSpace);
CUSTOMFUNCTION(ComponentsConfiguration);
  CUSTOMINTERPRET(ComponentsConfiguration);

CUSTOMFUNCTION(MakerNote);
  CUSTOMINTERPRET(MakerNote);

CUSTOMFUNCTION(ExposureProgram);
  CUSTOMINTERPRET(ExposureProgram);
CUSTOMFUNCTION(MeteringMode);
  CUSTOMINTERPRET(MeteringMode);
CUSTOMFUNCTION(LightSource);
  CUSTOMINTERPRET(LightSource);
CUSTOMFUNCTION(Flash);
  CUSTOMINTERPRET(Flash);
CUSTOMFUNCTION(SubjectArea);
  CUSTOMINTERPRET(SubjectArea);
CUSTOMFUNCTION(SensingMethod);
  CUSTOMINTERPRET(SensingMethod);
CUSTOMFUNCTION(FileSource);
  CUSTOMINTERPRET(FileSource);
CUSTOMFUNCTION(SceneType);
  CUSTOMINTERPRET(SceneType);
CUSTOMFUNCTION(CustomRendered);
  CUSTOMINTERPRET(CustomRendered);
CUSTOMFUNCTION(ExposureMode);
  CUSTOMINTERPRET(ExposureMode);
CUSTOMFUNCTION(WhiteBalance);
  CUSTOMINTERPRET(WhiteBalance);
CUSTOMFUNCTION(SceneCaptureType);
  CUSTOMINTERPRET(SceneCaptureType);
CUSTOMFUNCTION(GainControl);
  CUSTOMINTERPRET(GainControl);
CUSTOMFUNCTION(Contrast);
  CUSTOMINTERPRET(Contrast);
CUSTOMFUNCTION(Saturation);
  CUSTOMINTERPRET(Saturation);
CUSTOMFUNCTION(Sharpness);
  CUSTOMINTERPRET(Sharpness);
CUSTOMFUNCTION(SubjectDistanceRange);
  CUSTOMINTERPRET(SubjectDistanceRange);
CUSTOMFUNCTION(SensitivityType);
  CUSTOMINTERPRET(SensitivityType);

// gps:
CUSTOMFUNCTION(AltitudeRef);

// special functions:

CUSTOMFUNCTION(DotSeparatedBytes);
  CUSTOMINTERPRET(DotSeparatedBytes);
CUSTOMFUNCTION(DegreesMinutesSeconds);
  CUSTOMINTERPRET(DegreesMinutesSeconds);
CUSTOMFUNCTION(HourMinuteSecond);
  CUSTOMINTERPRET(HourMinuteSecond);

CUSTOMFUNCTION(InterchangeFormat);
  CUSTOMINTERPRET(InterchangeFormat);
CUSTOMFUNCTION(InterchangeFormatLength);
  CUSTOMINTERPRET(InterchangeFormatLength);

CUSTOMFUNCTION(MPEntry);
  CUSTOMINTERPRET(MPEntry);
CUSTOMFUNCTION(PanOrientation);
  CUSTOMINTERPRET(PanOrientation);
CUSTOMFUNCTION(All1UnknownRational);
  CUSTOMINTERPRET(All1UnknownRational);

CUSTOMINTERPRET(StringIsAlwaysAtLeast2Bytes);

extern custom_tag_t global_tags[];

void resetTags();

#endif
