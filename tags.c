/* Copyright (c) 2012, Movidius Ltd.
   Released under the BSD 3-clause license.
   Author: Purdea Andrei <andrei -at- purdea.ro>
                         <andrei.purdea -at- movidius.com> */

#include "tags.h"
#include "image2xmlmem.h"
#include "xmlmem2image.h"

#include "referenced_images.h"
#include "common.h"
#include "mp_entry.h"

custom_tag_t global_tags[] = {
//{ 0xFFFF                   , NULL                    , 0                  , 0 , NULL, 0 }, // ending record // TODO
// MPO tags:
 // Index IFD tags:
 { TAG_MPFVersion           , "MPFVersion"            , TAG_TYPE_UNDEFINED , 4, TAG_customfunction_UndefinedVersion, TAG_custominterpret_UndefinedVersion, 0 },
 { TAG_NumberOfImages       , "NumberOfImages"        , TAG_TYPE_LONG      , 1, NULL, NULL, 0 },
 { TAG_MPEntry              , "MPEntry"               , TAG_TYPE_UNDEFINED , 0, TAG_customfunction_MPEntry, TAG_custominterpret_MPEntry, 0 },
 { TAG_ImageUIDList         , "ImageUIDList"          , TAG_TYPE_UNDEFINED , 0, NULL, NULL, 0 }, // todo. count should be divisible by 33!
 { TAG_TotalFrames          , "TotalFrames"           , TAG_TYPE_UNDEFINED , 1, NULL, NULL, 0 },

 // Attribute IFD tags:
 // Common tags:
 //  TAG_MPFVersion  // Also an Index IFD tag
 {TAG_MPIndividualNum       , "MPIndividualNum"       , TAG_TYPE_LONG      , 1, NULL, NULL, 0 },
 // Multi-view Tags:
 { TAG_PanOrientation       , "PanOrientation"        , TAG_TYPE_LONG      , 1, TAG_customfunction_PanOrientation, TAG_custominterpret_PanOrientation, 0 },
 { TAG_PanOverlap_H         , "PanOverlap_H"          , TAG_TYPE_RATIONAL  , 1, NULL, NULL, 0 },
 { TAG_PanOverlap_V         , "PanOverlap_V"          , TAG_TYPE_RATIONAL  , 1, NULL, NULL, 0 },
 { TAG_BaseViewpointNum     , "BaseViewpointNum"      , TAG_TYPE_LONG      , 1, NULL, NULL, 0 },
 { TAG_ConvergenceAngle     , "ConvergenceAngle"      , TAG_TYPE_SRATIONAL , 1, TAG_customfunction_All1UnknownRational, TAG_custominterpret_All1UnknownRational, 0 },
 { TAG_BaselineLength       , "BaselineLength"        , TAG_TYPE_RATIONAL  , 1, TAG_customfunction_All1UnknownRational, TAG_custominterpret_All1UnknownRational, 0 },
 { TAG_VerticalDivergence   , "VerticalDivergence"    , TAG_TYPE_SRATIONAL , 1, TAG_customfunction_All1UnknownRational, TAG_custominterpret_All1UnknownRational, 0 },
 { TAG_AxisDistance_X       , "AxisDistance_X"        , TAG_TYPE_SRATIONAL , 1, NULL, NULL, 0 },
 { TAG_AxisDistance_Y       , "AxisDistance_Y"        , TAG_TYPE_SRATIONAL , 1, NULL, NULL, 0 },
 { TAG_AxisDistance_Z       , "AxisDistance_Z"        , TAG_TYPE_SRATIONAL , 1, NULL, NULL, 0 },
 { TAG_YawAngle             , "YawAngle"              , TAG_TYPE_SRATIONAL , 1, NULL, NULL, 0 },
 { TAG_PitchAngle           , "PitchAngle"            , TAG_TYPE_SRATIONAL , 1, NULL, NULL, 0 },
 { TAG_RollAngle            , "RollAngle"             , TAG_TYPE_SRATIONAL , 1, NULL, NULL, 0 },
// Exif tags:
 { TAG_Exif_IFD_Pointer     , "Exif_IFD_Pointer"      , TAG_TYPE_LONG      , 1, TAG_customfunction_Exif_IFD_Pointer, TAG_custominterpret_Exif_IFD_Pointer, 1 },
 { TAG_GPS_Info_IFD_Pointer , "GPS_Info_IFD_Pointer"  , TAG_TYPE_LONG      , 1, TAG_customfunction_GPS_Info_IFD_Pointer, TAG_custominterpret_GPS_Info_IFD_Pointer, 1 },
 { TAG_Interoperability_IFD_Pointer , "Interoperability_IFD_Pointer" , TAG_TYPE_LONG, 1, TAG_customfunction_Interoperability_IFD_Pointer, TAG_custominterpret_Interoperability_IFD_Pointer, 1 },
 // TIFF tags used by Exif:
 { TAG_ImageWidth           , "ImageWidth"            , 0                  , 1, NULL, NULL, 0 }, // tag type can be short or long
 { TAG_ImageLength          , "ImageLength"           , 0                  , 1, NULL, NULL, 0 }, // tag type can be short or long
 { TAG_BitsPerSample        , "BitsPerSample"         , TAG_TYPE_SHORT     , 3, NULL, NULL, 0 },
 { TAG_Compression          , "Compression"           , TAG_TYPE_SHORT     , 1, TAG_customfunction_Compression, TAG_custominterpret_Compression, 0 },
 { TAG_PhotometricInterpretation , "PhotometricInterpretation" , TAG_TYPE_SHORT     , 1, TAG_customfunction_PhotometricInterpretation, TAG_custominterpret_PhotometricInterpretation, 0 },
 { TAG_Orientation          , "Orientation"           , TAG_TYPE_SHORT     , 1, TAG_customfunction_Orientation, TAG_custominterpret_Orientation, 0 },
 { TAG_SamplesPerPixel      , "SamplesPerPixel"       , TAG_TYPE_SHORT     , 1, NULL, NULL, 0 },
 { TAG_PlanarConfiguration  , "PlanarConfiguration"   , TAG_TYPE_SHORT     , 1, TAG_customfunction_PlanarConfiguration, TAG_custominterpret_PlanarConfiguration, 0 },
 { TAG_YCbCrSubSampling     , "YCbCrSubSampling"      , TAG_TYPE_SHORT     , 2, TAG_customfunction_YCbCrSubsampling, TAG_custominterpret_YCbCrSubsampling, 0 },
 { TAG_YCbCrPositioning     , "YCbCrPositioning"      , TAG_TYPE_SHORT     , 1, TAG_customfunction_YCbCrPositioning, TAG_custominterpret_YCbCrPositioning, 0 },
 { TAG_XResolution          , "XResolution"           , TAG_TYPE_RATIONAL  , 1, NULL, NULL, 0 },
 { TAG_YResolution          , "YResolution"           , TAG_TYPE_RATIONAL  , 1, NULL, NULL, 0 },
 { TAG_ResolutionUnit       , "ResolutionUnit"        , TAG_TYPE_SHORT     , 1, TAG_customfunction_ResolutionUnit, TAG_custominterpret_ResolutionUnit, 0 },
 // Tags relating to recording offset
 { TAG_StripOffsets         , "StripOffsets"          , 0                  , 0, NULL, NULL, 0 }, // tag type can be short or long
 { TAG_RowsPerStrip         , "RowsPerStrip"          , 0                  , 1, NULL, NULL, 0 }, // tag type can be short or long
 { TAG_StripByteCounts      , "StripByteCounts"       , 0                  , 0, NULL, NULL, 0 }, // tag type can be short or long
 { TAG_JPEGInterchangeFormat , "JPEGInterchangeFormat", TAG_TYPE_LONG      , 1, TAG_customfunction_InterchangeFormat, TAG_custominterpret_InterchangeFormat, 1 },
 { TAG_JPEGInterchangeFormatLength , "JPEGInterchangeFormatLength", TAG_TYPE_LONG      , 1, TAG_customfunction_InterchangeFormatLength, TAG_custominterpret_InterchangeFormatLength, 0 },
 // Tags relating to image data characteristics:
 { TAG_TransferFunction     , "TransferFunction"      , TAG_TYPE_SHORT     , 3*256, NULL, NULL, 0 },
 { TAG_WhitePoint           , "WhitePoint"            , TAG_TYPE_RATIONAL  , 2, NULL, NULL, 0 },
 { TAG_PrimaryChromaticities , "PrimaryChromaticities", TAG_TYPE_RATIONAL  , 6, NULL, NULL, 0 },
 { TAG_YCbCrCoefficients    , "YCbCrCoefficients"     , TAG_TYPE_RATIONAL  , 3, NULL, NULL, 0 },
 { TAG_ReferenceBlackWhite  , "ReferenceBlackWhite"   , TAG_TYPE_RATIONAL  , 6, NULL, NULL, 0 },
 // Other tags:
 { TAG_DateTime             , "DateTime"              , TAG_TYPE_ASCII     , 20, NULL, NULL, 0 },
 { TAG_ImageDescription     , "ImageDescription"      , TAG_TYPE_ASCII     , 0, NULL, NULL, 0 },
 { TAG_Make                 , "Make"                  , TAG_TYPE_ASCII     , 0, NULL, NULL, 0 },
 { TAG_Model                , "Model"                 , TAG_TYPE_ASCII     , 0, NULL, NULL, 0 },
 { TAG_Software             , "Software"              , TAG_TYPE_ASCII     , 0, NULL, NULL, 0 },
 { TAG_Artist               , "Artist"                , TAG_TYPE_ASCII     , 0, NULL, NULL, 0 },
 { TAG_Copyright            , "Copyright"             , TAG_TYPE_ASCII     , 0, NULL, NULL, 0 },
 // tags from other sources:
 { TAG_PrintImageMatching   , "PrintImageMatching"    , TAG_TYPE_UNDEFINED , 0, NULL, NULL, 0 },  // libexif
 { TAG_SensitivityType      , "SensitivityType"       , TAG_TYPE_SHORT     , 1, TAG_customfunction_SensitivityType, TAG_custominterpret_SensitivityType, 0 },  // exiftool
 // tags relating to Version
 { TAG_ExifVersion          , "ExifVersion"           , TAG_TYPE_UNDEFINED , 4, TAG_customfunction_UndefinedVersion, TAG_custominterpret_UndefinedVersion, 0 },
 { TAG_FlashpixVersion      , "FlashpixVersion"       , TAG_TYPE_UNDEFINED , 4, TAG_customfunction_UndefinedVersion, TAG_custominterpret_UndefinedVersion, 0 },
 // tag relating to colorspace:
 { TAG_ColorSpace           , "ColorSpace"            , TAG_TYPE_SHORT     , 1, TAG_customfunction_ColorSpace, TAG_custominterpret_ColorSpace, 0 },
 // tags relating to image configuration
 { TAG_PixelXDimension      , "PixelXDimension"       , 0                  , 1, NULL, NULL, 0 }, // can be short or long
 { TAG_PixelYDimension      , "PixelYDimension"       , 0                  , 1, NULL, NULL, 0 }, // can be short or long
 { TAG_ComponentsConfiguration , "ComponentsConfiguration", TAG_TYPE_UNDEFINED, 4, TAG_customfunction_ComponentsConfiguration, TAG_custominterpret_ComponentsConfiguration, 0 },
 { TAG_CompressedBitsPerPixel, "CompressedBitsPerPixel", TAG_TYPE_RATIONAL , 1, NULL, NULL, 0 },
 // tags related to user information
 { TAG_MakerNote            , "MakerNote"             , TAG_TYPE_UNDEFINED , 0, TAG_customfunction_MakerNote, NULL, 0 }, // NO need for custom interpreter.
 { TAG_UserComment          , "UserComment"           , TAG_TYPE_UNDEFINED , 0, NULL, NULL, 0 }, // TODO!!!
 // tag relating to related file
 { TAG_RelatedSoundFile     , "RelatedSoundFile"      , TAG_TYPE_ASCII     , 13, NULL, NULL, 0 },
 { TAG_DateTimeOriginal     , "DateTimeOriginal"      , TAG_TYPE_ASCII     , 20, NULL, NULL, 0 },
 { TAG_DateTimeDigitized    , "DateTimeDigitized"     , TAG_TYPE_ASCII     , 20, NULL, NULL, 0 },
 { TAG_SubsecTime           , "SubsecTime"            , TAG_TYPE_ASCII     , 0 , NULL, NULL, 0 },
 { TAG_SubsecTimeOriginal   , "SubsecTimeOriginal"    , TAG_TYPE_ASCII     , 0 , NULL, NULL, 0 },
 { TAG_SubsecTimeDigitized  , "SubsecTimeDigitized"   , TAG_TYPE_ASCII     , 0 , NULL, NULL, 0 },
 // tags relating to picture-taking conditions
 { TAG_ExposureTime         , "ExposureTime"          , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_FNumber              , "FNumber"               , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_ExposureProgram      , "ExposureProgram"       , TAG_TYPE_SHORT     , 1 , TAG_customfunction_ExposureProgram, TAG_custominterpret_ExposureProgram, 0 },
 { TAG_SpectralSensitivity  , "SpectralSensitivity"   , TAG_TYPE_ASCII     , 0 , NULL, NULL, 0 },
 { TAG_ISOSpeedRatings      , "ISOSpeedRatings"       , TAG_TYPE_SHORT     , 0 , NULL, NULL, 0 },
 { TAG_OECF                 , "OECF"                  , TAG_TYPE_UNDEFINED , 0 , NULL, NULL, 0 }, // TODO!!!
 { TAG_ShutterSpeedValue    , "ShutterSpeedValue"     , TAG_TYPE_SRATIONAL , 1 , NULL, NULL, 0 },
 { TAG_ApertureValue        , "ApertureValue"         , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_BrightnessValue      , "BrightnessValue"       , TAG_TYPE_SRATIONAL , 1 , NULL, NULL, 0 },
 { TAG_ExposureBiasValue    , "ExposureBiasValue"     , TAG_TYPE_SRATIONAL , 1 , NULL, NULL, 0 },
 { TAG_MaxApertureValue     , "MaxApertureValue"      , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_SubjectDistance      , "SubjectDistance"       , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_MeteringMode         , "MeteringMode"          , TAG_TYPE_SHORT     , 1 , TAG_customfunction_MeteringMode, TAG_custominterpret_MeteringMode, 0 },
 { TAG_LightSource          , "LightSource"           , TAG_TYPE_SHORT     , 1 , TAG_customfunction_LightSource, TAG_custominterpret_LightSource, 0 },
 { TAG_Flash                , "Flash"                 , TAG_TYPE_SHORT     , 1 , TAG_customfunction_Flash, TAG_custominterpret_Flash, 0 },
 { TAG_SubjectArea          , "SubjectArea"           , TAG_TYPE_SHORT     , 0 , TAG_customfunction_SubjectArea, TAG_custominterpret_SubjectArea, 0 },
 { TAG_FocalLength          , "FocalLength"           , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_FlashEnergy          , "FlashEnergy"           , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_SpatialFrequencyResponse, "SpatialFrequencyResponse", TAG_TYPE_UNDEFINED, 0 , NULL, NULL, 0 }, // TODO!!!
 { TAG_FocalPlaneXResolution, "FocalPlaneXResolution" , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_FocalPlaneYResolution, "FocalPlaneYResolution" , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_FocalPlaneResolutionUnit, "FocalPlaneResolutionUnit" , TAG_TYPE_SHORT  , 1 , TAG_customfunction_ResolutionUnit, TAG_custominterpret_ResolutionUnit, 0 },
 { TAG_SubjectLocation      , "SubjectLocation"       , TAG_TYPE_SHORT     , 2 , TAG_customfunction_SubjectArea, TAG_custominterpret_SubjectArea, 0 },
 { TAG_ExposureIndex        , "ExposureIndex"         , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_SensingMethod        , "SensingMethod"         , TAG_TYPE_SHORT     , 1 , TAG_customfunction_SensingMethod, TAG_custominterpret_SensingMethod, 0 },
 { TAG_FileSource           , "FileSource"            , TAG_TYPE_UNDEFINED , 1 , TAG_customfunction_FileSource, TAG_custominterpret_FileSource, 0 },
 { TAG_SceneType            , "SceneType"             , TAG_TYPE_UNDEFINED , 1 , TAG_customfunction_SceneType, TAG_custominterpret_SceneType, 0 },
 { TAG_CFAPattern           , "CFAPattern"            , TAG_TYPE_UNDEFINED , 0 , NULL, NULL, 0 }, // TODO!!!
 { TAG_CustomRendered       , "CustomRendered"        , TAG_TYPE_SHORT     , 1 , TAG_customfunction_CustomRendered, TAG_custominterpret_CustomRendered, 0 },
 { TAG_ExposureMode         , "ExposureMode"          , TAG_TYPE_SHORT     , 1 , TAG_customfunction_ExposureMode, TAG_custominterpret_ExposureMode, 0 },
 { TAG_WhiteBalance         , "WhiteBalance"          , TAG_TYPE_SHORT     , 1 , TAG_customfunction_WhiteBalance, TAG_custominterpret_WhiteBalance, 0 },
 { TAG_DigitalZoomRatio     , "DigitalZoomRatio"      , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_FocalLengthIn35mmFilm, "FocalLengthIn35mmFilm" , TAG_TYPE_SHORT     , 1 , NULL, NULL, 0 },
 { TAG_SceneCaptureType     , "SceneCaptureType"      , TAG_TYPE_SHORT     , 1 , TAG_customfunction_SceneCaptureType, TAG_custominterpret_SceneCaptureType, 0 },
 { TAG_GainControl          , "GainControl"           , TAG_TYPE_SHORT     , 1 , TAG_customfunction_GainControl, TAG_custominterpret_GainControl, 0 },
 { TAG_Contrast             , "Contrast"              , TAG_TYPE_SHORT     , 1 , TAG_customfunction_Contrast, TAG_custominterpret_Contrast, 0 },
 { TAG_Saturation           , "Saturation"            , TAG_TYPE_SHORT     , 1 , TAG_customfunction_Saturation, TAG_custominterpret_Saturation, 0 },
 { TAG_Sharpness            , "Sharpness"             , TAG_TYPE_SHORT     , 1 , TAG_customfunction_Sharpness, TAG_custominterpret_Sharpness, 0 },
 { TAG_DeviceSettingDescription, "DeviceSettingDescription", TAG_TYPE_UNDEFINED, 0 , NULL, NULL, 0 }, //TODO?
 { TAG_SubjectDistanceRange , "SubjectDistanceRange"  , TAG_TYPE_SHORT     , 1 , TAG_customfunction_SubjectDistanceRange, TAG_custominterpret_SubjectDistanceRange, 0 },
 { TAG_ImageUniqueID        , "ImageUniqueID"         , TAG_TYPE_ASCII     , 33 , NULL, NULL, 0 },
 { 0xFFFF                   , NULL                    , 0                  , 0 , NULL, NULL, 0 }, // ending record
};
custom_tag_t GPS_tags[] = {
 // tags relating to gps:
 { TAG_GPSVersionID         , "GPSVersionID"          , TAG_TYPE_BYTE      , 4 , TAG_customfunction_DotSeparatedBytes, TAG_custominterpret_DotSeparatedBytes, 0 },
 { TAG_GPSLatitudeRef       , "GPSLatitudeRef"        , TAG_TYPE_ASCII     , 2 , NULL, NULL, 0 },
 { TAG_GPSLatitude          , "GPSLatitude"           , TAG_TYPE_RATIONAL  , 3 , TAG_customfunction_DegreesMinutesSeconds, TAG_custominterpret_DegreesMinutesSeconds, 0 },
 { TAG_GPSLongitudeRef      , "GPSLongitudeRef"       , TAG_TYPE_ASCII     , 2 , NULL, NULL, 0 },
 { TAG_GPSLongitude         , "GPSLongitude"          , TAG_TYPE_RATIONAL  , 3 , TAG_customfunction_DegreesMinutesSeconds, TAG_custominterpret_DegreesMinutesSeconds, 0 },
 { TAG_GPSAltitudeRef       , "GPSAltitudeRef"        , TAG_TYPE_BYTE      , 1 , TAG_customfunction_AltitudeRef, NULL, 0 },
 { TAG_GPSAltitude          , "GPSAltitude"           , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_GPSTimeStamp         , "GPSTimeStamp"          , TAG_TYPE_RATIONAL  , 3 , TAG_customfunction_HourMinuteSecond, TAG_custominterpret_HourMinuteSecond, 0 },
 { TAG_GPSSatellites        , "GPSSatellites"         , TAG_TYPE_ASCII     , 0 , NULL, NULL, 0 },
 { TAG_GPSStatus            , "GPSStatus"             , TAG_TYPE_ASCII     , 2 , NULL, NULL, 0 },
 { TAG_GPSMeasureMode       , "GPSMeasureMode"        , TAG_TYPE_ASCII     , 2 , NULL, NULL, 0 },
 { TAG_GPSDOP               , "GPSDOP"                , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_GPSSpeedRef          , "GPSSpeedRef"           , TAG_TYPE_ASCII     , 2 , NULL, NULL, 0 },
 { TAG_GPSSpeed             , "GPSSpeed"              , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_GPSTrackRef          , "GPSTrackRef"           , TAG_TYPE_ASCII     , 2 , NULL, NULL, 0 },
 { TAG_GPSTrack             , "GPSTrack"              , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_GPSImgDirectionRef   , "GPSImgDirectionalRef"  , TAG_TYPE_ASCII     , 2 , NULL, NULL, 0 },
 { TAG_GPSImgDirection      , "GPSImgDirection"       , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_GPSMapDatum          , "GPSMapDatum"           , TAG_TYPE_ASCII     , 0 , NULL, NULL, 0 },
 { TAG_GPSDestLatitudeRef   , "GPSDestLatitudeRef"    , TAG_TYPE_ASCII     , 2 , NULL, NULL, 0 },
 { TAG_GPSDestLatitude      , "GPSDestLatitude"       , TAG_TYPE_RATIONAL  , 3 , TAG_customfunction_DegreesMinutesSeconds, TAG_custominterpret_DegreesMinutesSeconds, 0 },
 { TAG_GPSDestLongitudeRef  , "GPSDestLongitudeRef"   , TAG_TYPE_ASCII     , 2 , NULL, NULL, 0 },
 { TAG_GPSDestLongitude     , "GPSDestLongitude"      , TAG_TYPE_RATIONAL  , 3 , TAG_customfunction_DegreesMinutesSeconds, TAG_custominterpret_DegreesMinutesSeconds, 0 },
 { TAG_GPSDestBearingRef    , "GPSDestBearingRef"     , TAG_TYPE_ASCII     , 2 , NULL, TAG_custominterpret_StringIsAlwaysAtLeast2Bytes, 0 },
 { TAG_GPSDestBearing       , "GPSDestBearing"        , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_GPSDestDistanceRef   , "GPSDestDistanceRef"    , TAG_TYPE_ASCII     , 2 , NULL, TAG_custominterpret_StringIsAlwaysAtLeast2Bytes, 0 },
 { TAG_GPSDestDistance      , "GPSDestDistance"       , TAG_TYPE_RATIONAL  , 1 , NULL, NULL, 0 },
 { TAG_GPSProcessingMethod  , "GPSProcessingMethod"   , TAG_TYPE_UNDEFINED , 0 , NULL, NULL, 0 }, // TODO
 { TAG_GPSAreaInformation   , "GPSAreaInformation"    , TAG_TYPE_UNDEFINED , 0 , NULL, NULL, 0 }, // TODO
 { TAG_GPSDateStamp         , "GPSDateStamp"          , TAG_TYPE_ASCII     , 11, NULL, NULL, 0 }, 
 { TAG_GPSDifferential      , "GPSDifferential"       , TAG_TYPE_SHORT     , 1,  NULL, NULL, 0 }, // TODO?
 { 0xFFFF                   , NULL                    , 0                  , 0 , NULL, NULL, 0 }, // ending record
};

custom_tag_t Interoperability_tags[] = {
 // tags relating to gps:
 { TAG_InteroperabilityIndex, "InteroperabilityIndex" , TAG_TYPE_ASCII     , 0 , NULL, NULL, 0 },
 { TAG_InteroperabilityVersion, "InteroperabilityVersion", TAG_TYPE_UNDEFINED , 4, TAG_customfunction_UndefinedVersion, TAG_custominterpret_UndefinedVersion, 0 },
 { 0xFFFF                   , NULL                    , 0                  , 0 , NULL, NULL, 0 }, // ending record
};

CUSTOMFUNCTION(UndefinedVersion) {
    char *temp = malloc(bytespertag + 1);
    memcpy(temp, data + absolute_pos, bytespertag);
    temp[bytespertag] = 0;
    mxmlElementSetAttrf(tag, "value", "%s", temp);
    free(temp);
}

CUSTOMINTERPRET(UndefinedVersion) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, strlen(val_attr), le);
    return make_dobj_from_buffer(strdup(val_attr), strlen(val_attr), pointer, full_name, dobj, le);
}

CUSTOMFUNCTION(Compression) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    if (i==1) mxmlElementSetAttrf(tag, "value", "uncompressed");
    else if (i==6) mxmlElementSetAttrf(tag, "value", "JPEG compression");
    else mxmlElementSetAttrf(tag, "value", "%u", i);
}

CUSTOMINTERPRET(Compression) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if (0==strcmp(val_attr, "uncompressed"))          return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "JPEG compression"))    return make_short_obj(6, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(PhotometricInterpretation) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    if (i==2) mxmlElementSetAttrf(tag, "value", "RGB");
    else if (i==6) mxmlElementSetAttrf(tag, "value", "YCbCr");
    else mxmlElementSetAttrf(tag, "value", "%u", i);
}

CUSTOMINTERPRET(PhotometricInterpretation) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "RGB"))   return make_short_obj(2, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "YCbCr")) return make_short_obj(6, pointer, full_name, dobj, le);
    return 0;
}
CUSTOMINTERPRET(StringIsAlwaysAtLeast2Bytes) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 2, le);
    if (val_attr[0] == 0) {
        // if empty string:
        return make_short_obj(0, pointer, full_name, dobj, le);
    }
    return 0;
}

CUSTOMFUNCTION(Orientation) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case 1: mxmlElementSetAttrf(tag, "value", "top-left"); break;
        case 2: mxmlElementSetAttrf(tag, "value", "top-right"); break;
        case 3: mxmlElementSetAttrf(tag, "value", "bottom-left"); break;
        case 4: mxmlElementSetAttrf(tag, "value", "bottom-right"); break;
        case 5: mxmlElementSetAttrf(tag, "value", "left-top"); break;
        case 6: mxmlElementSetAttrf(tag, "value", "right-top"); break;
        case 7: mxmlElementSetAttrf(tag, "value", "right-bottom"); break;
        case 8: mxmlElementSetAttrf(tag, "value", "left-bottom"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
    mxmlElementSetAttrf(tag, "refers_to", "row0-col0");
}

CUSTOMINTERPRET(Orientation) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "top-left"))     return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "top-right"))    return make_short_obj(2, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "bottom-left"))  return make_short_obj(3, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "bottom-right")) return make_short_obj(4, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "left-top"))     return make_short_obj(5, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "right-top"))    return make_short_obj(6, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "right-bottom")) return make_short_obj(7, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "left-bottom"))  return make_short_obj(8, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(PlanarConfiguration) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    if (i==1) mxmlElementSetAttrf(tag, "value", "chunky format");
    else if (i==2) mxmlElementSetAttrf(tag, "value", "planar format");
    else mxmlElementSetAttrf(tag, "value", "%u", i);
}

CUSTOMINTERPRET(PlanarConfiguration) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "chunky format"))    return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "planar format"))    return make_short_obj(2, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(YCbCrSubsampling) {
    uint16_t i1 = i2xm_read_uint16(data, absolute_pos, le);
    uint16_t i2 = i2xm_read_uint16(data, absolute_pos + 2, le);
    if (i1==2 && i2==1) mxmlElementSetAttrf(tag, "value", "YCbCr4:2:2");
    else if (i1==2 && i2==2) mxmlElementSetAttrf(tag, "value", "YCbCr4:2:0");
    else mxmlElementSetAttrf(tag, "value", "[%u, %u]", i1, i2);
}

CUSTOMINTERPRET(YCbCrSubsampling) {
    unsigned int i1, i2;
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 2, le);
    if      (0==strcmp(val_attr, "YCbCr4:2:2"))    return make_2_short_obj(2, 1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "YCbCr4:2:0"))    return make_2_short_obj(2, 2, pointer, full_name, dobj, le);
    else if (2==sscanf(val_attr, "[%u, %u]", &i1, &i2)) {
        return make_2_short_obj(i1, i2, pointer, full_name, dobj, le);
    }
    return 0;
}

CUSTOMFUNCTION(YCbCrPositioning) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    if (i==1) mxmlElementSetAttrf(tag, "value", "centered");
    else if (i==2) mxmlElementSetAttrf(tag, "value", "co-sited");
    else mxmlElementSetAttrf(tag, "value", "%u", i);
}

CUSTOMINTERPRET(YCbCrPositioning) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if (0==strcmp(val_attr, "centered"))          return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "co-sited"))    return make_short_obj(2, pointer, full_name, dobj, le);
    return 0;
}


CUSTOMFUNCTION(ResolutionUnit) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    if (i==2) mxmlElementSetAttrf(tag, "value", "inches");
    else if (i==3) mxmlElementSetAttrf(tag, "value", "centimeters");
    else mxmlElementSetAttrf(tag, "value", "%u", i);
}

CUSTOMINTERPRET(ResolutionUnit) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "inches"))      return make_short_obj(2, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "centimeters")) return make_short_obj(3, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(Exif_IFD_Pointer) {
    i2xm_read_ifds(data, endian_pos, size, absolute_pos - endian_pos, tag, global_tags, dp, full_name, images, 1, le);
}

CUSTOMINTERPRET(Exif_IFD_Pointer) {
    write_uint32(count, 1, le);
    interpret_ifd_chain(node, pointer, full_name, objdb, global_tags, endian_pos, le);
    *dobj = NULL;
    return 1;
}

CUSTOMFUNCTION(Interoperability_IFD_Pointer) {
    i2xm_read_ifds(data, endian_pos, size, absolute_pos - endian_pos, tag, Interoperability_tags, dp, full_name, images, 1, le);
}

CUSTOMINTERPRET(Interoperability_IFD_Pointer) {
    write_uint32(count, 1, le);
    interpret_ifd_chain(node, pointer, full_name, objdb, Interoperability_tags, endian_pos, le);
    *dobj = NULL;
    return 1;
}

CUSTOMFUNCTION(GPS_Info_IFD_Pointer) {
    i2xm_read_ifds(data, endian_pos, size, absolute_pos - endian_pos, tag, GPS_tags, dp, full_name, images, 1, le);
}

CUSTOMINTERPRET(GPS_Info_IFD_Pointer) {
    write_uint32(count, 1, le);
    interpret_ifd_chain(node, pointer, full_name, objdb, GPS_tags, endian_pos, le);
    *dobj = NULL;
    return 1;
}

CUSTOMFUNCTION(ColorSpace) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    if (i==1) mxmlElementSetAttrf(tag, "value", "sRGB");
    else if (i==0xffff) mxmlElementSetAttrf(tag, "value", "Uncalibrated");
    else mxmlElementSetAttrf(tag, "value", "%u", i);
}

CUSTOMINTERPRET(ColorSpace) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if (0==strcmp(val_attr, "sRGB")) return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Uncalibrated")) return make_short_obj(0xffff, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(MakerNote) {
    i2xm_read_binary(data, absolute_pos, bytespertag, tag);
}

CUSTOMFUNCTION(ComponentsConfiguration) {
    char res[4*4 + 1];
    char *resp = res;
    int i;
    for (i=0;i<4;i++) {
        switch(data[absolute_pos+i]) {
            case 0: resp += sprintf(resp, " "); break;
            case 1: resp += sprintf(resp, "Y"); break;
            case 2: resp += sprintf(resp, "Cb"); break;
            case 3: resp += sprintf(resp, "Cr"); break;
            case 4: resp += sprintf(resp, "R"); break;
            case 5: resp += sprintf(resp, "G"); break;
            case 6: resp += sprintf(resp, "B"); break;
            default: resp += sprintf(resp, "%u", data[absolute_pos+i]); break;
        }
        if (i!=3) resp += sprintf(resp, ",");
    }
    mxmlElementSetAttrf(tag, "value", "%s", res);
}

CUSTOMINTERPRET(ComponentsConfiguration) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    int i; char *data = (char*)malloc(4 * sizeof(char));
    for (i=0;i<4;i++) {
        if (val_attr[0] == 0) { printf("ERROR: there must be 4 color components!\n"); return 0; }
        if      (val_attr[0] == 'Y' && (val_attr[1]==',' || val_attr[1]==0)) { data[i] = 1; val_attr += 2; }
        else if (val_attr[0] == 'C' && val_attr[1]=='b' && (val_attr[2]==',' || val_attr[2]==0)) { data[i] = 2; val_attr += 3; }
        else if (val_attr[0] == 'C' && val_attr[1]=='r' && (val_attr[2]==',' || val_attr[2]==0)) { data[i] = 3; val_attr += 3; }
        else if (val_attr[0] == 'R' && (val_attr[1]==',' || val_attr[1]==0)) { data[i] = 4; val_attr += 2; }
        else if (val_attr[0] == 'G' && (val_attr[1]==',' || val_attr[1]==0)) { data[i] = 5; val_attr += 2; }
        else if (val_attr[0] == 'B' && (val_attr[1]==',' || val_attr[1]==0)) { data[i] = 6; val_attr += 2; }
        else if (val_attr[0] == ' ' && (val_attr[1]==',' || val_attr[1]==0)) { data[i] = 0; val_attr += 2; }
        else if ((val_attr[0]==',' || val_attr[0]==0)) { data[i] = 0; val_attr += 1; }
        else {
            unsigned int t;
            if (1==sscanf(val_attr, "%u", &t)) {
                while (val_attr[0]!=0 && val_attr[0]!=',') val_attr++;
                if (val_attr[0]==',') val_attr++;
                if (t>255) { printf("ERROR color component value too big in ComponentsConfiguration\n"); return 0; }
                data[i] = t;
            } else {
                printf("ERROR: unable to recognize color component in ComponentsConfiguration (%s)\n", val_attr);
                return 0;
            }
        }
    }
    write_uint32(count, 4, le);
    return make_dobj_from_buffer(data, 4*sizeof(char), pointer, full_name, dobj, le);
}

CUSTOMFUNCTION(ExposureProgram) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case 0: mxmlElementSetAttrf(tag, "value", "Not defined"); break;
        case 1: mxmlElementSetAttrf(tag, "value", "Manual"); break;
        case 2: mxmlElementSetAttrf(tag, "value", "Normal program"); break;
        case 3: mxmlElementSetAttrf(tag, "value", "Aperture priority"); break;
        case 4: mxmlElementSetAttrf(tag, "value", "Shutter priority"); break;
        case 5: mxmlElementSetAttrf(tag, "value", "Creative program"); break;
        case 6: mxmlElementSetAttrf(tag, "value", "Action program"); break;
        case 7: mxmlElementSetAttrf(tag, "value", "Portrait mode"); break;
        case 8: mxmlElementSetAttrf(tag, "value", "Landscape mode"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
}

CUSTOMINTERPRET(ExposureProgram) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "Not defined"))       return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Manual"))            return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Normal program"))    return make_short_obj(2, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Aperture priority")) return make_short_obj(3, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Shutter priority"))  return make_short_obj(4, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Creative program"))  return make_short_obj(5, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Action program"))    return make_short_obj(6, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Portrait mode"))     return make_short_obj(7, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Landscape mode"))    return make_short_obj(8, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(MeteringMode) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case 0: mxmlElementSetAttrf(tag, "value", "unknown"); break;
        case 1: mxmlElementSetAttrf(tag, "value", "Average"); break;
        case 2: mxmlElementSetAttrf(tag, "value", "CenterWeightedAverage"); break;
        case 3: mxmlElementSetAttrf(tag, "value", "Spot"); break;
        case 4: mxmlElementSetAttrf(tag, "value", "MultiSpot"); break;
        case 5: mxmlElementSetAttrf(tag, "value", "Pattern"); break;
        case 6: mxmlElementSetAttrf(tag, "value", "Partial"); break;
        case 255: mxmlElementSetAttrf(tag, "value", "other"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
}

CUSTOMINTERPRET(MeteringMode) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "unknown"))   return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Average"))   return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "CenterWeightedAverage")) return make_short_obj(2, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Spot"))      return make_short_obj(3, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "MultiSpot")) return make_short_obj(4, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Pattern"))   return make_short_obj(5, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Partial"))   return make_short_obj(6, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "other"))     return make_short_obj(255, pointer, full_name, dobj, le);
    return 0;
}


CUSTOMFUNCTION(LightSource) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case  0: mxmlElementSetAttrf(tag, "value", "unknown"); break;
        case  1: mxmlElementSetAttrf(tag, "value", "Daylight"); break;
        case  2: mxmlElementSetAttrf(tag, "value", "Fluorescent"); break;
        case  3: mxmlElementSetAttrf(tag, "value", "Tungsten"); break;
        case  4: mxmlElementSetAttrf(tag, "value", "Flash"); break;
        
        case  9: mxmlElementSetAttrf(tag, "value", "Fine weather"); break;
        case 10: mxmlElementSetAttrf(tag, "value", "Cloudy weather"); break;
        case 11: mxmlElementSetAttrf(tag, "value", "Shade"); break;
        case 12: mxmlElementSetAttrf(tag, "value", "Daylight fluorescent"); break;
        case 13: mxmlElementSetAttrf(tag, "value", "Day white fluorescent"); break;
        case 14: mxmlElementSetAttrf(tag, "value", "Cool white fluorescent"); break;
        case 15: mxmlElementSetAttrf(tag, "value", "White fluorescent"); break;
        
        case 17: mxmlElementSetAttrf(tag, "value", "Standard light A"); break;
        case 18: mxmlElementSetAttrf(tag, "value", "Standard light B"); break;
        case 19: mxmlElementSetAttrf(tag, "value", "Standard light C"); break;
        case 20: mxmlElementSetAttrf(tag, "value", "D55"); break;
        case 21: mxmlElementSetAttrf(tag, "value", "D65"); break;
        case 22: mxmlElementSetAttrf(tag, "value", "D75"); break;
        case 23: mxmlElementSetAttrf(tag, "value", "D50"); break;
        case 24: mxmlElementSetAttrf(tag, "value", "ISO studio tungsten"); break;
        
        case 255: mxmlElementSetAttrf(tag, "value", "other light source"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
}

CUSTOMINTERPRET(LightSource) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "unknown")) return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Daylight")) return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Fluorescent")) return make_short_obj(2, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Tungsten")) return make_short_obj(3, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Flash")) return make_short_obj(4, pointer, full_name, dobj, le);
    
    else if (0==strcmp(val_attr, "Fine weather")) return make_short_obj(9, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Cloudy weather")) return make_short_obj(10, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Shade")) return make_short_obj(11, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Daylight fluorescent")) return make_short_obj(12, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Day white fluorescent")) return make_short_obj(13, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Cool white fluorescent")) return make_short_obj(14, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "White fluorescent")) return make_short_obj(15, pointer, full_name, dobj, le);
    
    else if (0==strcmp(val_attr, "Standard light A")) return make_short_obj(17, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Standard light B")) return make_short_obj(18, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Standard light C")) return make_short_obj(19, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "D55")) return make_short_obj(20, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "D65")) return make_short_obj(21, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "D75")) return make_short_obj(22, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "D50")) return make_short_obj(23, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "ISO studio tungsten")) return make_short_obj(24, pointer, full_name, dobj, le);
    
    else if (0==strcmp(val_attr, "other light source")) return make_short_obj(255, pointer, full_name, dobj, le);
    return 0;
}


CUSTOMFUNCTION(Flash) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    mxmlElementSetAttrf(mxmlNewElement(tag, "flash_fired"), "value", "%d", i & 1);
    switch ((i>>1)&3) {
        case 0: mxmlElementSetAttrf(mxmlNewElement(tag, "strobe_return"), "value", "no function"); break;
        case 1: mxmlElementSetAttrf(mxmlNewElement(tag, "strobe_return"), "value", "reserved"); break;
        case 2: mxmlElementSetAttrf(mxmlNewElement(tag, "strobe_return"), "value", "not detected"); break;
        case 3: mxmlElementSetAttrf(mxmlNewElement(tag, "strobe_return"), "value", "detected"); break;
    }
    switch ((i>>3)&3) {
        case 0: mxmlElementSetAttrf(mxmlNewElement(tag, "flash_mode"), "value", "unknown"); break;
        case 1: mxmlElementSetAttrf(mxmlNewElement(tag, "flash_mode"), "value", "compulsory firing"); break;
        case 2: mxmlElementSetAttrf(mxmlNewElement(tag, "flash_mode"), "value", "compulsory suppression"); break;
        case 3: mxmlElementSetAttrf(mxmlNewElement(tag, "flash_mode"), "value", "auto mode"); break;
    }
    mxmlElementSetAttrf(mxmlNewElement(tag, "flash_function_present"), "value", "%d", (i>>5) & 1);
    mxmlElementSetAttrf(mxmlNewElement(tag, "red_eye_mode"), "value", "%d", (i>>6) & 1);
}

CUSTOMINTERPRET(Flash) {
    write_uint32(count, 1, le);
    uint16_t v = 0;
    FOR_CHILDREN(child, node) {
        if (IS_ELEMENT(child, "flash_fired")) {
            const char *val_attr = mxmlElementGetAttr(child, "value");
            if (val_attr==NULL) {
                printf("ERROR: in the 'Flash' tag the flash_fired element must have a value attribute!\n");
                return 0;
            }
            if (val_attr[0]=='1' && val_attr[1] == 0) v = v | 1; 
        } else if (IS_ELEMENT(child, "strobe_return")) {
            const char *val_attr = mxmlElementGetAttr(child, "value");
            if (val_attr==NULL) {
                printf("ERROR: in the 'Flash' tag the strobe_return element must have a value attribute!\n");
                return 0;
            }
            if      (0==strcmp(val_attr, "no function"))  v |= 0 << 1;
            else if (0==strcmp(val_attr, "reserved"))     v |= 1 << 1;
            else if (0==strcmp(val_attr, "not detected")) v |= 2 << 1;
            else if (0==strcmp(val_attr, "detected"))     v |= 3 << 1;
            else {
                printf("ERROR: unknown value: %s in strobe_return\n", val_attr);
                return 0;
            }
        } else if (IS_ELEMENT(child, "flash_mode")) {
            const char *val_attr = mxmlElementGetAttr(child, "value");
            if (val_attr==NULL) {
                printf("ERROR: in the 'Flash' tag the flash_mode element must have a value attribute!\n");
                return 0;
            }
            if      (0==strcmp(val_attr, "unknown"))                v |= 0 << 3;
            else if (0==strcmp(val_attr, "compulsory firing"))      v |= 1 << 3;
            else if (0==strcmp(val_attr, "compulsory suppression")) v |= 2 << 3;
            else if (0==strcmp(val_attr, "auto mode"))              v |= 3 << 3;
            else {
                printf("ERROR: unknown value: %s in flash_mode\n", val_attr);
                return 0;
            }
        } else if (IS_ELEMENT(child, "flash_function_present")) {
            const char *val_attr = mxmlElementGetAttr(child, "value");
            if (val_attr==NULL) {
                printf("ERROR: in the 'Flash' tag the flash_function_present element must have a value attribute!\n");
                return 0;
            }
            if (val_attr[0]=='1' && val_attr[1] == 0) v |= 1<<5; 
        } else if (IS_ELEMENT(child, "red_eye_mode")) {
            const char *val_attr = mxmlElementGetAttr(child, "value");
            if (val_attr==NULL) {
                printf("ERROR: in the 'Flash' tag the red_eye_mode element must have a value attribute!\n");
                return 0;
            }
            if (val_attr[0]=='1' && val_attr[1] == 0) v |= 1<<6; 
        } else {
            printf("ERROR: unexpected stuff inside a 'Flash' tag\n");
            return 0;
        }
    }
    return make_short_obj(v, pointer, full_name, dobj, le);
}

CUSTOMFUNCTION(SubjectArea) {
    if (count<2 || count>4) {
        i2xm_read_array_value(data, &absolute_pos, count, tag, type, le);
    } else {
        mxmlElementSetAttrf(mxmlNewElement(tag, "x"), "value", "%u", i2xm_read_uint16(data, absolute_pos, le));
        mxmlElementSetAttrf(mxmlNewElement(tag, "y"), "value", "%u", i2xm_read_uint16(data, absolute_pos + sizeof(uint16_t), le));
        if (count==3) {
            mxmlElementSetAttrf(mxmlNewElement(tag, "diameter"), "value", "%u", i2xm_read_uint16(data, absolute_pos + 2 * sizeof(uint16_t), le));
        } else {
            mxmlElementSetAttrf(mxmlNewElement(tag, "width"), "value", "%u", i2xm_read_uint16(data, absolute_pos + 2 * sizeof(uint16_t), le));
            mxmlElementSetAttrf(mxmlNewElement(tag, "height"), "value", "%u", i2xm_read_uint16(data, absolute_pos + 3 * sizeof(uint16_t), le));
        }
    }
}

CUSTOMINTERPRET(SubjectArea) {
    uint16_t v1=0, v2=0, v3=0, v4=0, count_nr=2;
    FOR_CHILDREN(child, node) {
        if (child->type!=MXML_ELEMENT) { printf("ERROR: unexpected non-element in SubjectArea tag!\n"); return 0; }
        const char *val_attr = mxmlElementGetAttr(child, "value");
        if (val_attr == NULL) {
            printf("ERROR: Element %s must have a value attribute!\n", child->value.element.name);
            return 0;
        }
        unsigned int t;
        if (1!=sscanf(val_attr, "%u", &t)) { printf("ERROR: Element %s should contain a decimal value!\n", child->value.element.name); return 0; }
        if (t>>16) { printf("ERROR: value %s of element %s is out of bounds!\n", val_attr, child->value.element.name); return 0; }
        if (IS_ELEMENT(child, "x")) {
            v1 = t;
        } else if (IS_ELEMENT(child, "y")) {
            v2 = t;
        } else if (IS_ELEMENT(child, "diameter")) {
            count_nr = 3;
            v3 = t;
        } else if (IS_ELEMENT(child, "width")) {
            if (count_nr<3) count_nr = 3;
            v3 = t;
        } else if (IS_ELEMENT(child, "height")) {
            count_nr = 4;
            v4 = t;
        } else { printf("ERROR: unexpected stuff inside the 'SubjectArea tag\n"); return 0; }
    }
    write_uint32(count, count_nr, le);
    if (count_nr==2) return make_2_short_obj(v1, v2, pointer, full_name, dobj, le);
    else if (count_nr==3) return make_3_short_obj(v1, v2, v3, pointer, full_name, dobj, le);
    else if (count_nr==4) return make_4_short_obj(v1, v2, v3, v4, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(SensingMethod) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case 1: mxmlElementSetAttrf(tag, "value", "Not defined"); break;
        case 2: mxmlElementSetAttrf(tag, "value", "One-chip color area sensor"); break;
        case 3: mxmlElementSetAttrf(tag, "value", "Two-chip color area sensor"); break;
        case 4: mxmlElementSetAttrf(tag, "value", "Three-chip color area sensor"); break;
        case 5: mxmlElementSetAttrf(tag, "value", "Color sequential area sensor"); break;

        case 7: mxmlElementSetAttrf(tag, "value", "Trilinear sensor"); break;
        case 8: mxmlElementSetAttrf(tag, "value", "Color sequential linear sensor"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
}

CUSTOMINTERPRET(SensingMethod) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "Not defined")) return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "One-chip color area sensor")) return make_short_obj(2, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Two-chip color area sensor")) return make_short_obj(3, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Three-chip color area sensor")) return make_short_obj(4, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Color sequential area sensor")) return make_short_obj(5, pointer, full_name, dobj, le);

    else if (0==strcmp(val_attr, "Trilinear sensor")) return make_short_obj(7, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Color sequential linear sensor")) return make_short_obj(8, pointer, full_name, dobj, le);

    return 0;
}


CUSTOMFUNCTION(FileSource) {
    if (data[absolute_pos]==3) mxmlElementSetAttrf(tag, "value", "DSC");
    else mxmlElementSetAttrf(tag, "value", "%d", data[absolute_pos]);
}

CUSTOMINTERPRET(FileSource) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if (0==strcmp(val_attr, "DSC")) {
        char *data = malloc(sizeof(char));
        data[0] = 3;
        return make_dobj_from_buffer(data, 1*sizeof(char), pointer, full_name, dobj, le);
    }
    return 0;
}

CUSTOMFUNCTION(SceneType) {
    if (data[absolute_pos]==1) mxmlElementSetAttrf(tag, "value", "A directly photographed image");
    else mxmlElementSetAttrf(tag, "value", "%d", data[absolute_pos]);
}

CUSTOMINTERPRET(SceneType) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if (0==strcmp(val_attr, "A directly photographed image")) {
        char *data = malloc(sizeof(char));
        data[0] = 1;
        return make_dobj_from_buffer(data, 1*sizeof(char), pointer, full_name, dobj, le);
    }
    return 0;
}

CUSTOMFUNCTION(CustomRendered) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    if (i==0) mxmlElementSetAttrf(tag, "value", "Normal process");
    else if (i==1) mxmlElementSetAttrf(tag, "value", "Custom process");
    else mxmlElementSetAttrf(tag, "value", "%u", i);
}

CUSTOMINTERPRET(CustomRendered) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "Normal process")) return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Custom process")) return make_short_obj(1, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(ExposureMode) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    if (i==0) mxmlElementSetAttrf(tag, "value", "Auto Exposure");
    else if (i==1) mxmlElementSetAttrf(tag, "value", "Manual Exposure");
    else if (i==2) mxmlElementSetAttrf(tag, "value", "Auto bracket");
    else mxmlElementSetAttrf(tag, "value", "%u", i);
}

CUSTOMINTERPRET(ExposureMode) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "Auto Exposure")) return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Manual Exposure")) return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Auto bracket")) return make_short_obj(2, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(WhiteBalance) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    if (i==0) mxmlElementSetAttrf(tag, "value", "Auto white balance");
    else if (i==1) mxmlElementSetAttrf(tag, "value", "Manual white balance");
    else mxmlElementSetAttrf(tag, "value", "%u", i);
}

CUSTOMINTERPRET(WhiteBalance) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "Auto white balance")) return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Manual white balance")) return make_short_obj(1, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(SceneCaptureType) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case 0: mxmlElementSetAttrf(tag, "value", "Standard"); break;
        case 1: mxmlElementSetAttrf(tag, "value", "Landscape"); break;
        case 2: mxmlElementSetAttrf(tag, "value", "Portrait"); break;
        case 3: mxmlElementSetAttrf(tag, "value", "Night scene"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
}

CUSTOMINTERPRET(SceneCaptureType) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "Standard"))    return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Landscape"))   return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Portrait"))    return make_short_obj(2, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Night scene")) return make_short_obj(3, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(GainControl) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case 0: mxmlElementSetAttrf(tag, "value", "None"); break;
        case 1: mxmlElementSetAttrf(tag, "value", "Low gain up"); break;
        case 2: mxmlElementSetAttrf(tag, "value", "High gain up"); break;
        case 3: mxmlElementSetAttrf(tag, "value", "Low gain down"); break;
        case 4: mxmlElementSetAttrf(tag, "value", "High gain down"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
}

CUSTOMINTERPRET(GainControl) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "None"))       return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Low gain up")) return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "High gain up")) return make_short_obj(2, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Low gain down")) return make_short_obj(3, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "High gain down")) return make_short_obj(4, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(Contrast) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case 0: mxmlElementSetAttrf(tag, "value", "Normal"); break;
        case 1: mxmlElementSetAttrf(tag, "value", "Soft"); break;
        case 2: mxmlElementSetAttrf(tag, "value", "Hard"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
}

CUSTOMINTERPRET(Contrast) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "Normal")) return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Soft"))   return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Hard"))   return make_short_obj(2, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(Saturation) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case 0: mxmlElementSetAttrf(tag, "value", "Normal"); break;
        case 1: mxmlElementSetAttrf(tag, "value", "Low"); break;
        case 2: mxmlElementSetAttrf(tag, "value", "High"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
}

CUSTOMINTERPRET(Saturation) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "Normal")) return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Low"))    return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "High"))   return make_short_obj(2, pointer, full_name, dobj, le);
    return 0;
}


CUSTOMFUNCTION(Sharpness) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case 0: mxmlElementSetAttrf(tag, "value", "Normal"); break;
        case 1: mxmlElementSetAttrf(tag, "value", "Soft"); break;
        case 2: mxmlElementSetAttrf(tag, "value", "Hard"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
}

CUSTOMINTERPRET(Sharpness) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "Normal")) return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Soft"))   return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Hard"))   return make_short_obj(2, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(SubjectDistanceRange) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case 0: mxmlElementSetAttrf(tag, "value", "unknown"); break;
        case 1: mxmlElementSetAttrf(tag, "value", "Macro"); break;
        case 2: mxmlElementSetAttrf(tag, "value", "Close view"); break;
        case 3: mxmlElementSetAttrf(tag, "value", "Distant view"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
}

CUSTOMINTERPRET(SubjectDistanceRange) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "unknown")) return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Macro"))    return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Close view")) return make_short_obj(2, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Distant view")) return make_short_obj(3, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(SensitivityType) {
    uint16_t i = i2xm_read_uint16(data, absolute_pos, le);
    switch (i) {
        case 0: mxmlElementSetAttrf(tag, "value", "Unknown"); break;
        case 1: mxmlElementSetAttrf(tag, "value", "Standard Output Sensitivity"); break;
        case 2: mxmlElementSetAttrf(tag, "value", "Recommended Exposure Index"); break;
        case 3: mxmlElementSetAttrf(tag, "value", "ISO Speed"); break;
        case 4: mxmlElementSetAttrf(tag, "value", "Standard Output Sensisivity and Recommended Exposure Index"); break;
        case 5: mxmlElementSetAttrf(tag, "value", "Standard Output Sensitivity and ISO Speed"); break;
        case 6: mxmlElementSetAttrf(tag, "value", "Recommended Exposure Index and ISO Speed"); break;
        case 7: mxmlElementSetAttrf(tag, "value", "Standard Output Sensitivity, Recommended Exposure Index and ISO Speed"); break;
        default:
            mxmlElementSetAttrf(tag, "value", "%u", i);
    }
}

CUSTOMINTERPRET(SensitivityType) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if      (0==strcmp(val_attr, "Unknown"))     return make_short_obj(0, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Standard Output Sensitivity"))    return make_short_obj(1, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Recommended Exposure Index"))  return make_short_obj(2, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "ISO Speed")) return make_short_obj(3, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Standard Output Sensisivity and Recommended Exposure Index"))     return make_short_obj(4, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Standard Output Sensitivity and ISO Speed"))    return make_short_obj(5, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Recommended Exposure Index and ISO Speed")) return make_short_obj(6, pointer, full_name, dobj, le);
    else if (0==strcmp(val_attr, "Standard Output Sensitivity, Recommended Exposure Index and ISO Speed"))  return make_short_obj(7, pointer, full_name, dobj, le);
    return 0;
}

CUSTOMFUNCTION(AltitudeRef) {
    if (data[0]==0) mxmlElementSetAttrf(tag, "value", "Sea level");
    else if (data[0]==1) mxmlElementSetAttrf(tag, "value", "Sea level reference (negative value)");
    else mxmlElementSetAttrf(tag, "value", "%u", data[0]);
}

CUSTOMINTERPRET(AltitudeRef) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    write_uint32(count, 1, le);
    if (0==strcmp(val_attr, "Sea level")) {
        char *data = malloc(sizeof(char));
        data[0] = 0;
        return make_dobj_from_buffer(data, 1*sizeof(char), pointer, full_name, dobj, le);
    } else if (0==strcmp(val_attr, "Sea level reference (negative value)")) {
        char *data = malloc(sizeof(char));
        data[0] = 1;
        return make_dobj_from_buffer(data, 1*sizeof(char), pointer, full_name, dobj, le);
    }
    return 0;
}

CUSTOMFUNCTION(DotSeparatedBytes) {
    int i;
    char *s = malloc(4 * count + 1);
    char *sp = s;
    for (i=0;i<count;i++) {
        if (i!=0) sp += sprintf(sp, ".");
        sp += sprintf(sp, "%u" , data[absolute_pos + i]);
    }
    mxmlElementSetAttrf(tag, "value" , "%s", s);
    free(s);
}

CUSTOMINTERPRET(DotSeparatedBytes) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (val_attr==NULL) return 0;
    int i; char *data = (char*)malloc(4 * sizeof(char));
    for (i=0;i<4;i++) {
        if (val_attr[0] == 0) { printf("ERROR: there must be 4 bytes in tag %s!\n", node->value.element.name); return 0; }
        unsigned int t;
        if (1==sscanf(val_attr, "%u", &t)) {
            while (val_attr[0]!=0 && val_attr[0]!='.') val_attr++;
            if (val_attr[0]=='.') val_attr++;
            if (t>255) { printf("ERROR byte too big in tag %s\n", node->value.element.name); return 0; }
            data[i] = t;
        } else {
            printf("ERROR: unable to recognize byte '%s' in tag '%s'\n", val_attr, node->value.element.name);
            return 0;
        }
    }
    write_uint32(count, 4, le);
    return make_dobj_from_buffer(data, 4*sizeof(char), pointer, full_name, dobj, le);
}

CUSTOMFUNCTION(DegreesMinutesSeconds) {
    i2xm_read_single_value(data, &absolute_pos, mxmlNewElement(tag, "degrees"), type, le);
    i2xm_read_single_value(data, &absolute_pos, mxmlNewElement(tag, "minutes"), type, le);
    i2xm_read_single_value(data, &absolute_pos, mxmlNewElement(tag, "seconds"), type, le);
}

CUSTOMINTERPRET(DegreesMinutesSeconds) {
    const char *deg="";
    const char *min="";
    const char *sec="";
    uint32_t c;
    writer_t wr;
    init_memory_writer(&wr, 8 * 3, 8 * 3);
    FOR_CHILDREN(child, node) {
        if (child->type!=MXML_ELEMENT) { printf("ERROR: unexpected non-element in an hour-minute-second tag (%s)!\n", node->value.element.name); return 0; }
        const char *val_attr = mxmlElementGetAttr(child, "value");
        if (val_attr == NULL) {
            printf("ERROR: Element %s must have a value attribute!\n", child->value.element.name);
            return 0;
        }
        if (IS_ELEMENT(child, "degrees")) {
            deg = val_attr;
        } else if (IS_ELEMENT(child, "minutes")) {
            min = val_attr;
        } else if (IS_ELEMENT(child, "seconds")) {
            sec = val_attr;
        } else { printf("ERROR: unexpected stuff inside the '%s' tag\n", node->value.element.name); return 0; }
    }
    write_uint32(count, 3, le);
    interpret_single_tag_value(&wr, deg, TAG_TYPE_RATIONAL, &c, le);
    interpret_single_tag_value(&wr, min, TAG_TYPE_RATIONAL, &c, le);
    interpret_single_tag_value(&wr, sec, TAG_TYPE_RATIONAL, &c, le);
    return make_dobj_from_mem_writer(&wr, pointer, full_name, dobj, le);
}

CUSTOMFUNCTION(HourMinuteSecond) {
    i2xm_read_single_value(data, &absolute_pos, mxmlNewElement(tag, "hour"), type, le);
    i2xm_read_single_value(data, &absolute_pos, mxmlNewElement(tag, "minute"), type, le);
    i2xm_read_single_value(data, &absolute_pos, mxmlNewElement(tag, "second"), type, le);
}

CUSTOMINTERPRET(HourMinuteSecond) {
    const char *hou="";
    const char *min="";
    const char *sec="";
    uint32_t c;
    writer_t wr;
    init_memory_writer(&wr, 8 * 3, 8 * 3);
    FOR_CHILDREN(child, node) {
        if (child->type!=MXML_ELEMENT) { printf("ERROR: unexpected non-element in an hour-minute-second tag (%s)!\n", node->value.element.name); return 0; }
        const char *val_attr = mxmlElementGetAttr(child, "value");
        if (val_attr == NULL) {
            printf("ERROR: Element %s must have a value attribute!\n", child->value.element.name);
            return 0;
        }
        if (IS_ELEMENT(child, "hour")) {
            hou = val_attr;
        } else if (IS_ELEMENT(child, "minute")) {
            min = val_attr;
        } else if (IS_ELEMENT(child, "second")) {
            sec = val_attr;
        } else { printf("ERROR: unexpected stuff inside the %s tag\n", node->value.element.name); return 0; }
    }
    write_uint32(count, 3, le);
    interpret_single_tag_value(&wr, hou, TAG_TYPE_RATIONAL, &c, le);
    interpret_single_tag_value(&wr, min, TAG_TYPE_RATIONAL, &c, le);
    interpret_single_tag_value(&wr, sec, TAG_TYPE_RATIONAL, &c, le);
    return make_dobj_from_mem_writer(&wr, pointer, full_name, dobj, le);
}


static uint32_t iformat = 0;
static uint32_t iformatlength = 0;

void getthumbnail(unsigned char *data, size_t endian_pos, mxml_node_t *tag, char *full_name, data_placement_db_t *dp) {
    i2xm_read_binary(data, iformat, iformatlength, tag);
    add_referenced_data(dp, iformat, iformatlength, full_name);
}

static mxml_node_t *get_prev_tag(mxml_node_t *tag, const char *name) {
    mxml_node_t *stag = tag;
    while (stag!=NULL) {
        if (IS_ELEMENT_ATTR(stag, "id", name)) break;
        stag = stag->prev;
    }
    return stag;
}

CUSTOMFUNCTION(InterchangeFormat) {
    mxml_node_t *ltag = get_prev_tag(tag, "JPEGInterchangeFormatLength");
    iformat = absolute_pos;
    if (ltag != NULL) {
        // found the length tag. that means iformatlength global variable should be valid
        getthumbnail(data, endian_pos, tag, full_name, dp);
        iformat = iformatlength = 0;
    }
}

CUSTOMFUNCTION(InterchangeFormatLength) {
    mxml_node_t *itag = get_prev_tag(tag, "JPEGInterchangeFormat");
    iformatlength = i2xm_read_uint32(data, absolute_pos, le);
    if (itag != NULL) {
        // found the index tag
        char *i_full_name = (char *) malloc(strlen(parent_full_name) + strlen("/JPEGInterchangeFormat") + 1);
        sprintf(i_full_name , "%s/JPEGInterchangeFormat", parent_full_name);
        getthumbnail(data, endian_pos, itag, i_full_name, dp);
        iformat = iformatlength = 0;
    }
}

static pointer_t *format_length = NULL; // this is used if the FormatLength tag comes first
static uint32_t format_length_after = 0;

void resetTags() { // really ugly!!
    format_length_after = 0;
    format_length = NULL;
    iformat = iformatlength = 0;
}

CUSTOMINTERPRET(InterchangeFormat) {
    write_uint32(count, 1, le);
    if (node->child != NULL && IS_ELEMENT(node->child, "binary")) {
        writer_t wr;
        init_memory_writer(&wr, 4000, 4000);
        xm2i_write_binary(&wr, node->child);
        if (format_length != NULL) {
            set_pointer(format_length, wr.size, le);
            format_length = NULL;
        } else {
            format_length_after = wr.size;
        }
        return make_dobj_from_mem_writer(&wr, pointer, full_name, dobj, le);
    }
    return 0;
}

CUSTOMINTERPRET(InterchangeFormatLength) {
    write_uint32(count, 1, le);
    int ret = make_long_obj_with_ptr(format_length_after, pointer, full_name, dobj, le);
    format_length_after = 0;
    format_length = (*dobj)->my_pointers[0];
    return ret;
}

CUSTOMFUNCTION(MPEntry) {
    if ((count & 0xf) != 0) WARNING("the count for the MP_Entry field should be divisible by 16\n");
    count = count >> 4;
    int i;
    for (i=0;i<count;i++) {
        mxml_node_t *entry = mxmlNewElement(tag, "entry");
        uint32_t attrs = i2xm_read_uint32(data, absolute_pos + i * 16, le);
        uint32_t individual_image_size = i2xm_read_uint32(data, absolute_pos + i * 16 + 4, le);
        uint32_t individual_image_data_offset = i2xm_read_uint32(data, absolute_pos + i * 16 + 8, le);
        if (individual_image_data_offset!=0) individual_image_data_offset += endian_pos;
        uint16_t dependent_image_1_entry_number = i2xm_read_uint16(data, absolute_pos + i * 16 + 12, le);
        uint16_t dependent_image_2_entry_number = i2xm_read_uint16(data, absolute_pos + i * 16 + 14, le);
        mxml_node_t *attrs_tag = mxmlNewElement(entry, "individual_image_attribute");
        mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "dependent_parent_image_flag"), "value", "%u", (attrs >> 31) & 1);
        mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "dependent_child_image_flag"), "value", "%u", (attrs >> 30) & 1);
        mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "representative_image_flag"), "value", "%u", (attrs >> 29) & 1);
        if ((attrs >> 28) & 1)
            mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "reserved_flag_28"), "value", "%u", (attrs >> 28) & 1);
        if ((attrs >> 27) & 1)
            mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "reserved_flag_27"), "value", "%u", (attrs >> 27) & 1);
        int format = (attrs >> 24) & 0x07;
        if (format == 0)
            mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "image_data_format"), "value", "JPEG");
        else
            mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "image_data_format"), "value", "reserved_%d", format);
        int mptype = attrs & ((1<<24)-1);
        switch (mptype) {
            case 0x030000: mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "mp_type"), "value", "Baseline MP Primary Image"); break;
            case 0x010001: mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "mp_type"), "value", "Large Thumbnail Class 1 (VGA equivalent)"); break;
            case 0x010002: mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "mp_type"), "value", "Large Thumbnail Class 2 (Full HD equivalent)"); break;
            case 0x020001: mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "mp_type"), "value", "Multi-Frame Image - Panorama"); break;
            case 0x020002: mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "mp_type"), "value", "Multi-Frame Image - Disparity"); break;
            case 0x020003: mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "mp_type"), "value", "Multi-Frame Image - Multi-Angle"); break;
            case 0x000000: mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "mp_type"), "value", "Undefined"); break;
            default: mxmlElementSetAttrf(mxmlNewElement(attrs_tag, "mp_type"), "value", "0x%x", mptype); break;
        }
        mxmlElementSetAttrf(mxmlNewElement(entry, "individual_image_size"), "value", "%u", individual_image_size);
        mxmlElementSetAttrf(mxmlNewElement(entry, "individual_image_data_offset"), "value", "%u", individual_image_data_offset);
        add_referenced_image(images, individual_image_data_offset);
        mxmlElementSetAttrf(mxmlNewElement(entry, "dependent_image_1_entry_number"), "value", "%u", dependent_image_1_entry_number);
        mxmlElementSetAttrf(mxmlNewElement(entry, "dependent_image_2_entry_number"), "value", "%u", dependent_image_2_entry_number);
    }
}

CUSTOMINTERPRET(MPEntry) {
    writer_t wr;
    init_memory_writer(&wr, 16 * 2, 16 * 2);
    *dobj = (data_object_t *) malloc(sizeof(data_object_t) + MAX_IMAGES * 2 * sizeof(pointer_t *));
    (*dobj)->full_name = strdup(full_name);
    (*dobj)->written = 0;
    (*dobj)->pointer = pointer;
    (*dobj)->nr_my_pointers = 0;
    (*dobj)->nr_my_offsets = 0;
    char entry_fn[200], t[200];
    int entry_n = 0;
    FOR_CHILDREN(child, node) {
        uint32_t iia = 0, iis = 0, iio = 0, dep1 = 0, dep2 = 0;
        if (IS_ELEMENT(child, "entry")) {
            snprintf(entry_fn, 200, "%s/entry[%d]", full_name, entry_n);
            add_offset((*dobj), strdup(entry_fn), 16 * entry_n);
            
            snprintf(t, 200, "%s/individual_image_attribute", entry_fn);
            add_offset((*dobj), strdup(t), 16 * entry_n);
            
            snprintf(t, 200, "%s/individual_image_size", entry_fn);
            add_offset((*dobj), strdup(t), 16 * entry_n + 4);
            
            snprintf(t, 200, "%s/individual_image_data_offset", entry_fn);
            add_offset((*dobj), strdup(t), 16 * entry_n + 8);
            
            snprintf(t, 200, "%s/individual_image_1_entry_number", entry_fn);
            add_offset((*dobj), strdup(t), 16 * entry_n + 12);
            
            snprintf(t, 200, "%s/individual_image_2_entry_number", entry_fn);
            add_offset((*dobj), strdup(t), 16 * entry_n + 14);
            
            FOR_CHILDREN(part, child) {
                if (IS_ELEMENT(part, "individual_image_attribute")) {
                    FOR_CHILDREN(at, part) {
                        const char *val_attr = mxmlElementGetAttr(at, "value");
                        if (val_attr == NULL) {
                            printf("ERROR: Element %s must have a value attribute!\n", at->value.element.name);
                            return 0;
                        }
                        if (IS_ELEMENT(at, "dependent_parent_image_flag")) {
                            if (IS_ELEMENT_ATTR(at, "value", "1")) iia |= 1<<31;
                        } else if (IS_ELEMENT(at, "dependent_child_image_flag")) {
                            if (IS_ELEMENT_ATTR(at, "value", "1")) iia |= 1<<30;
                        } else if (IS_ELEMENT(at, "representative_image_flag")) {
                            if (IS_ELEMENT_ATTR(at, "value", "1")) iia |= 1<<29;
                        } else if (IS_ELEMENT(at, "image_data_format")) {
                            if (IS_ELEMENT_ATTR(at, "value", "JPEG")) continue;
                            int res;
                            if (1==sscanf(val_attr, "reserved_%u", &res)) {
                                if (res>7) {
                                    printf("ERROR image_data_fromat must contain JPEG, or reserved_0..7\n");
                                    return 0;
                                }
                                iia |= res << 24;
                            } else {
                                 printf("ERROR image_data_fromat must contain JPEG, or reserved_0..7\n");
                                 return 0;
                            }
                        } else if (IS_ELEMENT(at, "mp_type")) {
                            int temp;
                            if (0==strcmp(val_attr, "Baseline MP Primary Image")) {
                                iia |= 0x030000;
                            } else if (0==strcmp(val_attr, "Large Thumbnail Class 1 (VGA equivalent)")) {
                                iia |= 0x010001;
                            } else if (0==strcmp(val_attr, "Large Thumbnail Class 2 (Full HD equivalent)")) {
                                iia |= 0x010002;
                            } else if (0==strcmp(val_attr, "Multi-Frame Image - Panorama")) {
                                iia |= 0x020001;
                            } else if (0==strcmp(val_attr, "Multi-Frame Image - Disparity")) {
                                iia |= 0x020002;
                            } else if (0==strcmp(val_attr, "Multi-Frame Image - Multi-Angle")) {
                                iia |= 0x020003;
                            } else if (0==strcmp(val_attr, "Undefined")) {
                                iia |= 0x000000;
                            } else if (1==sscanf(val_attr, "0x%x", &temp) && temp>=0 && temp<(1<<24)) {
                                iia |= temp;
                            } else {
                                printf("ERROR invalid value in MP_Entry->mp_type\n");
                                return 0;
                            }
                        } else if (IS_ELEMENT(at, "reserved_flag_28")) {
                            if (IS_ELEMENT_ATTR(at, "value", "1")) iia |= 1<<28;
                        } else if (IS_ELEMENT(at, "reserved_flag_27")) {
                            if (IS_ELEMENT_ATTR(at, "value", "1")) iia |= 1<<27;
                        } else {
                            printf("ERROR: invalid xml node in MPEntry tag, inside individual_image_attribute!\n");
                            return 0;
                        }
                    }
                } else if (IS_ELEMENT(part, "individual_image_size")) {
                    // ignore this, it's gonna be calculated later
                } else if (part->type == MXML_ELEMENT) {
                    // else we care about the value
                    const char *val_attr = mxmlElementGetAttr(part, "value");
                    if (val_attr == NULL) {
                        printf("ERROR: Element %s must have a value attribute!\n", part->value.element.name);
                        return 0;
                    }
                    unsigned int val;
                    if (1!=sscanf(val_attr, "%u", &val)) {
                        printf("value must be an unsigned number in tag node '%s'\n", part->value.element.name);
                    };
                    if (IS_ELEMENT(part, "individual_image_data_offset")) {
                        iio = val;
                    } else if (IS_ELEMENT(part, "dependent_image_1_entry_number")) {
                        dep1 = val;
                    } else if (IS_ELEMENT(part, "dependent_image_2_entry_number")) {
                        dep2 = val;
                    } else {
                        printf("ERROR: invalid xml node in MPEntry tag!\n");
                        return 0;
                    }
                } else {
                    printf("ERROR: invalid xml node in MPEntry tag!\n");
                    return 0;
                }
            }
            entry_n ++;
            write_uint32_writer(&wr, iia, le);
            pointer_t *size_pointer = mem_pointer(get_mem_writer_pointer_to_current_position(&wr));
            write_uint32_writer(&wr, iis, le);
            pointer_t *offset_pointer = mem_pointer(get_mem_writer_pointer_to_current_position(&wr));
            write_uint32_writer(&wr, iio, le);
            write_uint16_writer(&wr, dep1, le);
            write_uint16_writer(&wr, dep2, le);
            
            (*dobj)->my_pointers[(*dobj)->nr_my_pointers] = size_pointer;
            (*dobj)->nr_my_pointers ++;
            (*dobj)->my_pointers[(*dobj)->nr_my_pointers] = offset_pointer;
            (*dobj)->nr_my_pointers ++;
            
            add_mp_entry(&mp_entry_db, iio, offset_pointer, size_pointer, endian_pos, le);
            
        } else {
            printf("ERROR: invalid xml node in MPEntry tag!\n");
            return 0;
        }
    }
    write_uint32(count, wr.size, le);
    (*dobj)->size = wr.size;
    (*dobj)->buffer = free_memory_writer(&wr);
    return 1;
}

CUSTOMFUNCTION(PanOrientation) {
    uint32_t pan = i2xm_read_uint32(data, absolute_pos, le);
    switch (pan & 0xff) {
        case 0x01: mxmlElementSetAttrf(mxmlNewElement(tag, "shot_type"), "value", "left to right"); break;
        case 0x02: mxmlElementSetAttrf(mxmlNewElement(tag, "shot_type"), "value", "right to left"); break;
        case 0x03: mxmlElementSetAttrf(mxmlNewElement(tag, "shot_type"), "value", "top to bottom"); break;
        case 0x04: mxmlElementSetAttrf(mxmlNewElement(tag, "shot_type"), "value", "bottom to top"); break;
        case 0x10: mxmlElementSetAttrf(mxmlNewElement(tag, "shot_type"), "value", "Clockwise"); break;
        case 0x20: mxmlElementSetAttrf(mxmlNewElement(tag, "shot_type"), "value", "Counter Clockwise"); break;
        case 0x30: mxmlElementSetAttrf(mxmlNewElement(tag, "shot_type"), "value", "Zigzag (row start)"); break;
        case 0x40: mxmlElementSetAttrf(mxmlNewElement(tag, "shot_type"), "value", "Zigzag (column start)"); break;
        default: mxmlElementSetAttrf(mxmlNewElement(tag, "shot_type"), "value", "0x%x", pan & 0xff); break;
    }
    switch ((pan >> 8) & 0x0f) {
        case 0x1: mxmlElementSetAttrf(mxmlNewElement(tag, "start_of_first_image"), "value", "Top Right"); break;
        case 0x2: mxmlElementSetAttrf(mxmlNewElement(tag, "start_of_first_image"), "value", "Top Left"); break;
        case 0x3: mxmlElementSetAttrf(mxmlNewElement(tag, "start_of_first_image"), "value", "Bottom Left"); break;
        case 0x4: mxmlElementSetAttrf(mxmlNewElement(tag, "start_of_first_image"), "value", "Bottom Right"); break;
        default: mxmlElementSetAttrf(mxmlNewElement(tag, "start_of_first_image"), "value", "0x%x", (pan >> 8) & 0x0f); break;
    }
    mxmlElementSetAttrf(mxmlNewElement(tag, "number_of_images_per_row"), "value", "%u", (pan >> 16) & 0xff);
    mxmlElementSetAttrf(mxmlNewElement(tag, "number_of_images_per_column"), "value", "%u", (pan >> 24) & 0xff);
}

CUSTOMINTERPRET(PanOrientation) {
    unsigned int per_row=0, per_col=0, start=0, shot=0;
    write_uint32(count, 1, le);
    FOR_CHILDREN(child, node) {
        if (child->type!=MXML_ELEMENT) { printf("ERROR: unexpected non-element in PanOrientation tag!\n"); return 0; }
        const char *val_attr = mxmlElementGetAttr(child, "value");
        if (val_attr == NULL) {
            printf("ERROR: Element %s must have a value attribute!\n", child->value.element.name);
            return 0;
        }
        if (IS_ELEMENT(child, "shot_type")) {
            if      (0==strcmp(val_attr, "left to right")) shot = 1;
            else if (0==strcmp(val_attr, "right to left")) shot = 2;
            else if (0==strcmp(val_attr, "top to bottom")) shot = 3;
            else if (0==strcmp(val_attr, "bottom to top")) shot = 4;
            else if (0==strcmp(val_attr, "Clockwise")) shot = 0x10;
            else if (0==strcmp(val_attr, "Counter Clockwise")) shot = 0x20;
            else if (0==strcmp(val_attr, "Zigzag (row start)")) shot = 0x30;
            else if (0==strcmp(val_attr, "Zigzag (column start)")) shot = 0x40;
            else if (1!=sscanf(val_attr, "0x%x", &shot) || shot > 0xff) {
                printf("Invalid value in node '%s'\n", child->value.element.name);
                return 0;
            }
        } else if (IS_ELEMENT(child, "start_of_first_image")) {
            if      (0==strcmp(val_attr, "Top Right"))    start = 1;
            else if (0==strcmp(val_attr, "Top Left"))     start = 2;
            else if (0==strcmp(val_attr, "Bottom Left"))  start = 3;
            else if (0==strcmp(val_attr, "Bottom Right")) start = 4;
            else if (1!=sscanf(val_attr, "0x%x", &start) || start > 0xf) {
                printf("Invalid value in node '%s'\n", child->value.element.name);
                return 0;
            }
        } else if (IS_ELEMENT(child, "number_of_images_per_row")) {
            if (1!=sscanf(val_attr, "%u", &per_row) || per_row > 0xff) {
                printf("Invalid value in node '%s'\n", child->value.element.name);
                return 0;
            }
        } else if (IS_ELEMENT(child, "number_of_images_per_column")) {
            if (1!=sscanf(val_attr, "%u", &per_col) || per_col > 0xff) {
                printf("Invalid value in node '%s'\n", child->value.element.name);
                return 0;
            }
        } else { printf("ERROR: unexpected stuff inside the %s tag\n", node->value.element.name); return 0; }
    }
    uint32_t pan = shot | (start<<8) | (per_row<<16) | (per_col<<24);
    return make_long_obj(pan, pointer, full_name, dobj, le);
}

CUSTOMFUNCTION(All1UnknownRational) {
    if (i2xm_read_uint32(data, absolute_pos, le) == 0xffffffff && i2xm_read_uint32(data, absolute_pos + 4, le) == 0xffffffff) 
        mxmlElementSetAttrf(tag, "value", "unknown");
    else
        i2xm_read_single_value(data, &absolute_pos, tag, type, le);
}

CUSTOMINTERPRET(All1UnknownRational) {
    const char *val_attr = mxmlElementGetAttr(node, "value");
    if (0==strcmp(val_attr, "unknown")) {
        // make 64 bits of ones
        write_uint32(count, 1, le);
        return make_4_short_obj(0xffff, 0xffff, 0xffff, 0xffff, pointer, full_name, dobj, le);
    }
    return 0;
}

custom_tag_t *getCustomTag(custom_tag_t *tags, uint16_t tag_id) {
    int i;
    for (i=0; tags[i].id!=0xFFFF; i++) {
        if (tags[i].id == tag_id) return &tags[i];
    }
    return NULL;
}

custom_tag_t *getCustomTagFromName(custom_tag_t *tags, const char *tag_name) {
    int i;
    for (i=0; tags[i].id!=0xFFFF; i++) {
        if (0==strcmp(tags[i].name, tag_name)) return &tags[i];
    }
    return NULL;
}
