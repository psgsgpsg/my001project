/*******************************************************************************
 * SVGigE       Dynamic loading of GigE camera access functions
 *******************************************************************************
 *
 * Version:     1.4.14.26 / November 2008 
 *   
 * Copyright:   SVS VISTEK GmbH
 *
 *******************************************************************************
 *
 * THIS FILE CONTAINS BAYER PATTERN CONVERSION FROM THE FOLLOWING SOURCE: 
 *
 * 1394-Based Digital Camera Control Library
 * Bayer pattern decoding functions
 * Copyright (C) Damien Douxchamps <ddouxchamps@users.sf.net>
 *
 * Written by Damien Douxchamps and Frederic Devernay
 *******************************************************************************
 */
  
#include "windows.h"

#include "SVGigE.h" 

// Define a name for the SVS GigE API DLL
#ifdef X64
  #define SVGigE_DLL   "SVGigE.x64.dll"
#else
  #define SVGigE_DLL   "SVGigE.dll"
#endif
 
/**
 *  Handle for the GigE DLL
 */
HINSTANCE GigEDLL = NULL;

/**
 *  Enumerate all GigE function calls
 */
typedef
  enum
  {
    // DLL functions
    Func_isVersionCompliantDLL                        = 0,

    // Camera container functions
    Func_CameraContainer_create                       = 1,
    Func_CameraContainer_delete                       = 2,
    Func_CameraContainer_discovery                    = 3,
    Func_CameraContainer_getNumberOfCameras           = 4,
    Func_CameraContainer_getCamera                    = 5,
    Func_CameraContainer_findCamera                   = 6,

    // Camera functions
    Func_Camera_forceValidNetworkSettings             = 7,
    Func_Camera_openConnection                        = 8,
    Func_Camera_closeConnection                       = 9,
    Func_Camera_getManufacturerName                   = 10,
    Func_Camera_getModelName                          = 11,
    Func_Camera_getDeviceVersion                      = 12,
    Func_Camera_getManufacturerSpecificInformation    = 13,
    Func_Camera_getSerialNumber                       = 14,
    Func_Camera_setUserDefinedName                    = 15,
    Func_Camera_getUserDefinedName                    = 16,
    Func_Camera_getMacAddress                         = 17,
    Func_Camera_setIPAddress                          = 18,
    Func_Camera_getIPAddress                          = 19,
    Func_Camera_evaluateMaximalPacketSize             = 20,
    Func_Camera_setStreamingPacketSize								= 21,
    Func_Camera_getImagerWidth                        = 22,
    Func_Camera_getImagerHeight                       = 23,
    Func_Camera_getSizeX                              = 24,
    Func_Camera_getSizeY                              = 25,
    Func_Camera_getPitch                              = 26,
    Func_Camera_getImageSize                          = 27,
    Func_Camera_getPixelClock                         = 28,
    Func_Camera_getPixelType                          = 29,
    Func_Camera_setPixelDepth                         = 30,
    Func_Camera_getPixelDepth                         = 31,
    Func_Camera_isCameraFeature                       = 32,
    Func_Camera_readXMLFile                           = 33,
    Func_Camera_setFrameRate                          = 34,
    Func_Camera_getFrameRate                          = 35,
    Func_Camera_getFrameRateRange                     = 36,
    Func_Camera_setExposureTime                       = 37,
    Func_Camera_getExposureTime                       = 38,
    Func_Camera_getExposureTimeRange                  = 39,
    Func_Camera_setGain                               = 40,
    Func_Camera_getGain                               = 41,
    Func_Camera_setOffset                             = 42,
    Func_Camera_getOffset                             = 43,
    Func_Camera_setWhiteBalance                       = 44,
    Func_Camera_getWhiteBalance                       = 45,
    Func_Camera_setLookupTableMode                    = 46,
    Func_Camera_getLookupTableMode                    = 47,
    Func_Camera_setLookupTable                        = 48,
    Func_Camera_getLookupTable                        = 49,
    Func_Camera_createLUTwhiteBalance                 = 50,
    Func_Camera_setBinningMode                        = 51,
    Func_Camera_getBinningMode                        = 52,
    Func_Camera_setAreaOfInterest                     = 53,
    Func_Camera_getAreaOfInterest                     = 54,
    Func_Camera_saveTapBalanceSettings                = 55,
    Func_Camera_loadTapBalanceSettings                = 56,
    Func_Camera_setTapCalibration                     = 57,
    Func_Camera_getTapCalibration                     = 58,
    Func_Camera_setTapConfiguration                   = 59,
    Func_Camera_getTapConfiguration                   = 60,
    Func_Camera_setAcquisitionMode                    = 61,
    Func_Camera_setAcquisitionModeAndStart            = 62,
    Func_Camera_getAcquisitionMode                    = 63,
    Func_Camera_setAcquisitionControl                 = 64,
    Func_Camera_getAcquisitionControl                 = 65,
    Func_Camera_startAcquisitionCycle                 = 66,
    Func_Camera_setTriggerPolarity                    = 67,
    Func_Camera_getTriggerPolarity                    = 68,
    Func_Camera_setExposureDelay                      = 69,
    Func_Camera_getExposureDelay                      = 70,
    Func_Camera_setInterPacketDelay                   = 71,
    Func_Camera_getInterPacketDelay                   = 72,
    Func_Camera_setStrobePolarity                     = 73,
    Func_Camera_getStrobePolarity                     = 74,
    Func_Camera_setStrobePosition                     = 75,
    Func_Camera_getStrobePosition                     = 76,
    Func_Camera_setStrobeDuration                     = 77,
    Func_Camera_getStrobeDuration                     = 78,
    Func_Camera_writeEEPROM                           = 79,
    Func_Camera_readEEPROM                            = 80,
    Func_Camera_restoreFactoryDefaults                = 81,
    Func_Camera_stampTimestamp                        = 82,
    Func_Camera_getTimestamp                          = 83,
    Func_Camera_setGigECameraRegister                 = 84,
    Func_Camera_getGigECameraRegister                 = 85,
    Func_Camera_writeGigECameraMemory                 = 86,
    Func_Camera_readGigECameraMemory                  = 87,
    Func_Camera_registerForLogMessages                = 88,

    // Streaming channel functions
    Func_StreamingChannel_create                      = 89,
    Func_StreamingChannel_delete                      = 90,
    Func_StreamingChannel_getFrameLoss                = 91,
    Func_StreamingChannel_getActualFrameRate          = 92,
    Func_StreamingChannel_getActualDataRate           = 93,

    // Image functions
    Func_Image_release                                = 94,
    Func_Image_getDataPointer                         = 95,
    Func_Image_getCamera                              = 96,
    Func_Image_getBufferIndex                         = 97,
    Func_Image_getImageRGB                            = 98,
    Func_Image_getImage12bitAs8bit                    = 99,
    Func_Image_getImage12bitAs16bit                   = 100,
    Func_Image_getImage16bitAs8bit                    = 101,
    Func_Image_getImageSize                           = 102,
    Func_Image_getImageID                             = 103,
    Func_Image_getTimestamp                           = 104,
    Func_Image_getSizeX                               = 105,
    Func_Image_getSizeY                               = 106,
    Func_Image_getPitch                               = 107,
    Func_Image_getPixelType                           = 108,
    Func_Image_getPacketCount                         = 109,
    Func_Image_getPacketResend                        = 110,
    Func_Image_getSignalType                          = 111,

    // Common SVGigE functions
    Func_SVGigE_writeImageToBitmapFile                = 112,
    Func_SVGigE_estimateWhiteBalance                  = 113,

    // Error translation to text function
    Func_Error_getMessage                             = 114,
  }                                                   
  SVGigE_FUNCTION;                                    

/**
 *  Array of function pointers that will be obtained from the GigE DLL
 */
struct _GigEFunc
{
  FARPROC function_pointer;
  SVGigE_FUNCTION function_id;
  char *function_name;
}
GigEFunc[] =
{
  // DLL functions
  NULL, Func_isVersionCompliantDLL,                     "isVersionCompliantDLL",

  // Camera container functions
  NULL, Func_CameraContainer_create,                    "CameraContainer_create",
  NULL, Func_CameraContainer_delete,                    "CameraContainer_delete",
  NULL, Func_CameraContainer_discovery,                 "CameraContainer_discovery",
  NULL, Func_CameraContainer_discovery,                 "CameraContainer_getNumberOfCameras",
  NULL, Func_CameraContainer_getCamera,                 "CameraContainer_getCamera",
  NULL, Func_CameraContainer_findCamera,                "CameraContainer_findCamera",
  

  // Camera functions
  NULL, Func_Camera_forceValidNetworkSettings,          "Camera_forceValidNetworkSettings",
  NULL, Func_Camera_openConnection,                     "Camera_openConnection",
  NULL, Func_Camera_closeConnection,                    "Camera_closeConnection",
  NULL, Func_Camera_getManufacturerName,                "Camera_getManufacturerName",
  NULL, Func_Camera_getModelName,                       "Camera_getModelName",
  NULL, Func_Camera_getDeviceVersion,                   "Camera_getDeviceVersion",
  NULL, Func_Camera_getManufacturerSpecificInformation, "Camera_getManufacturerSpecificInformation",
  NULL, Func_Camera_getSerialNumber,                    "Camera_getSerialNumber",
  NULL, Func_Camera_setUserDefinedName,                 "Camera_setUserDefinedName",
  NULL, Func_Camera_getUserDefinedName,                 "Camera_getUserDefinedName",
  NULL, Func_Camera_getMacAddress,                      "Camera_getMacAddress",
  NULL, Func_Camera_setIPAddress,                       "Camera_setIPAddress",
  NULL, Func_Camera_getIPAddress,                       "Camera_getIPAddress",
  NULL, Func_Camera_evaluateMaximalPacketSize,          "Camera_evaluateMaximalPacketSize",
	NULL, Func_Camera_setStreamingPacketSize,							"Camera_setStreamingPacketSize",
  NULL, Func_Camera_getImagerWidth,                     "Camera_getImagerWidth",
  NULL, Func_Camera_getImagerHeight,                    "Camera_getImagerHeight",
  NULL, Func_Camera_getSizeX,                           "Camera_getSizeX",
  NULL, Func_Camera_getSizeY,                           "Camera_getSizeY",
  NULL, Func_Camera_getPitch,                           "Camera_getPitch",
  NULL, Func_Camera_getImageSize,                       "Camera_getImageSize",
  NULL, Func_Camera_getPixelClock,                      "Camera_getPixelClock",
  NULL, Func_Camera_getPixelType,                       "Camera_getPixelType",
  NULL, Func_Camera_setPixelDepth,                      "Camera_setPixelDepth",
  NULL, Func_Camera_getPixelDepth,                      "Camera_getPixelDepth",
  NULL, Func_Camera_isCameraFeature,                    "Camera_isCameraFeature",
  NULL, Func_Camera_readXMLFile,                        "Camera_readXMLFile",
  NULL, Func_Camera_setFrameRate,                       "Camera_setFrameRate",
  NULL, Func_Camera_getFrameRate,                       "Camera_getFrameRate",
  NULL, Func_Camera_getFrameRateRange,                  "Camera_getFrameRateRange",
  NULL, Func_Camera_setExposureTime,                    "Camera_setExposureTime",
  NULL, Func_Camera_getExposureTime,                    "Camera_getExposureTime",
  NULL, Func_Camera_getExposureTimeRange,               "Camera_getExposureTimeRange",
  NULL, Func_Camera_setGain,                            "Camera_setGain",
  NULL, Func_Camera_getGain,                            "Camera_getGain",
  NULL, Func_Camera_setOffset,                          "Camera_setOffset",
  NULL, Func_Camera_getOffset,                          "Camera_getOffset",
  NULL, Func_Camera_setWhiteBalance,                    "Camera_setWhiteBalance",
  NULL, Func_Camera_getWhiteBalance,                    "Camera_getWhiteBalance",
  NULL, Func_Camera_setLookupTableMode,                 "Camera_setLookupTableMode",
  NULL, Func_Camera_getLookupTableMode,                 "Camera_getLookupTableMode",
  NULL, Func_Camera_setLookupTable,                     "Camera_setLookupTable",
  NULL, Func_Camera_getLookupTable,                     "Camera_getLookupTable",
  NULL, Func_Camera_createLUTwhiteBalance,              "Camera_createLUTwhiteBalance",
  NULL, Func_Camera_setBinningMode,                     "Camera_setBinningMode",
  NULL, Func_Camera_getBinningMode,                     "Camera_getBinningMode",
  NULL, Func_Camera_setAreaOfInterest,                  "Camera_setAreaOfInterest",
  NULL, Func_Camera_getAreaOfInterest,                  "Camera_getAreaOfInterest",
  NULL, Func_Camera_saveTapBalanceSettings,             "Camera_saveTapBalanceSettings",
  NULL, Func_Camera_loadTapBalanceSettings,             "Camera_loadTapBalanceSettings",
  NULL, Func_Camera_setTapCalibration,                  "Camera_setTapCalibration",
  NULL, Func_Camera_getTapCalibration,                  "Camera_getTapCalibration",
  NULL, Func_Camera_setTapConfiguration,                "Camera_setTapConfiguration",
  NULL, Func_Camera_getTapConfiguration,                "Camera_getTapConfiguration",
  NULL, Func_Camera_setAcquisitionMode,                 "Camera_setAcquisitionMode",
  NULL, Func_Camera_setAcquisitionModeAndStart,         "Camera_setAcquisitionModeAndStart",
  NULL, Func_Camera_getAcquisitionMode,                 "Camera_getAcquisitionMode",
  NULL, Func_Camera_setAcquisitionControl,              "Camera_setAcquisitionControl",
  NULL, Func_Camera_getAcquisitionControl,              "Camera_getAcquisitionControl",
  NULL, Func_Camera_startAcquisitionCycle,              "Camera_startAcquisitionCycle",
  NULL, Func_Camera_setTriggerPolarity,                 "Camera_setTriggerPolarity",
  NULL, Func_Camera_getTriggerPolarity,                 "Camera_getTriggerPolarity",
  NULL, Func_Camera_setExposureDelay,                   "Camera_setExposureDelay",
  NULL, Func_Camera_getExposureDelay,                   "Camera_getExposureDelay",
  NULL, Func_Camera_setInterPacketDelay,                "Camera_setInterPacketDelay",
  NULL, Func_Camera_getInterPacketDelay,                "Camera_getInterPacketDelay",
  NULL, Func_Camera_setStrobePolarity,                  "Camera_setStrobePolarity",
  NULL, Func_Camera_getStrobePolarity,                  "Camera_getStrobePolarity",
  NULL, Func_Camera_setStrobePosition,                  "Camera_setStrobePosition",
  NULL, Func_Camera_getStrobePosition,                  "Camera_getStrobePosition",
  NULL, Func_Camera_setStrobeDuration,                  "Camera_setStrobeDuration",
  NULL, Func_Camera_getStrobeDuration,                  "Camera_getStrobeDuration",
  NULL, Func_Camera_writeEEPROM,                        "Camera_writeEEPROM",
  NULL, Func_Camera_readEEPROM,                         "Camera_readEEPROM",
  NULL, Func_Camera_restoreFactoryDefaults,             "Camera_restoreFactoryDefaults",
  NULL, Func_Camera_stampTimestamp,                     "Camera_stampTimestamp",
  NULL, Func_Camera_getTimestamp,                       "Camera_getTimestamp",
  NULL, Func_Camera_setGigECameraRegister,              "Camera_setGigECameraRegister",
  NULL, Func_Camera_getGigECameraRegister,              "Camera_getGigECameraRegister",
  NULL, Func_Camera_writeGigECameraMemory,              "Camera_writeGigECameraMemory",
  NULL, Func_Camera_readGigECameraMemory,               "Camera_readGigECameraMemory",
  NULL, Func_Camera_registerForLogMessages,             "Camera_registerForLogMessages",

  // Streaming channel functions
  NULL, Func_StreamingChannel_create,                   "StreamingChannel_create",
  NULL, Func_StreamingChannel_delete,                   "StreamingChannel_delete",
  NULL, Func_StreamingChannel_getFrameLoss,             "StreamingChannel_getFrameLoss",
  NULL, Func_StreamingChannel_getActualFrameRate,       "StreamingChannel_getActualFrameRate",
  NULL, Func_StreamingChannel_getActualDataRate,        "StreamingChannel_getActualDataRate",

  // Image functions
  NULL, Func_Image_release,                             "Image_release",
  NULL, Func_Image_getDataPointer,                      "Image_getDataPointer",
  NULL, Func_Image_getCamera,                           "Image_getCamera",
  NULL, Func_Image_getBufferIndex,                      "Image_getBufferIndex",
  NULL, Func_Image_getImageRGB,                         "Image_getImageRGB",
  NULL, Func_Image_getImage12bitAs8bit,                 "Image_getImage12bitAs8bit",
  NULL, Func_Image_getImage12bitAs16bit,                "Image_getImage12bitAs16bit",
  NULL, Func_Image_getImage16bitAs8bit,                 "Image_getImage16bitAs8bit",
  NULL, Func_Image_getImageSize,                        "Image_getImageSize",
  NULL, Func_Image_getImageID,                          "Image_getImageID",
  NULL, Func_Image_getTimestamp,                        "Image_getTimestamp",
  NULL, Func_Image_getSizeX,                            "Image_getSizeX",
  NULL, Func_Image_getSizeY,                            "Image_getSizeY",
  NULL, Func_Image_getPitch,                            "Image_getPitch",
  NULL, Func_Image_getPixelType,                        "Image_getPixelType",
  NULL, Func_Image_getPacketCount,                      "Image_getPacketCount",
  NULL, Func_Image_getPacketResend,                     "Image_getPacketResend",
  NULL, Func_Image_getSignalType,                       "Image_getSignalType",

  // Common SVGigE functions
  NULL, Func_SVGigE_writeImageToBitmapFile,             "SVGigE_writeImageToBitmapFile",
  NULL, Func_SVGigE_estimateWhiteBalance,               "SVGigE_estimateWhiteBalance",

  // Error translation to text function
  NULL, Func_Error_getMessage,                          "Error_getMessage",
};

/**
 *  Specify function types to be used for casting function pointers that are retrieved from the DLL
 */
typedef SVGigE_RETURN
(*TFunc_isVersionCompliantDLL)(SVGigE_VERSION *DllVersion, 
                               SVGigE_VERSION *ExpectedVersion);

typedef CameraContainerClient_handle
(*TFunc_CameraContainer_create)(SVGigETL_Type TransportLayerType);

typedef SVGigE_RETURN
(*TFunc_CameraContainer_delete)(CameraContainerClient_handle hCameraContainer);

typedef SVGigE_RETURN
(*TFunc_CameraContainer_discovery)(CameraContainerClient_handle hCameraContainer);

typedef int
(*TFunc_CameraContainer_getNumberOfCameras)(CameraContainerClient_handle hCameraContainer);

typedef  Camera_handle
(*TFunc_CameraContainer_getCamera)(CameraContainerClient_handle hCameraContainer,
                                   int CameraIndex);

typedef  Camera_handle
(*TFunc_CameraContainer_findCamera)(CameraContainerClient_handle hCameraContainer,
                                    char *CameraName);

typedef  SVGigE_RETURN
(*TFunc_Camera_forceValidNetworkSettings)(Camera_handle hCamera, 
                                          unsigned int *IPAddress,
                                          unsigned int *SubnetMask);

typedef SVGigE_RETURN
(*TFunc_Camera_openConnection)(Camera_handle hCamera, float Timeout);

typedef SVGigE_RETURN
(*TFunc_Camera_closeConnection)(Camera_handle hCamera);

typedef char *
(*TFunc_Camera_getManufacturerName)(Camera_handle hCamera);

typedef char *
(*TFunc_Camera_getModelName)(Camera_handle hCamera);

typedef char *
(*TFunc_Camera_getDeviceVersion)(Camera_handle hCamera);

typedef char *
(*TFunc_Camera_getManufacturerSpecificInformation)(Camera_handle hCamera);

typedef char *
(*TFunc_Camera_getSerialNumber)(Camera_handle hCamera);

typedef SVGigE_RETURN
(*TFunc_Camera_setUserDefinedName)(Camera_handle hCamera, char *UserDefinedName);

typedef char *
(*TFunc_Camera_getUserDefinedName)(Camera_handle hCamera);

typedef char *
(*TFunc_Camera_getMacAddress)(Camera_handle hCamera);

typedef SVGigE_RETURN
(*TFunc_Camera_setIPAddress)(Camera_handle hCamera,
                             unsigned int IPAddress,
                             unsigned int SubnetMask);

typedef char *
(*TFunc_Camera_getIPAddress)(Camera_handle hCamera);

typedef SVGigE_RETURN
(*TFunc_Camera_evaluateMaximalPacketSize)(Camera_handle hCamera,
                                          int *MaximalPacketSize);

typedef SVGigE_RETURN
(*TFunc_Camera_setStreamingPacketSize)(Camera_handle hCamera,
                                       int StreamingPacketSize);

typedef SVGigE_RETURN
(*TFunc_Camera_getImagerWidth)(Camera_handle hCamera,
                               int *ImagerWidth);

typedef SVGigE_RETURN
(*TFunc_Camera_getImagerHeight)(Camera_handle hCamera,
                                int *ImagerHeight);

typedef SVGigE_RETURN
(*TFunc_Camera_getSizeX)(Camera_handle hCamera,
                         int *SizeX);

typedef SVGigE_RETURN
(*TFunc_Camera_getSizeY)(Camera_handle hCamera,
                         int *SizeY);

typedef SVGigE_RETURN
(*TFunc_Camera_getPitch)(Camera_handle hCamera,
                         int *Pitch);

typedef SVGigE_RETURN
(*TFunc_Camera_getImageSize)(Camera_handle hCamera,
                             int *ImageSize);

typedef SVGigE_RETURN
(*TFunc_Camera_getPixelClock)(Camera_handle hCamera,
                             int *PixelClock);

typedef SVGigE_RETURN
(*TFunc_Camera_getPixelType)(Camera_handle hCamera,
                             GVSP_PIXEL_TYPE *PixelType);

typedef SVGigE_RETURN
(*TFunc_Camera_setPixelDepth)(Camera_handle hCamera,
                              SVGIGE_PIXEL_DEPTH PixelDepth);

typedef SVGigE_RETURN
(*TFunc_Camera_getPixelDepth)(Camera_handle hCamera,
                              SVGIGE_PIXEL_DEPTH *PixelDepth);

typedef bool
(*TFunc_Camera_isCameraFeature)(Camera_handle hCamera,
                                CAMERA_FEATURE Feature);

typedef SVGigE_RETURN
(*TFunc_Camera_readXMLFile)(Camera_handle hCamera,
                            char **XML);

typedef SVGigE_RETURN
(*TFunc_Camera_setFrameRate)(Camera_handle hCamera,
                             float Framerate);

typedef SVGigE_RETURN
(*TFunc_Camera_getFrameRate)(Camera_handle hCamera,
                             float *ProgrammedFramerate);

typedef SVGigE_RETURN
(*TFunc_Camera_getFrameRateRange)(Camera_handle hCamera,
                                  float *MinFramerate,
                                  float *MaxFramerate);

typedef SVGigE_RETURN
(*TFunc_Camera_setExposureTime)(Camera_handle hCamera,
                                float ExposureTime);

typedef SVGigE_RETURN
(*TFunc_Camera_getExposureTime)(Camera_handle hCamera,
                                float *ProgrammedExposureTime);

typedef SVGigE_RETURN
(*TFunc_Camera_getExposureTimeRange)(Camera_handle hCamera,
                                     float *MinExposureTime,
                                     float *MaxExposureTime);

typedef SVGigE_RETURN
(*TFunc_Camera_setGain)(Camera_handle hCamera,
                        float Gain);

typedef SVGigE_RETURN
(*TFunc_Camera_getGain)(Camera_handle hCamera,
                        float *ProgrammedGain);

typedef SVGigE_RETURN
(*TFunc_Camera_setOffset)(Camera_handle hCamera,
                         float Offset);

typedef SVGigE_RETURN
(*TFunc_Camera_getOffset)(Camera_handle hCamera,
                         float *Offset);

typedef SVGigE_RETURN
(*TFunc_Camera_setWhiteBalance)(Camera_handle hCamera,
                                float Red,
                                float Green ,
                                float Blue);

typedef SVGigE_RETURN
(*TFunc_Camera_getWhiteBalance)(Camera_handle hCamera,
                                float *Red,
                                float *Green ,
                                float *Blue);

typedef SVGigE_RETURN
(*TFunc_Camera_setLookupTableMode)(Camera_handle hCamera,
                                   LUT_MODE LUTMode);

typedef SVGigE_RETURN
(*TFunc_Camera_getLookupTableMode)(Camera_handle hCamera,
                                   LUT_MODE *ProgrammedLUTMode);

typedef SVGigE_RETURN
(*TFunc_Camera_setLookupTable)(Camera_handle hCamera,
                               unsigned char *LookupTable,
                               int LookupTableSize);

typedef SVGigE_RETURN
(*TFunc_Camera_getLookupTable)(Camera_handle hCamera,
                               unsigned char *LookupTable,
                               int LookupTableSize);
//----DEPRECATED----
typedef SVGigE_RETURN
(*TFunc_Camera_createLUTwhiteBalance)(Camera_handle hCamera,
                                      float Red,
                                      float Green ,
                                      float Blue);

typedef SVGigE_RETURN
(*TFunc_Camera_setBinningMode)(Camera_handle hCamera,
                               BINNING_MODE BinningMode);

typedef SVGigE_RETURN
(*TFunc_Camera_getBinningMode)(Camera_handle hCamera,
                              BINNING_MODE *ProgrammedBinningMode);

typedef SVGigE_RETURN
(*TFunc_Camera_setAreaOfInterest)(Camera_handle hCamera,
                                  int SizeX,
                                  int SizeY,
                                  int OffsetX,
                                  int OffsetY);

typedef SVGigE_RETURN
(*TFunc_Camera_getAreaOfInterest)(Camera_handle hCamera,
                                  int *ProgrammedSizeX,
                                  int *ProgrammedSizeY,
                                  int *ProgrammedOffsetX,
                                  int *ProgrammedOffsetY);

typedef SVGigE_RETURN 
(*TFunc_Camera_saveTapBalanceSettings)(Camera_handle hCamera,
                                       char *Filename);

typedef SVGigE_RETURN 
(*TFunc_Camera_loadTapBalanceSettings)(Camera_handle hCamera,
                                       char *Filename);

typedef SVGigE_RETURN 
(*TFunc_Camera_setTapCalibration)(Camera_handle hCamera,
                                  unsigned int TapID, 
                                  unsigned int Gain, 
                                  unsigned int Offset);

typedef SVGigE_RETURN 
(*TFunc_Camera_getTapCalibration)(Camera_handle hCamera,
                                  unsigned int TapID, 
                                  unsigned int *Gain, 
                                  unsigned int *Offset);

typedef SVGigE_RETURN 
(*TFunc_Camera_setTapConfiguration)(Camera_handle hCamera,
                                    int TapCount);

typedef SVGigE_RETURN 
(*TFunc_Camera_getTapConfiguration)(Camera_handle hCamera,
                                    int *TapCount);

typedef SVGigE_RETURN
(*TFunc_Camera_setAcquisitionMode)(Camera_handle hCamera,
                                  ACQUISITION_MODE AcquisitionMode);

typedef SVGigE_RETURN
(*TFunc_Camera_setAcquisitionModeAndStart)(Camera_handle hCamera,
                                           ACQUISITION_MODE AcquisitionMode,
                                           bool AcquisitionStart);

typedef SVGigE_RETURN
(*TFunc_Camera_getAcquisitionMode)(Camera_handle hCamera,
                                   ACQUISITION_MODE *ProgrammedAcquisitionMode);

typedef SVGigE_RETURN
(*TFunc_Camera_setAcquisitionControl)(Camera_handle hCamera,
                                      ACQUISITION_CONTROL AcquisitionControl);

typedef SVGigE_RETURN
(*TFunc_Camera_getAcquisitionControl)(Camera_handle hCamera,
                                      ACQUISITION_CONTROL *ProgrammedAcquisitionControl);

typedef SVGigE_RETURN
(*TFunc_Camera_startAcquisitionCycle)(Camera_handle hCamera);

typedef SVGigE_RETURN
(*TFunc_Camera_setTriggerPolarity)(Camera_handle hCamera,
                                   TRIGGER_POLARITY TriggerPolarity);

typedef SVGigE_RETURN
(*TFunc_Camera_getTriggerPolarity)(Camera_handle hCamera,
                                   TRIGGER_POLARITY *ProgrammedTriggerPolarity);

typedef SVGigE_RETURN
(*TFunc_Camera_setExposureDelay)(Camera_handle hCamera,
                                 float ExposureDelay);

typedef SVGigE_RETURN
(*TFunc_Camera_getExposureDelay)(Camera_handle hCamera,
                                 float *ProgrammedExposureDelay);

typedef SVGigE_RETURN
(*TFunc_Camera_setInterPacketDelay)(Camera_handle hCamera,
                                    float InterPacketDelay);

typedef SVGigE_RETURN
(*TFunc_Camera_getInterPacketDelay)(Camera_handle hCamera,
                                    float *ProgrammedInterPacketDelay);

typedef SVGigE_RETURN
(*TFunc_Camera_setStrobePolarity)(Camera_handle hCamera,
                                  STROBE_POLARITY StrobePolarity);

typedef SVGigE_RETURN
(*TFunc_Camera_getStrobePolarity)(Camera_handle hCamera,
                                  STROBE_POLARITY *ProgrammedStrobePolarity);

typedef SVGigE_RETURN
(*TFunc_Camera_setStrobePosition)(Camera_handle hCamera,
                                  float StrobePosition);

typedef SVGigE_RETURN
(*TFunc_Camera_getStrobePosition)(Camera_handle hCamera,
                                  float *ProgrammedStrobePosition);

typedef SVGigE_RETURN
(*TFunc_Camera_setStrobeDuration)(Camera_handle hCamera,
                                  float StrobeDuration);

typedef SVGigE_RETURN
(*TFunc_Camera_getStrobeDuration)(Camera_handle hCamera,
                                  float *ProgrammedStrobeDuration);

typedef SVGigE_RETURN
(*TFunc_Camera_writeEEPROM)(Camera_handle hCamera);

typedef SVGigE_RETURN
(*TFunc_Camera_readEEPROM)(Camera_handle hCamera);

typedef SVGigE_RETURN
(*TFunc_Camera_restoreFactoryDefaults)(Camera_handle hCamera);

typedef SVGigE_RETURN
(*TFunc_Camera_stampTimestamp)(Camera_handle hCamera,
                               int TimestampIndex);

typedef SVGigE_RETURN
(*TFunc_Camera_getTimestamp)(Camera_handle hCamera,
                             int TimestampIndex,
                             double *Timestamp);

typedef SVGigE_RETURN
(*TFunc_Camera_setGigECameraRegister)(Camera_handle hCamera,
                                      unsigned int RegisterAddress,
                                      unsigned int RegisterValue,
                                      unsigned int GigECameraAccessKey);

typedef SVGigE_RETURN
(*TFunc_Camera_getGigECameraRegister)(Camera_handle hCamera,
                                      unsigned int RegisterAddress,
                                      unsigned int *ProgrammedRegisterValue,
                                      unsigned int GigECameraAccessKey);

typedef SVGigE_RETURN
(*TFunc_Camera_writeGigECameraMemory)(Camera_handle hCamera,
                                      unsigned int  MemoryAddress,
                                      unsigned char *DataBlock,
                                      unsigned int  DataLength,
                                      unsigned int  GigECameraAccessKey);

typedef SVGigE_RETURN
(*TFunc_Camera_readGigECameraMemory)(Camera_handle hCamera,
                                     unsigned int  MemoryAddress,
                                     unsigned char *DataBlock,
                                     unsigned int  DataLength,
                                     unsigned int  GigECameraAccessKey);

typedef SVGigE_RETURN
(*TFunc_Camera_registerForLogMessages)(Camera_handle hCamera,
                                       int  LogLevel,
                                       char *LogFilename,
                                       LogMessageCallback LogCallback,
                                       void *MessageContext);

typedef SVGigE_RETURN
(*TFunc_StreamingChannel_create)(StreamingChannel_handle *hStreamingChannel,
                                 CameraContainerClient_handle hCameraContainer,
                                 Camera_handle hCamera,
                                 int BufferCount,
                                 StreamCallback CallbackFunction,
                                 void *Context);

typedef SVGigE_RETURN
(*TFunc_StreamingChannel_delete)(StreamingChannel_handle hStreamingChannel);

typedef SVGigE_RETURN
(*TFunc_StreamingChannel_getFrameLoss)(StreamingChannel_handle hStreamingChannel,
                                       int *FrameLoss);

typedef SVGigE_RETURN
(*TFunc_StreamingChannel_getActualFrameRate)(StreamingChannel_handle hStreamingChannel,
                                             float *ActualFrameRate);

typedef SVGigE_RETURN
(*TFunc_StreamingChannel_getActualDataRate)(StreamingChannel_handle hStreamingChannel,
                                            float *ActualDataRate);

typedef SVGigE_RETURN
(*TFunc_Image_release)(Image_handle hImage);

typedef unsigned char *
(*TFunc_Image_getDataPointer)(Image_handle hImage);

typedef Camera_handle
(*TFunc_Image_getCamera)(Image_handle hImage);

typedef int
(*TFunc_Image_getBufferIndex)(Image_handle hImage);

typedef SVGigE_RETURN
(*TFunc_Image_getImageRGB)(Image_handle hImage,
                           unsigned char *BufferRGB,
                           int BufferLength,
                           BAYER_METHOD BayerMethod);

typedef SVGigE_RETURN
(*TFunc_Image_getImage12bitAs8bit)(Image_handle hImage,
                                   unsigned char *Buffer8bit,
                                   int BufferLength);

typedef SVGigE_RETURN
(*TFunc_Image_getImage12bitAs16bit)(Image_handle hImage,
                                    unsigned char *Buffer16bit,
                                    int BufferLength);

typedef SVGigE_RETURN
(*TFunc_Image_getImage16bitAs8bit)(Image_handle hImage,
                                   unsigned char *Buffer8bit,
                                   int BufferLength);

typedef int
(*TFunc_Image_getImageSize)(Image_handle hImage);

typedef int
(*TFunc_Image_getImageID)(Image_handle hImage);

typedef double
(*TFunc_Image_getTimestamp)(Image_handle hImage);

typedef int
(*TFunc_Image_getSizeX)(Image_handle hImage);

typedef	int
(*TFunc_Image_getSizeY)(Image_handle hImage);

typedef	int
(*TFunc_Image_getPitch)(Image_handle hImage);

typedef GVSP_PIXEL_TYPE
(*TFunc_Image_getPixelType)(Image_handle hImage);

typedef int
(*TFunc_Image_getPacketCount)(Image_handle hImage);

typedef int
(*TFunc_Image_getPacketResend)(Image_handle hImage);

typedef SVGigE_SIGNAL_TYPE
(*TFunc_Image_getSignalType)(Image_handle hImage);

typedef SVGigE_RETURN 
(*TFunc_SVGigE_writeImageToBitmapFile)(char *Filename, unsigned char *Data, int SizeX, int SizeY, GVSP_PIXEL_TYPE PixelType);

typedef SVGigE_RETURN 
(*TFunc_SVGigE_estimateWhiteBalance)(unsigned char *DataRGB, int DataRGBLength, float *Red, float *Green, float *Blue);

typedef char *
(*TFunc_Error_getMessage)(SVGigE_RETURN ReturnCode);

/**
 *  isLoadedDLL.
 *  A check is performed if the SVGigE.dll has already been loaded and
 *  if this was not the case then it will be tried to load it.
 *
 *  @return a flag indicating whether the DLL is available to the application
 */
bool isLoadedGigEDLL()
{
  if( NULL == GigEDLL )
  {
    // Try to load GigE DLL
    GigEDLL = LoadLibrary(SVGigE_DLL);

    // Check DLL availability
    if( NULL == GigEDLL )
      return false;
  }

  // Obtain CameraContainer procedure addresses
  bool function_loaded = true;
  for( int function_index = Func_isVersionCompliantDLL;
       function_index < (sizeof(GigEFunc) / sizeof(struct _GigEFunc));
       function_index++
     )
  {
    GigEFunc[function_index].function_pointer = GetProcAddress(GigEDLL, GigEFunc[function_index].function_name);

    // Check if function was found
    if( NULL == GigEFunc[function_index].function_pointer )
      function_loaded = false;
  }

  // Check if all function pointers could successfully be obtained from the DLL
  if( function_loaded == false )
    return false;
  else
    return true;
}

SVGigE_RETURN
isVersionCompliantDLL(SVGigE_VERSION *DllVersion, 
                      SVGigE_VERSION *ExpectedVersion)
{
  // Check DLL availability
  if( NULL == GigEDLL )
  {
    // Try to load SVGigE DLL
    if( !isLoadedGigEDLL() )
      return SVGigE_DLL_NOT_LOADED;
  }

  // Pass through function call to DLL
  return ((TFunc_isVersionCompliantDLL)
  GigEFunc[Func_isVersionCompliantDLL].function_pointer)(DllVersion, ExpectedVersion);
}

CameraContainerClient_handle
CameraContainer_create(SVGigETL_Type TransportLayerType)
{
  // Load DLL 
  if( !isLoadedGigEDLL() )
    return SVGigE_NO_CLIENT;

  // Check DLL version against version at compile time
  SVGigE_VERSION DllVersion;
  SVGigE_VERSION ExpectedVersion;
  ExpectedVersion.MajorVersion  = SVGigE_VERSION_MAJOR;
  ExpectedVersion.MinorVersion  = SVGigE_VERSION_MINOR;
  ExpectedVersion.DriverVersion = SVGigE_VERSION_DRIVER;
  ExpectedVersion.BuildVersion  = SVGigE_VERSION_BUILD;
  if( SVGigE_SUCCESS != isVersionCompliantDLL(&DllVersion, &ExpectedVersion) )
    return SVGigE_NO_CLIENT;

  // Pass through function call to DLL
  return ((TFunc_CameraContainer_create)
  GigEFunc[Func_CameraContainer_create].function_pointer)(TransportLayerType);
}

SVGigE_RETURN
CameraContainer_delete(CameraContainerClient_handle hCameraContainer)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  SVGigE_RETURN Ret =
  ((TFunc_CameraContainer_delete)
  GigEFunc[Func_CameraContainer_delete].function_pointer)(hCameraContainer);

  // Release DLL (reference counter will be decreased)
  FreeLibrary(GigEDLL);
  GigEDLL = NULL;

	return Ret; 
}

SVGigE_RETURN
CameraContainer_discovery(CameraContainerClient_handle hCameraContainer)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_CameraContainer_discovery)
    GigEFunc[Func_CameraContainer_discovery].function_pointer)(hCameraContainer);
}

int
CameraContainer_getNumberOfCameras(CameraContainerClient_handle hCameraContainer)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_CameraContainer_getNumberOfCameras)
  GigEFunc[Func_CameraContainer_getNumberOfCameras].function_pointer)(hCameraContainer);
}

Camera_handle
CameraContainer_getCamera(CameraContainerClient_handle hCameraContainer,
                          int CameraIndex)
{
  // Check DLL availablility
  if( NULL == GigEDLL )
    return SVGigE_NO_CAMERA;

  // Pass through function call to DLL
  return ((TFunc_CameraContainer_getCamera)
  GigEFunc[Func_CameraContainer_getCamera].function_pointer)(hCameraContainer, CameraIndex);
}

Camera_handle
CameraContainer_findCamera(CameraContainerClient_handle hCameraContainer,
                           char *CameraItem)
{
  // Check DLL availablility
  if( NULL == GigEDLL )
    return SVGigE_NO_CAMERA;

  // Pass through function call to DLL
  return ((TFunc_CameraContainer_findCamera)
  GigEFunc[Func_CameraContainer_findCamera].function_pointer)(hCameraContainer, CameraItem);
}

SVGigE_RETURN
Camera_forceValidNetworkSettings(Camera_handle hCamera, 
                                 unsigned int *IPAddress,
                                 unsigned int *SubnetMask)
{
  // Check DLL availablility
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_forceValidNetworkSettings)
  GigEFunc[Func_Camera_forceValidNetworkSettings].function_pointer)(hCamera, IPAddress, SubnetMask);
}

SVGigE_RETURN
Camera_openConnection(Camera_handle hCamera, float Timeout)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_openConnection)
  GigEFunc[Func_Camera_openConnection].function_pointer)(hCamera, Timeout);
}

SVGigE_RETURN
Camera_closeConnection(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_closeConnection)
  GigEFunc[Func_Camera_closeConnection].function_pointer)(hCamera);
}

char *
Camera_getManufacturerName(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return "";

  // Pass through function call to DLL
  return ((TFunc_Camera_getManufacturerName)
  GigEFunc[Func_Camera_getManufacturerName].function_pointer)(hCamera);
}

char *
Camera_getModelName(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return "";

  // Pass through function call to DLL
  return ((TFunc_Camera_getModelName)
  GigEFunc[Func_Camera_getModelName].function_pointer)(hCamera);
}

char *
Camera_getDeviceVersion(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return "";

  // Pass through function call to DLL
  return ((TFunc_Camera_getDeviceVersion)
  GigEFunc[Func_Camera_getDeviceVersion].function_pointer)(hCamera);
}

char *
Camera_getManufacturerSpecificInformation(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return "";

  // Pass through function call to DLL
  return ((TFunc_Camera_getManufacturerSpecificInformation)
  GigEFunc[Func_Camera_getManufacturerSpecificInformation].function_pointer)(hCamera);
}

char *
Camera_getSerialNumber(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return "";

  // Pass through function call to DLL
  return ((TFunc_Camera_getSerialNumber)
  GigEFunc[Func_Camera_getSerialNumber].function_pointer)(hCamera);
}

SVGigE_RETURN
Camera_setUserDefinedName(Camera_handle hCamera, char *UserDefinedName)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setUserDefinedName)
  GigEFunc[Func_Camera_setUserDefinedName].function_pointer)(hCamera, UserDefinedName);
}

char *
Camera_getUserDefinedName(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return "";

  // Pass through function call to DLL
  return ((TFunc_Camera_getUserDefinedName)
  GigEFunc[Func_Camera_getUserDefinedName].function_pointer)(hCamera);
}

char *
Camera_getMacAddress(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return "";

  // Pass through function call to DLL
  return ((TFunc_Camera_getMacAddress)
  GigEFunc[Func_Camera_getMacAddress].function_pointer)(hCamera);
}

SVGigE_RETURN
Camera_setIPAddress(Camera_handle hCamera,
                    unsigned int IPAddress,
                    unsigned int SubnetMask)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setIPAddress)
  GigEFunc[Func_Camera_setIPAddress].function_pointer)(hCamera, IPAddress, SubnetMask);
}

char *
Camera_getIPAddress(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return "";

  // Pass through function call to DLL
  return ((TFunc_Camera_getIPAddress)
  GigEFunc[Func_Camera_getIPAddress].function_pointer)(hCamera);
}

SVGigE_RETURN
Camera_evaluateMaximalPacketSize(Camera_handle hCamera,
                                 int *MaximalPacketSize)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_evaluateMaximalPacketSize)
  GigEFunc[Func_Camera_evaluateMaximalPacketSize].function_pointer)(hCamera, MaximalPacketSize);
}

SVGigE_RETURN
Camera_setStreamingPacketSize(Camera_handle hCamera,
                              int StreamingPacketSize)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setStreamingPacketSize)
  GigEFunc[Func_Camera_setStreamingPacketSize].function_pointer)(hCamera, StreamingPacketSize);
}

SVGigE_RETURN
Camera_getImagerWidth(Camera_handle hCamera,
                      int *ImagerWidth)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getImagerWidth)
  GigEFunc[Func_Camera_getImagerWidth].function_pointer)(hCamera, ImagerWidth);
}

SVGigE_RETURN
Camera_getImagerHeight(Camera_handle hCamera,
                       int *ImagerHeight)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getImagerHeight)
  GigEFunc[Func_Camera_getImagerHeight].function_pointer)(hCamera, ImagerHeight);
}

SVGigE_RETURN
Camera_getSizeX(Camera_handle hCamera,
                int *SizeX)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getSizeX)
  GigEFunc[Func_Camera_getSizeX].function_pointer)(hCamera, SizeX);
}

SVGigE_RETURN
Camera_getSizeY(Camera_handle hCamera,
                int *SizeY)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getSizeY)
  GigEFunc[Func_Camera_getSizeY].function_pointer)(hCamera, SizeY);
}

SVGigE_RETURN
Camera_getPitch(Camera_handle hCamera,
                int *Pitch)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getPitch)
  GigEFunc[Func_Camera_getPitch].function_pointer)(hCamera, Pitch);
}

SVGigE_RETURN
Camera_getImageSize(Camera_handle hCamera,
                    int *ImageSize)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getImageSize)
  GigEFunc[Func_Camera_getImageSize].function_pointer)(hCamera, ImageSize);
}

SVGigE_RETURN
Camera_getPixelClock(Camera_handle hCamera,
                    int *PixelClock)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getPixelClock)
  GigEFunc[Func_Camera_getPixelClock].function_pointer)(hCamera, PixelClock);
}

SVGigE_RETURN
Camera_getPixelType(Camera_handle hCamera,
                    GVSP_PIXEL_TYPE *PixelType)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getPixelType)
  GigEFunc[Func_Camera_getPixelType].function_pointer)(hCamera, PixelType);
}

SVGigE_RETURN
Camera_setPixelDepth(Camera_handle hCamera,
                     SVGIGE_PIXEL_DEPTH PixelDepth)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setPixelDepth)
  GigEFunc[Func_Camera_setPixelDepth].function_pointer)(hCamera, PixelDepth);
}

SVGigE_RETURN
Camera_getPixelDepth(Camera_handle hCamera,
                     SVGIGE_PIXEL_DEPTH *PixelDepth)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getPixelDepth)
  GigEFunc[Func_Camera_getPixelDepth].function_pointer)(hCamera, PixelDepth);
}

bool
Camera_isCameraFeature(Camera_handle hCamera,
                       CAMERA_FEATURE Feature)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return false;

  // Pass through function call to DLL
  return ((TFunc_Camera_isCameraFeature)
  GigEFunc[Func_Camera_isCameraFeature].function_pointer)(hCamera, Feature);
}

SVGigE_RETURN
Camera_readXMLFile(Camera_handle hCamera,
                   char **XML)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_readXMLFile)
  GigEFunc[Func_Camera_readXMLFile].function_pointer)(hCamera, XML);
}


SVGigE_RETURN
Camera_setFrameRate(Camera_handle hCamera,
                    float Framerate)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setFrameRate)
  GigEFunc[Func_Camera_setFrameRate].function_pointer)(hCamera, Framerate);
}

SVGigE_RETURN
Camera_getFrameRate(Camera_handle hCamera,
                    float *ProgrammedFramerate)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getFrameRate)
  GigEFunc[Func_Camera_getFrameRate].function_pointer)(hCamera, ProgrammedFramerate);
}

SVGigE_RETURN
Camera_getFrameRateRange(Camera_handle hCamera,
                         float *MinFramerate,
                         float *MaxFramerate)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getFrameRateRange)
  GigEFunc[Func_Camera_getFrameRateRange].function_pointer)(hCamera, MinFramerate, MaxFramerate);
}

SVGigE_RETURN
Camera_setExposureTime(Camera_handle hCamera,
                       float ExposureTime)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setExposureTime)
  GigEFunc[Func_Camera_setExposureTime].function_pointer)(hCamera, ExposureTime);
}

SVGigE_RETURN
Camera_getExposureTime(Camera_handle hCamera,
                       float *ProgrammedExposureTime)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getExposureTime)
  GigEFunc[Func_Camera_getExposureTime].function_pointer)(hCamera, ProgrammedExposureTime);
}

SVGigE_RETURN
Camera_getExposureTimeRange(Camera_handle hCamera,
                            float *MinExposureTime,
                            float *MaxExposureTime)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getExposureTimeRange)
  GigEFunc[Func_Camera_getExposureTimeRange].function_pointer)(hCamera, MinExposureTime, MaxExposureTime);
}

SVGigE_RETURN
Camera_setGain(Camera_handle hCamera,
               float Gain)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setGain)
  GigEFunc[Func_Camera_setGain].function_pointer)(hCamera, Gain);
}

SVGigE_RETURN
Camera_getGain(Camera_handle hCamera,
               float *ProgrammedGain)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getGain)
  GigEFunc[Func_Camera_getGain].function_pointer)(hCamera, ProgrammedGain);
}

SVGigE_RETURN
Camera_setOffset(Camera_handle hCamera,
                 float Offset)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setOffset)
  GigEFunc[Func_Camera_setOffset].function_pointer)(hCamera, Offset);
}

SVGigE_RETURN
Camera_getOffset(Camera_handle hCamera,
                 float *ProgrammedOffset)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getOffset)
  GigEFunc[Func_Camera_getOffset].function_pointer)(hCamera, ProgrammedOffset);
}

SVGigE_RETURN
Camera_setWhiteBalance(Camera_handle hCamera,
                       float Red,
                       float Green,
                       float Blue)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setWhiteBalance)
  GigEFunc[Func_Camera_setWhiteBalance].function_pointer)(hCamera, Red, Green, Blue);
}

SVGigE_RETURN
Camera_getWhiteBalance(Camera_handle hCamera,
                       float *Red,
                       float *Green,
                       float *Blue)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getWhiteBalance)
  GigEFunc[Func_Camera_getWhiteBalance].function_pointer)(hCamera, Red, Green, Blue);
}

SVGigE_RETURN
Camera_setLookupTableMode(Camera_handle hCamera,
                          LUT_MODE LUTMode)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setLookupTableMode)
  GigEFunc[Func_Camera_setLookupTableMode].function_pointer)(hCamera, LUTMode);
}

SVGigE_RETURN
Camera_getLookupTableMode(Camera_handle hCamera,
                          LUT_MODE *ProgrammedLUTMode)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getLookupTableMode)
  GigEFunc[Func_Camera_getLookupTableMode].function_pointer)(hCamera, ProgrammedLUTMode);
}

SVGigE_RETURN
Camera_setLookupTable(Camera_handle hCamera,
                      unsigned char *LookupTable,
                      int LookupTableSize)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setLookupTable)
  GigEFunc[Func_Camera_setLookupTable].function_pointer)(hCamera, LookupTable, LookupTableSize);
}

SVGigE_RETURN
Camera_getLookupTable(Camera_handle hCamera,
                      unsigned char *LookupTable,
                      int LookupTableSize)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getLookupTable)
  GigEFunc[Func_Camera_getLookupTable].function_pointer)(hCamera, LookupTable, LookupTableSize);
}

//----DEPRECATED----
SVGigE_RETURN
Camera_createLUTwhiteBalance(Camera_handle hCamera,
                             float Red,
                             float Green ,
                             float Blue)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_createLUTwhiteBalance)
  GigEFunc[Func_Camera_createLUTwhiteBalance].function_pointer)(hCamera, Red, Green, Blue);
}

SVGigE_RETURN
Camera_setBinningMode(Camera_handle hCamera,
                      BINNING_MODE BinningMode)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setBinningMode)
  GigEFunc[Func_Camera_setBinningMode].function_pointer)(hCamera, BinningMode);
}

SVGigE_RETURN
Camera_getBinningMode(Camera_handle hCamera,
                      BINNING_MODE *ProgrammedBinningMode)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getBinningMode)
  GigEFunc[Func_Camera_getBinningMode].function_pointer)(hCamera, ProgrammedBinningMode);
}

SVGigE_RETURN
Camera_setAreaOfInterest(Camera_handle hCamera,
                         int SizeX,
                         int SizeY,
                         int OffsetX,
                         int OffsetY)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setAreaOfInterest)
  GigEFunc[Func_Camera_setAreaOfInterest].function_pointer)(hCamera, SizeX, SizeY, OffsetX, OffsetY);
}

SVGigE_RETURN
Camera_getAreaOfInterest(Camera_handle hCamera,
                         int *ProgrammedSizeX,
                         int *ProgrammedSizeY,
                         int *ProgrammedOffsetX,
                         int *ProgrammedOffsetY)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getAreaOfInterest)
  GigEFunc[Func_Camera_getAreaOfInterest].function_pointer)(hCamera, ProgrammedSizeX, ProgrammedSizeY, ProgrammedOffsetX, ProgrammedOffsetY);
}

SVGigE_RETURN 
Camera_saveTapBalanceSettings(Camera_handle hCamera,
                              char *Filename)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_saveTapBalanceSettings)
  GigEFunc[Func_Camera_saveTapBalanceSettings].function_pointer)(hCamera, Filename);
}

SVGigE_RETURN 
Camera_loadTapBalanceSettings(Camera_handle hCamera,
                              char *Filename)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_loadTapBalanceSettings)
  GigEFunc[Func_Camera_loadTapBalanceSettings].function_pointer)(hCamera, Filename);
}

SVGigE_RETURN 
Camera_setTapCalibration(Camera_handle hCamera,
                         unsigned int TapID, 
                         unsigned int Gain, 
                         unsigned int Offset)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setTapCalibration)
  GigEFunc[Func_Camera_setTapCalibration].function_pointer)(hCamera, TapID, Gain, Offset);
}

SVGigE_RETURN 
Camera_getTapCalibration(Camera_handle hCamera,
                         unsigned int TapID, 
                         unsigned int *Gain, 
                         unsigned int *Offset)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getTapCalibration)
  GigEFunc[Func_Camera_getTapCalibration].function_pointer)(hCamera, TapID, Gain, Offset);
}

SVGigE_RETURN 
Camera_setTapConfiguration(Camera_handle hCamera,
                           int TapCount)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setTapConfiguration)
  GigEFunc[Func_Camera_setTapConfiguration].function_pointer)(hCamera, TapCount);
}

SVGigE_RETURN 
Camera_getTapConfiguration(Camera_handle hCamera,
                           int *TapCount)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getTapConfiguration)
  GigEFunc[Func_Camera_getTapConfiguration].function_pointer)(hCamera, TapCount);
}

SVGigE_RETURN
Camera_setAcquisitionMode(Camera_handle hCamera,
                          ACQUISITION_MODE AcquisitionMode)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setAcquisitionMode)
  GigEFunc[Func_Camera_setAcquisitionMode].function_pointer)(hCamera, AcquisitionMode);
}

SVGigE_RETURN
Camera_setAcquisitionModeAndStart(Camera_handle hCamera,
                                  ACQUISITION_MODE AcquisitionMode,
                                  bool AcquisitionStart)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setAcquisitionModeAndStart)
  GigEFunc[Func_Camera_setAcquisitionModeAndStart].function_pointer)(hCamera, AcquisitionMode, AcquisitionStart);
}

SVGigE_RETURN
Camera_getAcquisitionMode(Camera_handle hCamera,
                          ACQUISITION_MODE *ProgrammedAcquisitionMode)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getAcquisitionMode)
  GigEFunc[Func_Camera_getAcquisitionMode].function_pointer)(hCamera, ProgrammedAcquisitionMode);
}

SVGigE_RETURN
Camera_setAcquisitionControl(Camera_handle hCamera,
                             ACQUISITION_CONTROL AcquisitionControl)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setAcquisitionControl)
  GigEFunc[Func_Camera_setAcquisitionControl].function_pointer)(hCamera, AcquisitionControl);
}

SVGigE_RETURN
Camera_getAcquisitionControl(Camera_handle hCamera,
                             ACQUISITION_CONTROL *ProgrammedAcquisitionControl)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getAcquisitionControl)
  GigEFunc[Func_Camera_getAcquisitionControl].function_pointer)(hCamera, ProgrammedAcquisitionControl);
}

SVGigE_RETURN
Camera_startAcquisitionCycle(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_startAcquisitionCycle)
  GigEFunc[Func_Camera_startAcquisitionCycle].function_pointer)(hCamera);
}

SVGigE_RETURN
Camera_setTriggerPolarity(Camera_handle hCamera,
                          TRIGGER_POLARITY TriggerPolarity)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setTriggerPolarity)
  GigEFunc[Func_Camera_setTriggerPolarity].function_pointer)(hCamera, TriggerPolarity);
}

SVGigE_RETURN
Camera_getTriggerPolarity(Camera_handle hCamera,
                          TRIGGER_POLARITY *ProgrammedTriggerPolarity)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getTriggerPolarity)
  GigEFunc[Func_Camera_getTriggerPolarity].function_pointer)(hCamera, ProgrammedTriggerPolarity);
}

SVGigE_RETURN
Camera_setExposureDelay(Camera_handle hCamera,
                         float ExposureDelay)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setExposureDelay)
  GigEFunc[Func_Camera_setExposureDelay].function_pointer)(hCamera, ExposureDelay);
}

SVGigE_RETURN
Camera_getExposureDelay(Camera_handle hCamera,
                        float *ProgrammedExposureDelay)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getExposureDelay)
  GigEFunc[Func_Camera_getExposureDelay].function_pointer)(hCamera, ProgrammedExposureDelay);
}

SVGigE_RETURN
Camera_setInterPacketDelay(Camera_handle hCamera,
                           float InterPacketDelay)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setInterPacketDelay)
  GigEFunc[Func_Camera_setInterPacketDelay].function_pointer)(hCamera, InterPacketDelay);
}

SVGigE_RETURN 
Camera_getInterPacketDelay(Camera_handle hCamera,
                           float *ProgrammedInterPacketDelay)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getInterPacketDelay)
  GigEFunc[Func_Camera_getInterPacketDelay].function_pointer)(hCamera, ProgrammedInterPacketDelay);
}


SVGigE_RETURN
Camera_setStrobePolarity(Camera_handle hCamera,
                         STROBE_POLARITY StrobePolarity)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setStrobePolarity)
  GigEFunc[Func_Camera_setStrobePolarity].function_pointer)(hCamera, StrobePolarity);
}

SVGigE_RETURN
Camera_getStrobePolarity(Camera_handle hCamera,
                         STROBE_POLARITY *ProgrammedStrobePolarity)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getStrobePolarity)
  GigEFunc[Func_Camera_getStrobePolarity].function_pointer)(hCamera, ProgrammedStrobePolarity);
}

SVGigE_RETURN
Camera_setStrobePosition(Camera_handle hCamera,
                         float StrobePosition)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setStrobePosition)
  GigEFunc[Func_Camera_setStrobePosition].function_pointer)(hCamera, StrobePosition);
}

SVGigE_RETURN
Camera_getStrobePosition(Camera_handle hCamera,
                         float *ProgrammedStrobePosition)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getStrobePosition)
  GigEFunc[Func_Camera_getStrobePosition].function_pointer)(hCamera, ProgrammedStrobePosition);
}

SVGigE_RETURN
Camera_setStrobeDuration(Camera_handle hCamera,
                         float StrobeDuration)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setStrobeDuration)
  GigEFunc[Func_Camera_setStrobeDuration].function_pointer)(hCamera, StrobeDuration);
}

SVGigE_RETURN
Camera_getStrobeDuration(Camera_handle hCamera,
                         float *ProgrammedStrobeDuration)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getStrobeDuration)
  GigEFunc[Func_Camera_getStrobeDuration].function_pointer)(hCamera, ProgrammedStrobeDuration);
}

SVGigE_RETURN
Camera_writeEEPROM(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_writeEEPROM)
  GigEFunc[Func_Camera_writeEEPROM].function_pointer)(hCamera);
}

SVGigE_RETURN
Camera_readEEPROM(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_readEEPROM)
  GigEFunc[Func_Camera_readEEPROM].function_pointer)(hCamera);
}

SVGigE_RETURN
Camera_restoreFactoryDefaults(Camera_handle hCamera)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_restoreFactoryDefaults)
  GigEFunc[Func_Camera_restoreFactoryDefaults].function_pointer)(hCamera);
}

SVGigE_RETURN
Camera_stampTimestamp(Camera_handle hCamera,
                      int TimestampIndex)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_stampTimestamp)
  GigEFunc[Func_Camera_stampTimestamp].function_pointer)(hCamera, TimestampIndex);
}

SVGigE_RETURN
Camera_getTimestamp(Camera_handle hCamera,
                    int TimestampIndex,
                    double *Timestamp)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getTimestamp)
  GigEFunc[Func_Camera_getTimestamp].function_pointer)(hCamera, TimestampIndex, Timestamp);
}

SVGigE_RETURN
Camera_setGigECameraRegister(Camera_handle hCamera,
                             unsigned int RegisterAddress,
                             unsigned int RegisterValue,
                             unsigned int GigECameraAccessKey)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_setGigECameraRegister)
  GigEFunc[Func_Camera_setGigECameraRegister].function_pointer)(hCamera, RegisterAddress, RegisterValue, GigECameraAccessKey);
}

SVGigE_RETURN
Camera_getGigECameraRegister(Camera_handle hCamera,
                             unsigned int RegisterAddress,
                             unsigned int *ProgramedRegisterValue,
                             unsigned int GigECameraAccessKey)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_getGigECameraRegister)
  GigEFunc[Func_Camera_getGigECameraRegister].function_pointer)(hCamera, RegisterAddress, ProgramedRegisterValue, GigECameraAccessKey);
}

SVGigE_RETURN
Camera_writeGigECameraMemory(Camera_handle hCamera,
                             unsigned int  MemoryAddress,
                             unsigned char *DataBlock,
                             unsigned int  DataLength,
                             unsigned int  GigECameraAccessKey)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_writeGigECameraMemory)
  GigEFunc[Func_Camera_writeGigECameraMemory].function_pointer)(hCamera, MemoryAddress, DataBlock, DataLength, GigECameraAccessKey);
}

SVGigE_RETURN
Camera_readGigECameraMemory(Camera_handle hCamera,
                            unsigned int  MemoryAddress,
                            unsigned char *DataBlock,
                            unsigned int  DataLength,
                            unsigned int  GigECameraAccessKey)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_readGigECameraMemory)
  GigEFunc[Func_Camera_readGigECameraMemory].function_pointer)(hCamera, MemoryAddress, DataBlock, DataLength, GigECameraAccessKey);
}

SVGigE_RETURN
Camera_registerForLogMessages(Camera_handle hCamera,
                              int  LogLevel,
                              char *LogFilename,
                              LogMessageCallback LogCallback,
                              void *MessageContext)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Camera_registerForLogMessages)
  GigEFunc[Func_Camera_registerForLogMessages].function_pointer)(hCamera, LogLevel, LogFilename, LogCallback, MessageContext);
}

SVGigE_RETURN
StreamingChannel_create(StreamingChannel_handle *hStreamingChannel,
                        CameraContainerClient_handle hCameraContainer,
                        Camera_handle hCamera,
                        int BufferCount,
                        StreamCallback CallbackFunction,
                        void *Context)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_StreamingChannel_create)
  GigEFunc[Func_StreamingChannel_create].function_pointer)(hStreamingChannel, hCameraContainer, hCamera, BufferCount, CallbackFunction, Context);
}

SVGigE_RETURN
StreamingChannel_delete(StreamingChannel_handle hStreamingChannel)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_StreamingChannel_delete)
  GigEFunc[Func_StreamingChannel_delete].function_pointer)(hStreamingChannel);
}

SVGigE_RETURN
StreamingChannel_getFrameLoss(StreamingChannel_handle hStreamingChannel,
                              int *FrameLoss)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_StreamingChannel_getFrameLoss)
  GigEFunc[Func_StreamingChannel_getFrameLoss].function_pointer)(hStreamingChannel, FrameLoss);
}

SVGigE_RETURN
StreamingChannel_getActualFrameRate(StreamingChannel_handle hStreamingChannel,
                                    float *ActualFrameRate)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_StreamingChannel_getActualFrameRate)
  GigEFunc[Func_StreamingChannel_getActualFrameRate].function_pointer)(hStreamingChannel, ActualFrameRate);
}

SVGigE_RETURN
StreamingChannel_getActualDataRate(StreamingChannel_handle hStreamingChannel,
                                   float *ActualDataRate)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_StreamingChannel_getActualDataRate)
  GigEFunc[Func_StreamingChannel_getActualDataRate].function_pointer)(hStreamingChannel, ActualDataRate);
}

SVGigE_RETURN
Image_release(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Image_release)
  GigEFunc[Func_Image_release].function_pointer)(hImage);
}

unsigned char *
Image_getDataPointer(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return NULL;

  // Pass through function call to DLL
  return ((TFunc_Image_getDataPointer)
  GigEFunc[Func_Image_getDataPointer].function_pointer)(hImage);
}

Camera_handle 
Image_getCamera(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_NO_CAMERA;

  // Pass through function call to DLL
  return ((TFunc_Image_getCamera)
  GigEFunc[Func_Image_getCamera].function_pointer)(hImage);
}

int
Image_getBufferIndex(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return NULL;

  // Pass through function call to DLL
  return ((TFunc_Image_getBufferIndex)
  GigEFunc[Func_Image_getBufferIndex].function_pointer)(hImage);
}

SVGigE_RETURN
Image_getImageRGB(Image_handle hImage,
                  unsigned char *BufferRGB,
                  int BufferLength,
                  BAYER_METHOD BayerMethod)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Image_getImageRGB)
  GigEFunc[Func_Image_getImageRGB].function_pointer)(hImage, BufferRGB, BufferLength, BayerMethod);
}

SVGigE_RETURN
Image_getImage12bitAs8bit(Image_handle hImage,
                          unsigned char *Buffer8bit,
                          int BufferLength)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Image_getImage12bitAs8bit)
  GigEFunc[Func_Image_getImage12bitAs8bit].function_pointer)(hImage, Buffer8bit, BufferLength);
}

SVGigE_RETURN
Image_getImage12bitAs16bit(Image_handle hImage,
                           unsigned char *Buffer16bit,
                           int BufferLength)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Image_getImage12bitAs16bit)
  GigEFunc[Func_Image_getImage12bitAs16bit].function_pointer)(hImage, Buffer16bit, BufferLength);
}

SVGigE_RETURN
Image_getImage16bitAs8bit(Image_handle hImage,
                          unsigned char *Buffer8bit,
                          int BufferLength)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_Image_getImage16bitAs8bit)
  GigEFunc[Func_Image_getImage16bitAs8bit].function_pointer)(hImage, Buffer8bit, BufferLength);
}

int
Image_getImageSize(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return 0;

  // Pass through function call to DLL
  return ((TFunc_Image_getImageSize)
  GigEFunc[Func_Image_getImageSize].function_pointer)(hImage);
}

int
Image_getImageID(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return 0;

  // Pass through function call to DLL
  return ((TFunc_Image_getImageID)
  GigEFunc[Func_Image_getImageID].function_pointer)(hImage);
}

double
Image_getTimestamp(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return 0;

  // Pass through function call to DLL
  return ((TFunc_Image_getTimestamp)
  GigEFunc[Func_Image_getTimestamp].function_pointer)(hImage);
}

int
Image_getSizeX(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return 0;

  // Pass through function call to DLL
  return ((TFunc_Image_getSizeX)
  GigEFunc[Func_Image_getSizeX].function_pointer)(hImage);
}

int
Image_getSizeY(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return 0;

  // Pass through function call to DLL
  return ((TFunc_Image_getSizeY)
  GigEFunc[Func_Image_getSizeY].function_pointer)(hImage);
}

int
Image_getPitch(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return 0;

  // Pass through function call to DLL
  return ((TFunc_Image_getPitch)
  GigEFunc[Func_Image_getPitch].function_pointer)(hImage);
}

GVSP_PIXEL_TYPE
Image_getPixelType(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return GVSP_PIX_MONO8;

  // Pass through function call to DLL
  return ((TFunc_Image_getPixelType)
  GigEFunc[Func_Image_getPixelType].function_pointer)(hImage);
}

int
Image_getPacketCount(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return 0;

  // Pass through function call to DLL
  return ((TFunc_Image_getPacketCount)
  GigEFunc[Func_Image_getPacketCount].function_pointer)(hImage);
}

int
Image_getPacketResend(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return 0;

  // Pass through function call to DLL
  return ((TFunc_Image_getPacketResend)
  GigEFunc[Func_Image_getPacketResend].function_pointer)(hImage);
}

SVGigE_SIGNAL_TYPE
Image_getSignalType(Image_handle hImage)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_SIGNAL_NONE;

  // Pass through function call to DLL
  return ((TFunc_Image_getSignalType)
  GigEFunc[Func_Image_getSignalType].function_pointer)(hImage);
}

SVGigE_RETURN 
SVGigE_writeImageToBitmapFile(char *Filename, unsigned char *Data, int SizeX, int SizeY, GVSP_PIXEL_TYPE PixelType)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_SVGigE_writeImageToBitmapFile)
  GigEFunc[Func_SVGigE_writeImageToBitmapFile].function_pointer)(Filename, Data, SizeX, SizeY, PixelType);
}

SVGigE_RETURN 
SVGigE_estimateWhiteBalance(unsigned char *DataRGB, int DataRGBLength, float *Red, float *Green, float *Blue)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return SVGigE_DLL_NOT_LOADED;

  // Pass through function call to DLL
  return ((TFunc_SVGigE_estimateWhiteBalance)
  GigEFunc[Func_SVGigE_estimateWhiteBalance].function_pointer)(DataRGB, DataRGBLength, Red, Green, Blue);
}

char *
Error_getMessage(SVGigE_RETURN ReturnCode)
{
  // Check DLL availability
  if( NULL == GigEDLL )
    return "";

  // Pass through function call to DLL
  return ((TFunc_Error_getMessage)
  GigEFunc[Func_Error_getMessage].function_pointer)(ReturnCode);
}

/**
 * 1394-Based Digital Camera Control Library
 * Bayer pattern decoding functions
 * Copyright (C) Damien Douxchamps <ddouxchamps@users.sf.net>
 *
 * Written by Damien Douxchamps and Frederic Devernay
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#define CLIP(in, out)\
   in = in < 0 ? 0 : in;\
   in = in > 255 ? 255 : in;\
   out=in;
  
#define CLIP16(in, out, bits)\
   in = in < 0 ? 0 : in;\
   in = in > ((1<<bits)-1) ? ((1<<bits)-1) : in;\
   out=in;
  
void
ClearBorders(unsigned char *rgb, int sx, int sy, int w)
{
    int i, j;
    // black edges are added with a width w:
    i = 3 * sx * w - 1;
    j = 3 * sx * sy - 1;
    while (i >= 0) {
	rgb[i--] = 0;
	rgb[j--] = 0;
    }
    i = sx * (sy - 1) * 3 - 1 + w * 3;
    while (i > sx) {
	j = 6 * w;
	while (j > 0) {
	    rgb[i--] = 0;
	    j--;
	}
	i -= (sx - 2 * w) * 3;
    }
}

void
ClearBorders_uint16(uint16_t * rgb, int sx, int sy, int w)
{
    int i, j;

    // black edges:
    i = 3 * sx * w - 1;
    j = 3 * sx * sy - 1;
    while (i >= 0) {
	rgb[i--] = 0;
	rgb[j--] = 0;
    }

    i = sx * (sy - 1) * 3 - 1 + w * 3;
    while (i > sx) {
	j = 6 * w;
	while (j > 0) {
	    rgb[i--] = 0;
	    j--;
	}
	i -= (sx - 2 * w) * 3;
    }

}

/**************************************************************
 *     Color conversion functions for cameras that can        * 
 * output raw-Bayer pattern images, such as some Basler and   *
 * Point Grey camera. Most of the algos presented here come   *
 * from http://www-ise.stanford.edu/~tingchen/ and have been  *
 * converted from Matlab to C and extended to all elementary  *
 * patterns.                                                  *
 **************************************************************/

/* 8-bits versions */
/* insprired by OpenCV's Bayer decoding */

void
dc1394_bayer_NearestNeighbor(const unsigned char *bayer, unsigned char *rgb, int sx, int sy, int tile)
{
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == DC1394_COLOR_FILTER_BGGR
	|| tile == DC1394_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == DC1394_COLOR_FILTER_GBRG
	|| tile == DC1394_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    /* add black border */
    imax = sx * sy * 3;
    for (i = sx * (sy - 1) * 3; i < imax; i++) {
	rgb[i] = 0;
    }
    iinc = (sx - 1) * 3;
    for (i = (sx - 1) * 3; i < imax; i += iinc) {
	rgb[i++] = 0;
	rgb[i++] = 0;
	rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
      //int t0, t1;
	const unsigned char *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[1];
            rgb[0] = bayer[bayerStep + 1];
            rgb[blue] = bayer[bayerStep];
            bayer++;
            rgb += 3;
        }

        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                rgb[-1] = bayer[0];
                rgb[0] = bayer[1];
                rgb[1] = bayer[bayerStep + 1];

                rgb[2] = bayer[2];
                rgb[3] = bayer[bayerStep + 2];
                rgb[4] = bayer[bayerStep + 1];
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                rgb[1] = bayer[0];
                rgb[0] = bayer[1];
                rgb[-1] = bayer[bayerStep + 1];

                rgb[4] = bayer[2];
                rgb[3] = bayer[bayerStep + 2];
                rgb[2] = bayer[bayerStep + 1];
            }
        }

        if (bayer < bayerEnd) {
            rgb[-blue] = bayer[0];
            rgb[0] = bayer[1];
            rgb[blue] = bayer[bayerStep + 1];
            bayer++;
            rgb += 3;
        }

	bayer -= width;
	rgb -= width * 3;

	blue = -blue;
	start_with_green = !start_with_green;
    }
}

/* OpenCV's Bayer decoding */
void
dc1394_bayer_Bilinear(const unsigned char *bayer, unsigned char *rgb, int sx, int sy, int tile)
{
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    /*
       the two letters  of the OpenCV name are respectively
       the 4th and 3rd letters from the blinky name,
       and we also have to switch R and B (OpenCV is BGR)

       CV_BayerBG2BGR <-> DC1394_COLOR_FILTER_BGGR
       CV_BayerGB2BGR <-> DC1394_COLOR_FILTER_GBRG
       CV_BayerGR2BGR <-> DC1394_COLOR_FILTER_GRBG

       int blue = tile == CV_BayerBG2BGR || tile == CV_BayerGB2BGR ? -1 : 1;
       int start_with_green = tile == CV_BayerGB2BGR || tile == CV_BayerGR2BGR;
     */
    int blue = tile == DC1394_COLOR_FILTER_BGGR
	|| tile == DC1394_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == DC1394_COLOR_FILTER_GBRG
	|| tile == DC1394_COLOR_FILTER_GRBG;

    ClearBorders(rgb, sx, sy, 1);
    rgb += rgbStep + 3 + 1;
    height -= 2;
    width -= 2;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
	int t0, t1;
	const unsigned char *bayerEnd = bayer + width;

	if (start_with_green) {
	    /* OpenCV has a bug in the next line, which was
	       t0 = (bayer[0] + bayer[bayerStep * 2] + 1) >> 1; */
	    t0 = (bayer[1] + bayer[bayerStep * 2 + 1] + 1) >> 1;
	    t1 = (bayer[bayerStep] + bayer[bayerStep + 2] + 1) >> 1;
	    rgb[-blue] = (unsigned char) t0;
	    rgb[0] = bayer[bayerStep + 1];
	    rgb[blue] = (unsigned char) t1;
	    bayer++;
	    rgb += 3;
	}

	if (blue > 0) {
	    for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
		t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
		      bayer[bayerStep * 2 + 2] + 2) >> 2;
		t1 = (bayer[1] + bayer[bayerStep] +
		      bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
		      2) >> 2;
		rgb[-1] = (unsigned char) t0;
		rgb[0] = (unsigned char) t1;
		rgb[1] = bayer[bayerStep + 1];

		t0 = (bayer[2] + bayer[bayerStep * 2 + 2] + 1) >> 1;
		t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
		      1) >> 1;
		rgb[2] = (unsigned char) t0;
		rgb[3] = bayer[bayerStep + 2];
		rgb[4] = (unsigned char) t1;
	    }
	} else {
	    for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
		t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
		      bayer[bayerStep * 2 + 2] + 2) >> 2;
		t1 = (bayer[1] + bayer[bayerStep] +
		      bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
		      2) >> 2;
		rgb[1] = (unsigned char) t0;
		rgb[0] = (unsigned char) t1;
		rgb[-1] = bayer[bayerStep + 1];

		t0 = (bayer[2] + bayer[bayerStep * 2 + 2] + 1) >> 1;
		t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
		      1) >> 1;
		rgb[4] = (unsigned char) t0;
		rgb[3] = bayer[bayerStep + 2];
		rgb[2] = (unsigned char) t1;
	    }
	}

	if (bayer < bayerEnd) {
	    t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
		  bayer[bayerStep * 2 + 2] + 2) >> 2;
	    t1 = (bayer[1] + bayer[bayerStep] +
		  bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
		  2) >> 2;
	    rgb[-blue] = (unsigned char) t0;
	    rgb[0] = (unsigned char) t1;
	    rgb[blue] = bayer[bayerStep + 1];
	    bayer++;
	    rgb += 3;
	}

	bayer -= width;
	rgb -= width * 3;

	blue = -blue;
	start_with_green = !start_with_green;
    }
}

/* High-Quality Linear Interpolation For Demosaicing Of
   Bayer-Patterned Color Images, by Henrique S. Malvar, Li-wei He, and
   Ross Cutler, in ICASSP'04 */
void
dc1394_bayer_HQLinear(const unsigned char *bayer, unsigned char *rgb, int sx, int sy, int tile)
{
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == DC1394_COLOR_FILTER_BGGR
	|| tile == DC1394_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == DC1394_COLOR_FILTER_GBRG
	|| tile == DC1394_COLOR_FILTER_GRBG;

    ClearBorders(rgb, sx, sy, 2);
    rgb += 2 * rgbStep + 6 + 1;
    height -= 4;
    width -= 4;

    /* We begin with a (+1 line,+1 column) offset with respect to bilinear decoding, so start_with_green is the same, but blue is opposite */
    blue = -blue;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
	int t0, t1;
	const unsigned char *bayerEnd = bayer + width;
	const int bayerStep2 = bayerStep * 2;
	const int bayerStep3 = bayerStep * 3;
	const int bayerStep4 = bayerStep * 4;

	if (start_with_green) {
	    /* at green pixel */
	    rgb[0] = bayer[bayerStep2 + 2];
	    t0 = rgb[0] * 5
		+ ((bayer[bayerStep + 2] + bayer[bayerStep3 + 2]) << 2)
		- bayer[2]
		- bayer[bayerStep + 1]
		- bayer[bayerStep + 3]
		- bayer[bayerStep3 + 1]
		- bayer[bayerStep3 + 3]
		- bayer[bayerStep4 + 2]
		+ ((bayer[bayerStep2] + bayer[bayerStep2 + 4] + 1) >> 1);
	    t1 = rgb[0] * 5 +
		((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 3]) << 2)
		- bayer[bayerStep2]
		- bayer[bayerStep + 1]
		- bayer[bayerStep + 3]
		- bayer[bayerStep3 + 1]
		- bayer[bayerStep3 + 3]
		- bayer[bayerStep2 + 4]
		+ ((bayer[2] + bayer[bayerStep4 + 2] + 1) >> 1);
	    t0 = (t0 + 4) >> 3;
	    CLIP(t0, rgb[-blue]);
	    t1 = (t1 + 4) >> 3;
	    CLIP(t1, rgb[blue]);
	    bayer++;
	    rgb += 3;
	}

	if (blue > 0) {
	    for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
		/* B at B */
		rgb[1] = bayer[bayerStep2 + 2];
		/* R at B */
		t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
		       bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
		    -
		    (((bayer[2] + bayer[bayerStep2] +
		       bayer[bayerStep2 + 4] + bayer[bayerStep4 +
						     2]) * 3 + 1) >> 1)
		    + rgb[1] * 6;
		/* G at B */
		t1 = ((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
		       bayer[bayerStep2 + 3] + bayer[bayerStep3 + 2]) << 1)
		    - (bayer[2] + bayer[bayerStep2] +
		       bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
		    + (rgb[1] << 2);
		t0 = (t0 + 4) >> 3;
		CLIP(t0, rgb[-1]);
		t1 = (t1 + 4) >> 3;
		CLIP(t1, rgb[0]);
		/* at green pixel */
		rgb[3] = bayer[bayerStep2 + 3];
		t0 = rgb[3] * 5
		    + ((bayer[bayerStep + 3] + bayer[bayerStep3 + 3]) << 2)
		    - bayer[3]
		    - bayer[bayerStep + 2]
		    - bayer[bayerStep + 4]
		    - bayer[bayerStep3 + 2]
		    - bayer[bayerStep3 + 4]
		    - bayer[bayerStep4 + 3]
		    +
		    ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 5] +
		      1) >> 1);
		t1 = rgb[3] * 5 +
		    ((bayer[bayerStep2 + 2] + bayer[bayerStep2 + 4]) << 2)
		    - bayer[bayerStep2 + 1]
		    - bayer[bayerStep + 2]
		    - bayer[bayerStep + 4]
		    - bayer[bayerStep3 + 2]
		    - bayer[bayerStep3 + 4]
		    - bayer[bayerStep2 + 5]
		    + ((bayer[3] + bayer[bayerStep4 + 3] + 1) >> 1);
		t0 = (t0 + 4) >> 3;
		CLIP(t0, rgb[2]);
		t1 = (t1 + 4) >> 3;
		CLIP(t1, rgb[4]);
	    }
	} else {
	    for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
		/* R at R */
		rgb[-1] = bayer[bayerStep2 + 2];
		/* B at R */
		t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
		       bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
		    -
		    (((bayer[2] + bayer[bayerStep2] +
		       bayer[bayerStep2 + 4] + bayer[bayerStep4 +
						     2]) * 3 + 1) >> 1)
		    + rgb[-1] * 6;
		/* G at R */
		t1 = ((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
		       bayer[bayerStep2 + 3] + bayer[bayerStep * 3 +
						     2]) << 1)
		    - (bayer[2] + bayer[bayerStep2] +
		       bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
		    + (rgb[-1] << 2);
		t0 = (t0 + 4) >> 3;
		CLIP(t0, rgb[1]);
		t1 = (t1 + 4) >> 3;
		CLIP(t1, rgb[0]);

		/* at green pixel */
		rgb[3] = bayer[bayerStep2 + 3];
		t0 = rgb[3] * 5
		    + ((bayer[bayerStep + 3] + bayer[bayerStep3 + 3]) << 2)
		    - bayer[3]
		    - bayer[bayerStep + 2]
		    - bayer[bayerStep + 4]
		    - bayer[bayerStep3 + 2]
		    - bayer[bayerStep3 + 4]
		    - bayer[bayerStep4 + 3]
		    +
		    ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 5] +
		      1) >> 1);
		t1 = rgb[3] * 5 +
		    ((bayer[bayerStep2 + 2] + bayer[bayerStep2 + 4]) << 2)
		    - bayer[bayerStep2 + 1]
		    - bayer[bayerStep + 2]
		    - bayer[bayerStep + 4]
		    - bayer[bayerStep3 + 2]
		    - bayer[bayerStep3 + 4]
		    - bayer[bayerStep2 + 5]
		    + ((bayer[3] + bayer[bayerStep4 + 3] + 1) >> 1);
		t0 = (t0 + 4) >> 3;
		CLIP(t0, rgb[4]);
		t1 = (t1 + 4) >> 3;
		CLIP(t1, rgb[2]);
	    }
	}

	if (bayer < bayerEnd) {
	    /* B at B */
	    rgb[blue] = bayer[bayerStep2 + 2];
	    /* R at B */
	    t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
		   bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
		-
		(((bayer[2] + bayer[bayerStep2] +
		   bayer[bayerStep2 + 4] + bayer[bayerStep4 +
						 2]) * 3 + 1) >> 1)
		+ rgb[blue] * 6;
	    /* G at B */
	    t1 = (((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
		    bayer[bayerStep2 + 3] + bayer[bayerStep3 + 2])) << 1)
		- (bayer[2] + bayer[bayerStep2] +
		   bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
		+ (rgb[blue] << 2);
	    t0 = (t0 + 4) >> 3;
	    CLIP(t0, rgb[-blue]);
	    t1 = (t1 + 4) >> 3;
	    CLIP(t1, rgb[0]);
	    bayer++;
	    rgb += 3;
	}

	bayer -= width;
	rgb -= width * 3;

	blue = -blue;
	start_with_green = !start_with_green;
    }
}

/* coriander's Bayer decoding (GPL) */
/* Edge Sensing Interpolation II from http://www-ise.stanford.edu/~tingchen/ */
/*   (Laroche,Claude A.  "Apparatus and method for adaptively
     interpolating a full color image utilizing chrominance gradients"
     U.S. Patent 5,373,322) */
void
dc1394_bayer_EdgeSense(const unsigned char *bayer, unsigned char *rgb, int sx, int sy, int tile)
{
    unsigned char *outR, *outG, *outB;
    register int i, j;
    int dh, dv;
    int tmp;

    // sx and sy should be even
    switch (tile) {
    case DC1394_COLOR_FILTER_GRBG:
    case DC1394_COLOR_FILTER_BGGR:
	outR = &rgb[0];
	outG = &rgb[1];
	outB = &rgb[2];
	break;
    case DC1394_COLOR_FILTER_GBRG:
    case DC1394_COLOR_FILTER_RGGB:
	outR = &rgb[2];
	outG = &rgb[1];
	outB = &rgb[0];
	break;
    default:
 //	fprintf(stderr, "Bad bayer pattern ID: %d\n", tile);
 //	return;
	break;
    }

    switch (tile) {
    case DC1394_COLOR_FILTER_GRBG:	//---------------------------------------------------------
    case DC1394_COLOR_FILTER_GBRG:
	// copy original RGB data to output images
      for (i = 0; i < sy*sx; i += (sx<<1)) {
	for (j = 0; j < sx; j += 2) {
	  outG[(i + j) * 3] = bayer[i + j];
	  outG[(i + sx + (j + 1)) * 3] = bayer[i + sx + (j + 1)];
	  outR[(i + j + 1) * 3] = bayer[i + j + 1];
	  outB[(i + sx + j) * 3] = bayer[i + sx + j];
	}
      }
      // process GREEN channel
      for (i = 3*sx; i < (sy - 2)*sx; i += (sx<<1)) {
	for (j = 2; j < sx - 3; j += 2) {
	  dh = abs(((outB[(i + j - 2) * 3] +
		     outB[(i + j + 2) * 3]) >> 1) -
		   outB[(i + j) * 3]);
	  dv = abs(((outB[(i - (sx<<1) + j) * 3] +
		     outB[(i + (sx<<1) + j) * 3]) >> 1)  -
		   outB[(i + j) * 3]);
	  if (dh < dv)
	    tmp = (outG[(i + j - 1) * 3] +
		   outG[(i + j + 1) * 3]) >> 1;
	  else {
	    if (dh > dv)
	      tmp = (outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 1;
	    else
	      tmp = (outG[(i + j - 1) * 3] +
		     outG[(i + j + 1) * 3] +
		     outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 2;
	  }
	  CLIP(tmp, outG[(i + j) * 3]);
	}
      }
	
      for (i = 2*sx; i < (sy - 3)*sx; i += (sx<<1)) {
	for (j = 3; j < sx - 2; j += 2) {
	  dh = abs(((outR[(i + j - 2) * 3] +
		     outR[(i + j + 2) * 3]) >>1 ) -
		   outR[(i + j) * 3]);
	  dv = abs(((outR[(i - (sx<<1) + j) * 3] +
		     outR[(i + (sx<<1) + j) * 3]) >>1 ) -
		   outR[(i + j) * 3]);
	  if (dh < dv)
	    tmp = (outG[(i + j - 1) * 3] +
		   outG[(i + j + 1) * 3]) >> 1;
	  else {
	    if (dh > dv)
	      tmp = (outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 1;
	    else
	      tmp = (outG[(i + j - 1) * 3] +
		     outG[(i + j + 1) * 3] +
		     outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 2;
	  }
	  CLIP(tmp, outG[(i + j) * 3]);
	}
      }
      // process RED channel
      for (i = 0; i < (sy - 1)*sx; i += (sx<<1)) {
	for (j = 2; j < sx - 1; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i + j - 1) * 3] -
		outG[(i + j - 1) * 3] +
		outR[(i + j + 1) * 3] -
		outG[(i + j + 1) * 3]) >> 1);
	  CLIP(tmp, outR[(i + j) * 3]);
	}
      }
      for (i = sx; i < (sy - 2)*sx; i += (sx<<1)) {
	for (j = 1; j < sx; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i - sx + j) * 3] -
		outG[(i - sx + j) * 3] +
		outR[(i + sx + j) * 3] -
		outG[(i + sx + j) * 3]) >> 1);
	  CLIP(tmp, outR[(i + j) * 3]);
	}
	for (j = 2; j < sx - 1; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i - sx + j - 1) * 3] -
		outG[(i - sx + j - 1) * 3] +
		outR[(i - sx + j + 1) * 3] -
		outG[(i - sx + j + 1) * 3] +
		outR[(i + sx + j - 1) * 3] -
		outG[(i + sx + j - 1) * 3] +
		outR[(i + sx + j + 1) * 3] -
		outG[(i + sx + j + 1) * 3]) >> 2);
	  CLIP(tmp, outR[(i + j) * 3]);
	}
      }

      // process BLUE channel
      for (i = sx; i < sy*sx; i += (sx<<1)) {
	for (j = 1; j < sx - 2; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i + j - 1) * 3] -
		outG[(i + j - 1) * 3] +
		outB[(i + j + 1) * 3] -
		outG[(i + j + 1) * 3]) >> 1);
	  CLIP(tmp, outB[(i + j) * 3]);
	}
      }
      for (i = 2*sx; i < (sy - 1)*sx; i += (sx<<1)) {
	for (j = 0; j < sx - 1; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i - sx + j) * 3] -
		outG[(i - sx + j) * 3] +
		outB[(i + sx + j) * 3] -
		outG[(i + sx + j) * 3]) >> 1);
	  CLIP(tmp, outB[(i + j) * 3]);
	}
	for (j = 1; j < sx - 2; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i - sx + j - 1) * 3] -
		outG[(i - sx + j - 1) * 3] +
		outB[(i - sx + j + 1) * 3] -
		outG[(i - sx + j + 1) * 3] +
		outB[(i + sx + j - 1) * 3] -
		outG[(i + sx + j - 1) * 3] +
		outB[(i + sx + j + 1) * 3] -
		outG[(i + sx + j + 1) * 3]) >> 2);
	  CLIP(tmp, outB[(i + j) * 3]);
	}
      }
      break;

    case DC1394_COLOR_FILTER_BGGR:	//---------------------------------------------------------
    case DC1394_COLOR_FILTER_RGGB:
	// copy original RGB data to output images
      for (i = 0; i < sy*sx; i += (sx<<1)) {
	for (j = 0; j < sx; j += 2) {
	  outB[(i + j) * 3] = bayer[i + j];
	  outR[(i + sx + (j + 1)) * 3] = bayer[i + sx + (j + 1)];
	  outG[(i + j + 1) * 3] = bayer[i + j + 1];
	  outG[(i + sx + j) * 3] = bayer[i + sx + j];
	}
      }
      // process GREEN channel
      for (i = 2*sx; i < (sy - 2)*sx; i += (sx<<1)) {
	for (j = 2; j < sx - 3; j += 2) {
	  dh = abs(((outB[(i + j - 2) * 3] +
		    outB[(i + j + 2) * 3]) >> 1) -
		   outB[(i + j) * 3]);
	  dv = abs(((outB[(i - (sx<<1) + j) * 3] +
		    outB[(i + (sx<<1) + j) * 3]) >> 1) -
		   outB[(i + j) * 3]);
	  if (dh < dv)
	    tmp = (outG[(i + j - 1) * 3] +
		   outG[(i + j + 1) * 3]) >> 1;
	  else {
	    if (dh > dv)
	      tmp = (outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 1;
	    else
	      tmp = (outG[(i + j - 1) * 3] +
		     outG[(i + j + 1) * 3] +
		     outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 2;
	  }
	  CLIP(tmp, outG[(i + j) * 3]);
	}
      }
      for (i = 3*sx; i < (sy - 3)*sx; i += (sx<<1)) {
	for (j = 3; j < sx - 2; j += 2) {
	  dh = abs(((outR[(i + j - 2) * 3] +
		    outR[(i + j + 2) * 3]) >> 1) -
		   outR[(i + j) * 3]);
	  dv = abs(((outR[(i - (sx<<1) + j) * 3] +
		    outR[(i + (sx<<1) + j) * 3]) >> 1) -
		   outR[(i + j) * 3]);
	  if (dh < dv)
	    tmp = (outG[(i + j - 1) * 3] +
		   outG[(i + j + 1) * 3]) >>1;
	  else {
	    if (dh > dv)
	      tmp = (outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >>1;
	    else
	      tmp = (outG[(i + j - 1) * 3] +
		     outG[(i + j + 1) * 3] +
		     outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >>2;
	  }
	  CLIP(tmp, outG[(i + j) * 3]);
	}
      }
      // process RED channel
      for (i = sx; i < (sy - 1)*sx; i += (sx<<1)) {	// G-points (1/2)
	for (j = 2; j < sx - 1; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i + j - 1) * 3] -
		outG[(i + j - 1) * 3] +
		outR[(i + j + 1) * 3] -
		outG[(i + j + 1) * 3]) >>1);
	  CLIP(tmp, outR[(i + j) * 3]);
	}
      }
      for (i = 2*sx; i < (sy - 2)*sx; i += (sx<<1)) {
	for (j = 1; j < sx; j += 2) {	// G-points (2/2)
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i - sx + j) * 3] -
		outG[(i - sx + j) * 3] +
		outR[(i + sx + j) * 3] -
		outG[(i + sx + j) * 3]) >> 1);
	  CLIP(tmp, outR[(i + j) * 3]);
	}
	for (j = 2; j < sx - 1; j += 2) {	// B-points
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i - sx + j - 1) * 3] -
		outG[(i - sx + j - 1) * 3] +
		outR[(i - sx + j + 1) * 3] -
		outG[(i - sx + j + 1) * 3] +
		outR[(i + sx + j - 1) * 3] -
		outG[(i + sx + j - 1) * 3] +
		outR[(i + sx + j + 1) * 3] -
		outG[(i + sx + j + 1) * 3]) >> 2);
	  CLIP(tmp, outR[(i + j) * 3]);
	}
      }
      
      // process BLUE channel
      for (i = 0; i < sy*sx; i += (sx<<1)) {
	for (j = 1; j < sx - 2; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i + j - 1) * 3] -
		outG[(i + j - 1) * 3] +
		outB[(i + j + 1) * 3] -
		outG[(i + j + 1) * 3]) >> 1);
	  CLIP(tmp, outB[(i + j) * 3]);
	}
      }
      for (i = sx; i < (sy - 1)*sx; i += (sx<<1)) {
	for (j = 0; j < sx - 1; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i - sx + j) * 3] -
		outG[(i - sx + j) * 3] +
		outB[(i + sx + j) * 3] -
		outG[(i + sx + j) * 3]) >> 1);
	  CLIP(tmp, outB[(i + j) * 3]);
	}
	for (j = 1; j < sx - 2; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i - sx + j - 1) * 3] -
		outG[(i - sx + j - 1) * 3] +
		outB[(i - sx + j + 1) * 3] -
		outG[(i - sx + j + 1) * 3] +
		outB[(i + sx + j - 1) * 3] -
		outG[(i + sx + j - 1) * 3] +
		outB[(i + sx + j + 1) * 3] -
		outG[(i + sx + j + 1) * 3]) >> 2);
	  CLIP(tmp, outB[(i + j) * 3]);
	}
      }
      break;
    default:			//---------------------------------------------------------
//      fprintf(stderr, "Bad bayer pattern ID: %d\n", tile);
//      return;
      break;
    }
    
    ClearBorders(rgb, sx, sy, 3);
}

/* coriander's Bayer decoding (GPL) */
void
dc1394_bayer_Downsample(const unsigned char *bayer, unsigned char *rgb, int sx, int sy, int tile)
{
    unsigned char *outR, *outG, *outB;
    register int i, j;
    int tmp;

    sx *= 2;
    sy *= 2;

    switch (tile) {
    case DC1394_COLOR_FILTER_GRBG:
    case DC1394_COLOR_FILTER_BGGR:
	outR = &rgb[0];
	outG = &rgb[1];
	outB = &rgb[2];
	break;
    case DC1394_COLOR_FILTER_GBRG:
    case DC1394_COLOR_FILTER_RGGB:
	outR = &rgb[2];
	outG = &rgb[1];
	outB = &rgb[0];
	break;
    default:
//	fprintf(stderr, "Bad Bayer pattern ID: %d\n", tile);
//	return;
	break;
    }

    switch (tile) {
    case DC1394_COLOR_FILTER_GRBG:	//---------------------------------------------------------
    case DC1394_COLOR_FILTER_GBRG:
	for (i = 0; i < sy*sx; i += (sx<<1)) {
	    for (j = 0; j < sx; j += 2) {
		tmp =
		    ((bayer[i + j] + bayer[i + sx + j + 1]) >> 1);
		CLIP(tmp, outG[((i >> 2) + (j >> 1)) * 3]);
		tmp = bayer[i + sx + j + 1];
		CLIP(tmp, outR[((i >> 2) + (j >> 1)) * 3]);
		tmp = bayer[i + sx + j];
		CLIP(tmp, outB[((i >> 2) + (j >> 1)) * 3]);
	    }
	}
	break;
    case DC1394_COLOR_FILTER_BGGR:	//---------------------------------------------------------
    case DC1394_COLOR_FILTER_RGGB:
	for (i = 0; i < sy*sx; i += (sx<<1)) {
	    for (j = 0; j < sx; j += 2) {
		tmp =
		    ((bayer[i + sx + j] + bayer[i + j + 1]) >> 1);
		CLIP(tmp, outG[((i >> 2) + (j >> 1)) * 3]);
		tmp = bayer[i + sx + j + 1];
		CLIP(tmp, outR[((i >> 2) + (j >> 1)) * 3]);
		tmp = bayer[i + j];
		CLIP(tmp, outB[((i >> 2) + (j >> 1)) * 3]);
	    }
	}
	break;
    default:			//---------------------------------------------------------
//	fprintf(stderr, "Bad Bayer pattern ID: %d\n", tile);
//	return;
	break;
    }

}

/* this is the method used inside AVT cameras. See AVT docs. */
void
dc1394_bayer_Simple(const unsigned char *bayer, unsigned char *rgb, int sx, int sy, int tile)
{
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == DC1394_COLOR_FILTER_BGGR
        || tile == DC1394_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == DC1394_COLOR_FILTER_GBRG
        || tile == DC1394_COLOR_FILTER_GRBG;
    int i, imax, iinc;

    /* add black border */
    imax = sx * sy * 3;
    for (i = sx * (sy - 1) * 3; i < imax; i++) {
        rgb[i] = 0;
    }
    iinc = (sx - 1) * 3;
    for (i = (sx - 1) * 3; i < imax; i += iinc) {
        rgb[i++] = 0;
        rgb[i++] = 0;
        rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        const unsigned char *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[1];
            rgb[0] = (bayer[0] + bayer[bayerStep + 1] + 1) >> 1;
            rgb[blue] = bayer[bayerStep];
            bayer++;
            rgb += 3;
        }

        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                rgb[-1] = bayer[0];
                rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                rgb[1] = bayer[bayerStep + 1];

                rgb[2] = bayer[2];
                rgb[3] = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                rgb[4] = bayer[bayerStep + 1];
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                rgb[1] = bayer[0];
                rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
                rgb[-1] = bayer[bayerStep + 1];

                rgb[4] = bayer[2];
                rgb[3] = (bayer[1] + bayer[bayerStep + 2] + 1) >> 1;
                rgb[2] = bayer[bayerStep + 1];
            }
        }

        if (bayer < bayerEnd) {
            rgb[-blue] = bayer[0];
            rgb[0] = (bayer[1] + bayer[bayerStep] + 1) >> 1;
            rgb[blue] = bayer[bayerStep + 1];
            bayer++;
            rgb += 3;
        }

        bayer -= width;
        rgb -= width * 3;

        blue = -blue;
        start_with_green = !start_with_green;
    }
}

/* 16-bits versions */

/* insprired by OpenCV's Bayer decoding */
void
dc1394_bayer_NearestNeighbor_uint16(const uint16_t *bayer, uint16_t *rgb, int sx, int sy, int tile, int bits)
{
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == DC1394_COLOR_FILTER_BGGR
	|| tile == DC1394_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == DC1394_COLOR_FILTER_GBRG
	|| tile == DC1394_COLOR_FILTER_GRBG;
    int i, iinc, imax;

    /* add black border */
    imax = sx * sy * 3;
    for (i = sx * (sy - 1) * 3; i < imax; i++) {
	rgb[i] = 0;
    }
    iinc = (sx - 1) * 3;
    for (i = (sx - 1) * 3; i < imax; i += iinc) {
	rgb[i++] = 0;
	rgb[i++] = 0;
	rgb[i++] = 0;
    }

    rgb += 1;
    height -= 1;
    width -= 1;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
      //int t0, t1;
	const uint16_t *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[1];
            rgb[0] = bayer[bayerStep + 1];
            rgb[blue] = bayer[bayerStep];
            bayer++;
            rgb += 3;
        }

        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                rgb[-1] = bayer[0];
                rgb[0] = bayer[1];
                rgb[1] = bayer[bayerStep + 1];

                rgb[2] = bayer[2];
                rgb[3] = bayer[bayerStep + 2];
                rgb[4] = bayer[bayerStep + 1];
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                rgb[1] = bayer[0];
                rgb[0] = bayer[1];
                rgb[-1] = bayer[bayerStep + 1];

                rgb[4] = bayer[2];
                rgb[3] = bayer[bayerStep + 2];
                rgb[2] = bayer[bayerStep + 1];
            }
        }

        if (bayer < bayerEnd) {
            rgb[-blue] = bayer[0];
            rgb[0] = bayer[1];
            rgb[blue] = bayer[bayerStep + 1];
            bayer++;
            rgb += 3;
        }

	bayer -= width;
	rgb -= width * 3;

	blue = -blue;
	start_with_green = !start_with_green;
    }
}
/* OpenCV's Bayer decoding */
void
dc1394_bayer_Bilinear_uint16(const uint16_t *bayer, uint16_t *rgb, int sx, int sy, int tile, int bits)
{
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == DC1394_COLOR_FILTER_BGGR
	|| tile == DC1394_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == DC1394_COLOR_FILTER_GBRG
	|| tile == DC1394_COLOR_FILTER_GRBG;

    rgb += rgbStep + 3 + 1;
    height -= 2;
    width -= 2;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
	int t0, t1;
	const uint16_t *bayerEnd = bayer + width;

	if (start_with_green) {
	    /* OpenCV has a bug in the next line, which was
	       t0 = (bayer[0] + bayer[bayerStep * 2] + 1) >> 1; */
	    t0 = (bayer[1] + bayer[bayerStep * 2 + 1] + 1) >> 1;
	    t1 = (bayer[bayerStep] + bayer[bayerStep + 2] + 1) >> 1;
	    rgb[-blue] = (uint16_t) t0;
	    rgb[0] = bayer[bayerStep + 1];
	    rgb[blue] = (uint16_t) t1;
	    bayer++;
	    rgb += 3;
	}

	if (blue > 0) {
	    for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
		t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
		      bayer[bayerStep * 2 + 2] + 2) >> 2;
		t1 = (bayer[1] + bayer[bayerStep] +
		      bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
		      2) >> 2;
		rgb[-1] = (uint16_t) t0;
		rgb[0] = (uint16_t) t1;
		rgb[1] = bayer[bayerStep + 1];

		t0 = (bayer[2] + bayer[bayerStep * 2 + 2] + 1) >> 1;
		t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
		      1) >> 1;
		rgb[2] = (uint16_t) t0;
		rgb[3] = bayer[bayerStep + 2];
		rgb[4] = (uint16_t) t1;
	    }
	} else {
	    for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
		t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
		      bayer[bayerStep * 2 + 2] + 2) >> 2;
		t1 = (bayer[1] + bayer[bayerStep] +
		      bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
		      2) >> 2;
		rgb[1] = (uint16_t) t0;
		rgb[0] = (uint16_t) t1;
		rgb[-1] = bayer[bayerStep + 1];

		t0 = (bayer[2] + bayer[bayerStep * 2 + 2] + 1) >> 1;
		t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
		      1) >> 1;
		rgb[4] = (uint16_t) t0;
		rgb[3] = bayer[bayerStep + 2];
		rgb[2] = (uint16_t) t1;
	    }
	}

	if (bayer < bayerEnd) {
	    t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
		  bayer[bayerStep * 2 + 2] + 2) >> 2;
	    t1 = (bayer[1] + bayer[bayerStep] +
		  bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
		  2) >> 2;
	    rgb[-blue] = (uint16_t) t0;
	    rgb[0] = (uint16_t) t1;
	    rgb[blue] = bayer[bayerStep + 1];
	    bayer++;
	    rgb += 3;
	}

	bayer -= width;
	rgb -= width * 3;

	blue = -blue;
	start_with_green = !start_with_green;
    }
}

/* High-Quality Linear Interpolation For Demosaicing Of
   Bayer-Patterned Color Images, by Henrique S. Malvar, Li-wei He, and
   Ross Cutler, in ICASSP'04 */
void
dc1394_bayer_HQLinear_uint16(const uint16_t *bayer, uint16_t *rgb, int sx, int sy, int tile, int bits)
{
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    /*
       the two letters  of the OpenCV name are respectively
       the 4th and 3rd letters from the blinky name,
       and we also have to switch R and B (OpenCV is BGR)

       CV_BayerBG2BGR <-> DC1394_COLOR_FILTER_BGGR
       CV_BayerGB2BGR <-> DC1394_COLOR_FILTER_GBRG
       CV_BayerGR2BGR <-> DC1394_COLOR_FILTER_GRBG

       int blue = tile == CV_BayerBG2BGR || tile == CV_BayerGB2BGR ? -1 : 1;
       int start_with_green = tile == CV_BayerGB2BGR || tile == CV_BayerGR2BGR;
     */
    int blue = tile == DC1394_COLOR_FILTER_BGGR
	|| tile == DC1394_COLOR_FILTER_GBRG ? -1 : 1;
    int start_with_green = tile == DC1394_COLOR_FILTER_GBRG
	|| tile == DC1394_COLOR_FILTER_GRBG;

    ClearBorders_uint16(rgb, sx, sy, 2);
    rgb += 2 * rgbStep + 6 + 1;
    height -= 4;
    width -= 4;

    /* We begin with a (+1 line,+1 column) offset with respect to bilinear decoding, so start_with_green is the same, but blue is opposite */
    blue = -blue;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
	int t0, t1;
	const uint16_t *bayerEnd = bayer + width;
	const int bayerStep2 = bayerStep * 2;
	const int bayerStep3 = bayerStep * 3;
	const int bayerStep4 = bayerStep * 4;

	if (start_with_green) {
	    /* at green pixel */
	    rgb[0] = bayer[bayerStep2 + 2];
	    t0 = rgb[0] * 5
		+ ((bayer[bayerStep + 2] + bayer[bayerStep3 + 2]) << 2)
		- bayer[2]
		- bayer[bayerStep + 1]
		- bayer[bayerStep + 3]
		- bayer[bayerStep3 + 1]
		- bayer[bayerStep3 + 3]
		- bayer[bayerStep4 + 2]
		+ ((bayer[bayerStep2] + bayer[bayerStep2 + 4] + 1) >> 1);
	    t1 = rgb[0] * 5 +
		((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 3]) << 2)
		- bayer[bayerStep2]
		- bayer[bayerStep + 1]
		- bayer[bayerStep + 3]
		- bayer[bayerStep3 + 1]
		- bayer[bayerStep3 + 3]
		- bayer[bayerStep2 + 4]
		+ ((bayer[2] + bayer[bayerStep4 + 2] + 1) >> 1);
	    t0 = (t0 + 4) >> 3;
	    CLIP16(t0, rgb[-blue], bits);
	    t1 = (t1 + 4) >> 3;
	    CLIP16(t1, rgb[blue], bits);
	    bayer++;
	    rgb += 3;
	}

	if (blue > 0) {
	    for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
		/* B at B */
		rgb[1] = bayer[bayerStep2 + 2];
		/* R at B */
		t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
		       bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
		    -
		    (((bayer[2] + bayer[bayerStep2] +
		       bayer[bayerStep2 + 4] + bayer[bayerStep4 +
						     2]) * 3 + 1) >> 1)
		    + rgb[1] * 6;
		/* G at B */
		t1 = ((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
		       bayer[bayerStep2 + 3] + bayer[bayerStep * 3 +
						     2]) << 1)
		    - (bayer[2] + bayer[bayerStep2] +
		       bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
		    + (rgb[1] << 2);
		t0 = (t0 + 4) >> 3;
		CLIP16(t0, rgb[-1], bits);
		t1 = (t1 + 4) >> 3;
		CLIP16(t1, rgb[0], bits);
		/* at green pixel */
		rgb[3] = bayer[bayerStep2 + 3];
		t0 = rgb[3] * 5
		    + ((bayer[bayerStep + 3] + bayer[bayerStep3 + 3]) << 2)
		    - bayer[3]
		    - bayer[bayerStep + 2]
		    - bayer[bayerStep + 4]
		    - bayer[bayerStep3 + 2]
		    - bayer[bayerStep3 + 4]
		    - bayer[bayerStep4 + 3]
		    +
		    ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 5] +
		      1) >> 1);
		t1 = rgb[3] * 5 +
		    ((bayer[bayerStep2 + 2] + bayer[bayerStep2 + 4]) << 2)
		    - bayer[bayerStep2 + 1]
		    - bayer[bayerStep + 2]
		    - bayer[bayerStep + 4]
		    - bayer[bayerStep3 + 2]
		    - bayer[bayerStep3 + 4]
		    - bayer[bayerStep2 + 5]
		    + ((bayer[3] + bayer[bayerStep4 + 3] + 1) >> 1);
		t0 = (t0 + 4) >> 3;
		CLIP16(t0, rgb[2], bits);
		t1 = (t1 + 4) >> 3;
		CLIP16(t1, rgb[4], bits);
	    }
	} else {
	    for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
		/* R at R */
		rgb[-1] = bayer[bayerStep2 + 2];
		/* B at R */
		t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
		       bayer[bayerStep * 3 + 1] + bayer[bayerStep3 +
							3]) << 1)
		    -
		    (((bayer[2] + bayer[bayerStep2] +
		       bayer[bayerStep2 + 4] + bayer[bayerStep4 +
						     2]) * 3 + 1) >> 1)
		    + rgb[-1] * 6;
		/* G at R */
		t1 = ((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
		       bayer[bayerStep2 + 3] + bayer[bayerStep3 + 2]) << 1)
		    - (bayer[2] + bayer[bayerStep2] +
		       bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
		    + (rgb[-1] << 2);
		t0 = (t0 + 4) >> 3;
		CLIP16(t0, rgb[1], bits);
		t1 = (t1 + 4) >> 3;
		CLIP16(t1, rgb[0], bits);

		/* at green pixel */
		rgb[3] = bayer[bayerStep2 + 3];
		t0 = rgb[3] * 5
		    + ((bayer[bayerStep + 3] + bayer[bayerStep3 + 3]) << 2)
		    - bayer[3]
		    - bayer[bayerStep + 2]
		    - bayer[bayerStep + 4]
		    - bayer[bayerStep3 + 2]
		    - bayer[bayerStep3 + 4]
		    - bayer[bayerStep4 + 3]
		    +
		    ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 5] +
		      1) >> 1);
		t1 = rgb[3] * 5 +
		    ((bayer[bayerStep2 + 2] + bayer[bayerStep2 + 4]) << 2)
		    - bayer[bayerStep2 + 1]
		    - bayer[bayerStep + 2]
		    - bayer[bayerStep + 4]
		    - bayer[bayerStep3 + 2]
		    - bayer[bayerStep3 + 4]
		    - bayer[bayerStep2 + 5]
		    + ((bayer[3] + bayer[bayerStep4 + 3] + 1) >> 1);
		t0 = (t0 + 4) >> 3;
		CLIP16(t0, rgb[4], bits);
		t1 = (t1 + 4) >> 3;
		CLIP16(t1, rgb[2], bits);
	    }
	}

	if (bayer < bayerEnd) {
	    /* B at B */
	    rgb[blue] = bayer[bayerStep2 + 2];
	    /* R at B */
	    t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
		   bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
		-
		(((bayer[2] + bayer[bayerStep2] +
		   bayer[bayerStep2 + 4] + bayer[bayerStep4 +
						 2]) * 3 + 1) >> 1)
		+ rgb[blue] * 6;
	    /* G at B */
	    t1 = (((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
		    bayer[bayerStep2 + 3] + bayer[bayerStep3 + 2])) << 1)
		- (bayer[2] + bayer[bayerStep2] +
		   bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
		+ (rgb[blue] << 2);
	    t0 = (t0 + 4) >> 3;
	    CLIP16(t0, rgb[-blue], bits);
	    t1 = (t1 + 4) >> 3;
	    CLIP16(t1, rgb[0], bits);
	    bayer++;
	    rgb += 3;
	}

	bayer -= width;
	rgb -= width * 3;

	blue = -blue;
	start_with_green = !start_with_green;
    }
}

/* coriander's Bayer decoding (GPL) */
void
dc1394_bayer_EdgeSense_uint16(const uint16_t *bayer, uint16_t *rgb, int sx, int sy, int tile, int bits)
{
    uint16_t *outR, *outG, *outB;
    register int i, j;
    int dh, dv;
    int tmp;

    // sx and sy should be even
    switch (tile) {
    case DC1394_COLOR_FILTER_GRBG:
    case DC1394_COLOR_FILTER_BGGR:
	outR = &rgb[0];
	outG = &rgb[1];
	outB = &rgb[2];
	break;
    case DC1394_COLOR_FILTER_GBRG:
    case DC1394_COLOR_FILTER_RGGB:
	outR = &rgb[2];
	outG = &rgb[1];
	outB = &rgb[0];
	break;
    default:
//	fprintf(stderr, "Bad bayer pattern ID: %d\n", tile);
//	return;
	break;
    }

    switch (tile) {
    case DC1394_COLOR_FILTER_GRBG:	//---------------------------------------------------------
    case DC1394_COLOR_FILTER_GBRG:
	// copy original RGB data to output images
      for (i = 0; i < sy*sx; i += (sx<<1)) {
	for (j = 0; j < sx; j += 2) {
	  outG[(i + j) * 3] = bayer[i + j];
	  outG[(i + sx + (j + 1)) * 3] = bayer[i + sx + (j + 1)];
	  outR[(i + j + 1) * 3] = bayer[i + j + 1];
	  outB[(i + sx + j) * 3] = bayer[i + sx + j];
	}
      }
      // process GREEN channel
      for (i = 3*sx; i < (sy - 2)*sx; i += (sx<<1)) {
	for (j = 2; j < sx - 3; j += 2) {
	  dh = abs(((outB[(i + j - 2) * 3] +
		     outB[(i + j + 2) * 3]) >> 1) -
		   outB[(i + j) * 3]);
	  dv = abs(((outB[(i - (sx<<1) + j) * 3] +
		     outB[(i + (sx<<1) + j) * 3]) >> 1)  -
		   outB[(i + j) * 3]);
	  if (dh < dv)
	    tmp = (outG[(i + j - 1) * 3] +
		   outG[(i + j + 1) * 3]) >> 1;
	  else {
	    if (dh > dv)
	      tmp = (outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 1;
	    else
	      tmp = (outG[(i + j - 1) * 3] +
		     outG[(i + j + 1) * 3] +
		     outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 2;
	  }
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }
	
      for (i = 2*sx; i < (sy - 3)*sx; i += (sx<<1)) {
	for (j = 3; j < sx - 2; j += 2) {
	  dh = abs(((outR[(i + j - 2) * 3] +
		     outR[(i + j + 2) * 3]) >>1 ) -
		   outR[(i + j) * 3]);
	  dv = abs(((outR[(i - (sx<<1) + j) * 3] +
		     outR[(i + (sx<<1) + j) * 3]) >>1 ) -
		   outR[(i + j) * 3]);
	  if (dh < dv)
	    tmp = (outG[(i + j - 1) * 3] +
		   outG[(i + j + 1) * 3]) >> 1;
	  else {
	    if (dh > dv)
	      tmp = (outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 1;
	    else
	      tmp = (outG[(i + j - 1) * 3] +
		     outG[(i + j + 1) * 3] +
		     outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 2;
	  }
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }
      // process RED channel
      for (i = 0; i < (sy - 1)*sx; i += (sx<<1)) {
	for (j = 2; j < sx - 1; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i + j - 1) * 3] -
		outG[(i + j - 1) * 3] +
		outR[(i + j + 1) * 3] -
		outG[(i + j + 1) * 3]) >> 1);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }
      for (i = sx; i < (sy - 2)*sx; i += (sx<<1)) {
	for (j = 1; j < sx; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i - sx + j) * 3] -
		outG[(i - sx + j) * 3] +
		outR[(i + sx + j) * 3] -
		outG[(i + sx + j) * 3]) >> 1);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
	for (j = 2; j < sx - 1; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i - sx + j - 1) * 3] -
		outG[(i - sx + j - 1) * 3] +
		outR[(i - sx + j + 1) * 3] -
		outG[(i - sx + j + 1) * 3] +
		outR[(i + sx + j - 1) * 3] -
		outG[(i + sx + j - 1) * 3] +
		outR[(i + sx + j + 1) * 3] -
		outG[(i + sx + j + 1) * 3]) >> 2);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }

      // process BLUE channel
      for (i = sx; i < sy*sx; i += (sx<<1)) {
	for (j = 1; j < sx - 2; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i + j - 1) * 3] -
		outG[(i + j - 1) * 3] +
		outB[(i + j + 1) * 3] -
		outG[(i + j + 1) * 3]) >> 1);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }
      for (i = 2*sx; i < (sy - 1)*sx; i += (sx<<1)) {
	for (j = 0; j < sx - 1; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i - sx + j) * 3] -
		outG[(i - sx + j) * 3] +
		outB[(i + sx + j) * 3] -
		outG[(i + sx + j) * 3]) >> 1);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
	for (j = 1; j < sx - 2; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i - sx + j - 1) * 3] -
		outG[(i - sx + j - 1) * 3] +
		outB[(i - sx + j + 1) * 3] -
		outG[(i - sx + j + 1) * 3] +
		outB[(i + sx + j - 1) * 3] -
		outG[(i + sx + j - 1) * 3] +
		outB[(i + sx + j + 1) * 3] -
		outG[(i + sx + j + 1) * 3]) >> 2);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }
      break;

    case DC1394_COLOR_FILTER_BGGR:	//---------------------------------------------------------
    case DC1394_COLOR_FILTER_RGGB:
	// copy original RGB data to output images
      for (i = 0; i < sy*sx; i += (sx<<1)) {
	for (j = 0; j < sx; j += 2) {
	  outB[(i + j) * 3] = bayer[i + j];
	  outR[(i + sx + (j + 1)) * 3] = bayer[i + sx + (j + 1)];
	  outG[(i + j + 1) * 3] = bayer[i + j + 1];
	  outG[(i + sx + j) * 3] = bayer[i + sx + j];
	}
      }
      // process GREEN channel
      for (i = 2*sx; i < (sy - 2)*sx; i += (sx<<1)) {
	for (j = 2; j < sx - 3; j += 2) {
	  dh = abs(((outB[(i + j - 2) * 3] +
		    outB[(i + j + 2) * 3]) >> 1) -
		   outB[(i + j) * 3]);
	  dv = abs(((outB[(i - (sx<<1) + j) * 3] +
		    outB[(i + (sx<<1) + j) * 3]) >> 1) -
		   outB[(i + j) * 3]);
	  if (dh < dv)
	    tmp = (outG[(i + j - 1) * 3] +
		   outG[(i + j + 1) * 3]) >> 1;
	  else {
	    if (dh > dv)
	      tmp = (outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 1;
	    else
	      tmp = (outG[(i + j - 1) * 3] +
		     outG[(i + j + 1) * 3] +
		     outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >> 2;
	  }
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }
      for (i = 3*sx; i < (sy - 3)*sx; i += (sx<<1)) {
	for (j = 3; j < sx - 2; j += 2) {
	  dh = abs(((outR[(i + j - 2) * 3] +
		    outR[(i + j + 2) * 3]) >> 1) -
		   outR[(i + j) * 3]);
	  dv = abs(((outR[(i - (sx<<1) + j) * 3] +
		    outR[(i + (sx<<1) + j) * 3]) >> 1) -
		   outR[(i + j) * 3]);
	  if (dh < dv)
	    tmp = (outG[(i + j - 1) * 3] +
		   outG[(i + j + 1) * 3]) >>1;
	  else {
	    if (dh > dv)
	      tmp = (outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >>1;
	    else
	      tmp = (outG[(i + j - 1) * 3] +
		     outG[(i + j + 1) * 3] +
		     outG[(i - sx + j) * 3] +
		     outG[(i + sx + j) * 3]) >>2;
	  }
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }
      // process RED channel
      for (i = sx; i < (sy - 1)*sx; i += (sx<<1)) {	// G-points (1/2)
	for (j = 2; j < sx - 1; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i + j - 1) * 3] -
		outG[(i + j - 1) * 3] +
		outR[(i + j + 1) * 3] -
		outG[(i + j + 1) * 3]) >>1);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }
      for (i = 2*sx; i < (sy - 2)*sx; i += (sx<<1)) {
	for (j = 1; j < sx; j += 2) {	// G-points (2/2)
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i - sx + j) * 3] -
		outG[(i - sx + j) * 3] +
		outR[(i + sx + j) * 3] -
		outG[(i + sx + j) * 3]) >> 1);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
	for (j = 2; j < sx - 1; j += 2) {	// B-points
	  tmp = outG[(i + j) * 3] +
	      ((outR[(i - sx + j - 1) * 3] -
		outG[(i - sx + j - 1) * 3] +
		outR[(i - sx + j + 1) * 3] -
		outG[(i - sx + j + 1) * 3] +
		outR[(i + sx + j - 1) * 3] -
		outG[(i + sx + j - 1) * 3] +
		outR[(i + sx + j + 1) * 3] -
		outG[(i + sx + j + 1) * 3]) >> 2);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }
      
      // process BLUE channel
      for (i = 0; i < sy*sx; i += (sx<<1)) {
	for (j = 1; j < sx - 2; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i + j - 1) * 3] -
		outG[(i + j - 1) * 3] +
		outB[(i + j + 1) * 3] -
		outG[(i + j + 1) * 3]) >> 1);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }
      for (i = sx; i < (sy - 1)*sx; i += (sx<<1)) {
	for (j = 0; j < sx - 1; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i - sx + j) * 3] -
		outG[(i - sx + j) * 3] +
		outB[(i + sx + j) * 3] -
		outG[(i + sx + j) * 3]) >> 1);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
	for (j = 1; j < sx - 2; j += 2) {
	  tmp = outG[(i + j) * 3] +
	      ((outB[(i - sx + j - 1) * 3] -
		outG[(i - sx + j - 1) * 3] +
		outB[(i - sx + j + 1) * 3] -
		outG[(i - sx + j + 1) * 3] +
		outB[(i + sx + j - 1) * 3] -
		outG[(i + sx + j - 1) * 3] +
		outB[(i + sx + j + 1) * 3] -
		outG[(i + sx + j + 1) * 3]) >> 2);
	  CLIP16(tmp, outR[(i + j) * 3], bits);
	}
      }
      break;
    default:			//---------------------------------------------------------
//      fprintf(stderr, "Bad bayer pattern ID: %d\n", tile);
//      return;
      break;
    }
   
    ClearBorders_uint16(rgb, sx, sy, 3);
}

/* coriander's Bayer decoding (GPL) */
void
dc1394_bayer_Downsample_uint16(const uint16_t *bayer, uint16_t *rgb, int sx, int sy, int tile, int bits)
{
    uint16_t *outR, *outG, *outB;
    register int i, j;
    int tmp;

    sx *= 2;
    sy *= 2;

    switch (tile) {
    case DC1394_COLOR_FILTER_GRBG:
    case DC1394_COLOR_FILTER_BGGR:
	outR = &rgb[0];
	outG = &rgb[1];
	outB = &rgb[2];
	break;
    case DC1394_COLOR_FILTER_GBRG:
    case DC1394_COLOR_FILTER_RGGB:
	outR = &rgb[2];
	outG = &rgb[1];
	outB = &rgb[0];
	break;
    default:
//	fprintf(stderr, "Bad Bayer pattern ID: %d\n", tile);
//	return;
	break;
    }

    switch (tile) {
    case DC1394_COLOR_FILTER_GRBG:	//---------------------------------------------------------
    case DC1394_COLOR_FILTER_GBRG:
	for (i = 0; i < sy*sx; i += (sx<<1)) {
	    for (j = 0; j < sx; j += 2) {
		tmp =
		    ((bayer[i + j] + bayer[i + sx + j + 1]) >> 1);
		CLIP16(tmp, outG[((i >> 2) + (j >> 1)) * 3], bits);
		tmp = bayer[i + sx + j + 1];
		CLIP16(tmp, outR[((i >> 2) + (j >> 1)) * 3], bits);
		tmp = bayer[i + sx + j];
		CLIP16(tmp, outB[((i >> 2) + (j >> 1)) * 3], bits);
	    }
	}
	break;
    case DC1394_COLOR_FILTER_BGGR:	//---------------------------------------------------------
    case DC1394_COLOR_FILTER_RGGB:
	for (i = 0; i < sy*sx; i += (sx<<1)) {
	    for (j = 0; j < sx; j += 2) {
		tmp =
		    ((bayer[i + sx + j] + bayer[i + j + 1]) >> 1);
		CLIP16(tmp, outG[((i >> 2) + (j >> 1)) * 3], bits);
		tmp = bayer[i + sx + j + 1];
		CLIP16(tmp, outR[((i >> 2) + (j >> 1)) * 3], bits);
		tmp = bayer[i + j];
		CLIP16(tmp, outB[((i >> 2) + (j >> 1)) * 3], bits);
	    }
	}
	break;
    default:			//---------------------------------------------------------
//	fprintf(stderr, "Bad Bayer pattern ID: %d\n", tile);
//	return;
	break;
    }

}

/* coriander's Bayer decoding (GPL) */
void dc1394_bayer_Simple_uint16(const uint16_t *bayer, uint16_t *rgb, int sx, int sy, int tile, int bits)
{
    uint16_t *outR, *outG, *outB;
    register int i, j;
    int tmp, base;

    // sx and sy should be even
    switch (tile) {
    case DC1394_COLOR_FILTER_GRBG:
    case DC1394_COLOR_FILTER_BGGR:
//	outR = &rgb[0];
//	outG = &rgb[1];
//	outB = &rgb[2];
	break;
    case DC1394_COLOR_FILTER_GBRG:
    case DC1394_COLOR_FILTER_RGGB:
//	outR = &rgb[2];
//	outG = &rgb[1];
//	outB = &rgb[0];
	break;
    default:
//	fprintf(stderr, "Bad bayer pattern ID: %d\n", tile);
//	return;
	break;
    }

    switch (tile) {
    case DC1394_COLOR_FILTER_GRBG:
    case DC1394_COLOR_FILTER_BGGR:
	outR = &rgb[0];
	outG = &rgb[1];
	outB = &rgb[2];
	break;
    case DC1394_COLOR_FILTER_GBRG:
    case DC1394_COLOR_FILTER_RGGB:
	outR = &rgb[2];
	outG = &rgb[1];
	outB = &rgb[0];
	break;
    default:
	outR = NULL;
	outG = NULL;
	outB = NULL;
	break;
    }

    switch (tile) {
    case DC1394_COLOR_FILTER_GRBG:	//---------------------------------------------------------
    case DC1394_COLOR_FILTER_GBRG:
	for (i = 0; i < sy - 1; i += 2) {
	    for (j = 0; j < sx - 1; j += 2) {
		base = i * sx + j;
		tmp = ((bayer[base] + bayer[base + sx + 1]) >> 1);
		CLIP16(tmp, outG[base * 3], bits);
		tmp = bayer[base + 1];
		CLIP16(tmp, outR[base * 3], bits);
		tmp = bayer[base + sx];
		CLIP16(tmp, outB[base * 3], bits);
	    }
	}
	for (i = 0; i < sy - 1; i += 2) {
	    for (j = 1; j < sx - 1; j += 2) {
		base = i * sx + j;
		tmp = ((bayer[base + 1] + bayer[base + sx]) >> 1);
		CLIP16(tmp, outG[(base) * 3], bits);
		tmp = bayer[base];
		CLIP16(tmp, outR[(base) * 3], bits);
		tmp = bayer[base + 1 + sx];
		CLIP16(tmp, outB[(base) * 3], bits);
	    }
	}
	for (i = 1; i < sy - 1; i += 2) {
	    for (j = 0; j < sx - 1; j += 2) {
		base = i * sx + j;
		tmp = ((bayer[base + sx] + bayer[base + 1]) >> 1);
		CLIP16(tmp, outG[base * 3], bits);
		tmp = bayer[base + sx + 1];
		CLIP16(tmp, outR[base * 3], bits);
		tmp = bayer[base];
		CLIP16(tmp, outB[base * 3], bits);
	    }
	}
	for (i = 1; i < sy - 1; i += 2) {
	    for (j = 1; j < sx - 1; j += 2) {
		base = i * sx + j;
		tmp = ((bayer[base] + bayer[base + 1 + sx]) >> 1);
		CLIP16(tmp, outG[(base) * 3], bits);
		tmp = bayer[base + sx];
		CLIP16(tmp, outR[(base) * 3], bits);
		tmp = bayer[base + 1];
		CLIP16(tmp, outB[(base) * 3], bits);
	    }
	}
	break;
    case DC1394_COLOR_FILTER_BGGR:	//---------------------------------------------------------
    case DC1394_COLOR_FILTER_RGGB:
	for (i = 0; i < sy - 1; i += 2) {
	    for (j = 0; j < sx - 1; j += 2) {
		base = i * sx + j;
		tmp = ((bayer[base + sx] + bayer[base + 1]) >> 1);
		CLIP16(tmp, outG[base * 3], bits);
		tmp = bayer[base + sx + 1];
		CLIP16(tmp, outR[base * 3], bits);
		tmp = bayer[base];
		CLIP16(tmp, outB[base * 3], bits);
	    }
	}
	for (i = 1; i < sy - 1; i += 2) {
	    for (j = 0; j < sx - 1; j += 2) {
		base = i * sx + j;
		tmp = ((bayer[base] + bayer[base + 1 + sx]) >> 1);
		CLIP16(tmp, outG[(base) * 3], bits);
		tmp = bayer[base + 1];
		CLIP16(tmp, outR[(base) * 3], bits);
		tmp = bayer[base + sx];
		CLIP16(tmp, outB[(base) * 3], bits);
	    }
	}
	for (i = 0; i < sy - 1; i += 2) {
	    for (j = 1; j < sx - 1; j += 2) {
		base = i * sx + j;
		tmp = ((bayer[base] + bayer[base + sx + 1]) >> 1);
		CLIP16(tmp, outG[base * 3], bits);
		tmp = bayer[base + sx];
		CLIP16(tmp, outR[base * 3], bits);
		tmp = bayer[base + 1];
		CLIP16(tmp, outB[base * 3], bits);
	    }
	}
	for (i = 1; i < sy - 1; i += 2) {
	    for (j = 1; j < sx - 1; j += 2) {
		base = i * sx + j;
		tmp = ((bayer[base + 1] + bayer[base + sx]) >> 1);
		CLIP16(tmp, outG[(base) * 3], bits);
		tmp = bayer[base];
		CLIP16(tmp, outR[(base) * 3], bits);
		tmp = bayer[base + 1 + sx];
		CLIP16(tmp, outB[(base) * 3], bits);
	    }
	}
	break;
    default:			//---------------------------------------------------------
//	fprintf(stderr, "Bad bayer pattern ID: %d\n", tile);
//	return;
	break;
    }

    /* add black border */
    for (i = sx * (sy - 1) * 3; i < sx * sy * 3; i++) {
	rgb[i] = 0;
    }
    for (i = (sx - 1) * 3; i < sx * sy * 3; i += (sx - 1) * 3) {
	rgb[i++] = 0;
	rgb[i++] = 0;
	rgb[i++] = 0;
    }
}

int dc1394_bayer_decoding_8bit(const unsigned char *bayer, unsigned char *rgb, unsigned int sx, unsigned int sy, unsigned int tile, unsigned int method)
{
  switch (method) {
  case DC1394_BAYER_METHOD_NEAREST:
    dc1394_bayer_NearestNeighbor(bayer, rgb, sx, sy, tile);
    return DC1394_SUCCESS;
  case DC1394_BAYER_METHOD_SIMPLE:
    dc1394_bayer_Simple(bayer, rgb, sx, sy, tile);
    return DC1394_SUCCESS;
  case DC1394_BAYER_METHOD_BILINEAR:
    dc1394_bayer_Bilinear(bayer, rgb, sx, sy, tile);
    return DC1394_SUCCESS;
  case DC1394_BAYER_METHOD_HQLINEAR:
    dc1394_bayer_HQLinear(bayer, rgb, sx, sy, tile);
    return DC1394_SUCCESS;
  case DC1394_BAYER_METHOD_DOWNSAMPLE:
    dc1394_bayer_Downsample(bayer, rgb, sx, sy, tile);
    return DC1394_SUCCESS;
  case DC1394_BAYER_METHOD_EDGESENSE:
    dc1394_bayer_EdgeSense(bayer, rgb, sx, sy, tile);
    return DC1394_SUCCESS;
  }

  return DC1394_INVALID_BAYER_METHOD;
}

int
dc1394_bayer_decoding_16bit(const uint16_t *bayer, uint16_t *rgb, uint_t sx, uint_t sy, uint_t tile, uint_t bits, uint_t method)
{
  switch (method) {
  case DC1394_BAYER_METHOD_NEAREST:
    dc1394_bayer_NearestNeighbor_uint16(bayer, rgb, sx, sy, tile, bits);
    return DC1394_SUCCESS;
  case DC1394_BAYER_METHOD_SIMPLE:
    dc1394_bayer_Simple_uint16(bayer, rgb, sx, sy, tile, bits);
    return DC1394_SUCCESS;
  case DC1394_BAYER_METHOD_BILINEAR:
    dc1394_bayer_Bilinear_uint16(bayer, rgb, sx, sy, tile, bits);
    return DC1394_SUCCESS;
  case DC1394_BAYER_METHOD_HQLINEAR:
    dc1394_bayer_HQLinear_uint16(bayer, rgb, sx, sy, tile, bits);
    return DC1394_SUCCESS;
  case DC1394_BAYER_METHOD_DOWNSAMPLE:
    dc1394_bayer_Downsample_uint16(bayer, rgb, sx, sy, tile, bits);
    return DC1394_SUCCESS;
  case DC1394_BAYER_METHOD_EDGESENSE:
    dc1394_bayer_EdgeSense_uint16(bayer, rgb, sx, sy, tile, bits);
    return DC1394_SUCCESS;
  }

  return DC1394_INVALID_BAYER_METHOD;
}

