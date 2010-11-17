/*******************************************************************************
 * SVS GigE API   Declaration of GigE camera access functions
 *******************************************************************************
 *
 * Version:     1.4.14.26 / November 2008
 * 
 * Copyright:   SVS VISTEK GmbH 
 *
 *******************************************************************************
 * Revision history:
 *
 * Version 1.4.14.26 
 * -----------------
 *  - a need for firmware upgrade is checked on Camera_open()   
 *     + Camera_setAcquisitionModeAndStart()
 *     + Camera_forceValidNetworkSettings()
 *     + Camera_setIPAddress()
 *
 * Version 1.4.14.22 
 * -----------------
 *     + Camera_setLookupTableMode()
 *     + Camera_getLookupTableMode()
 *     + Camera_setLookupTable()
 *     + Camera_getLookupTable()
 *     + Camera_setAcquisitionControl()
 *     + Camera_getAcquisitionControl()
 *     + Image_getImage12bitAs8bit()
 *     + Image_getImage12bitAs16bit()
 *     + Image_getImage16bitAs8bit()
 *
 * Version 1.4.13.20 
 * -----------------
 *     + Camera_setWhiteBalance()
 *     + Camera_getWhiteBalance()
 *     + SVGigE_estimateWhiteBalance()
 *     + Camera_registerForLogMessages()
 *
 * Version 1.4.12.18
 * -----------------
 *     + Camera_getPixelClock()
 *
 * Version 1.4.11.16
 * -----------------
 *     + Camera_writeGigECameraMemory()
 *     + Camera_readGigECameraMemory()
 *
 * Version 1.4.8.13
 * ----------------
 *   - fast switching of tap configuration enabled (< 200 ms)
 *
 * Version 1.4.7.11
 * ----------------
 *   - isVersionCompliantDLL() can now run before other functions
 *   - CameraContainer_create() checks also SVGigE driver availability
 *
 *     + Camera_setPixelDepth()
 *     + Camera_getPixelDepth()
 *     + Camera_saveTapBalanceSettings()
 *     + Camera_loadTapBalanceSettings()
 *     + Camera_setTapCalibration()
 *     + Camera_getTapCalibration()
 *     + Camera_setTapConfiguration()
 *     + Camera_getTapConfiguration()
 *
 * Version 1.4.6.9
 * ---------------
 *   - function added:
 *     + Camera_readXMLFile()
 *
 * Version 1.4.3.1
 * ---------------
 *  - packet resend capabilities improved
 *    NOTE: The image pointer will be NULL for uncomplete images 
 *          which may be the case for unsufficient network bandwidth
 *
 * Version 1.4.1.4
 * ---------------
 *   - 64-bit supported
 *   - function added:
 *     + Camera_setStreamingPacketSize()
 *
 * Version 1.4.1.3
 * ---------------
 *   - non-admin account: All users are granted access to the SVGigE driver
 *
 * Version 1.4.0.0
 * ---------------
 *   - shorter names: SVGigE_RETURN instead of SVS_GigE_API_RETURN
 *   - CameraContainer_create(), parameter added: SVGigETL_Type TransportLayerType
 *   - Camera_openConnection(), parameter added: float Timeout
 *   - StreamingChannel_create(), parameter removed: bool DriverEnabled
 *
 *   - functions added:
 *     + Camera_evaluateMaximalPacketSize()
 *     + Camera_getImagerWidth()
 *     + Camera_getImagerHeight()
 *     + Camera_getFrameRateRange()
 *     + Camera_getExposureTimeRange()
 *     + Camera_setAreaOfInterest()
 *     + Camera_getAreaOfInterest()
 *     + StreamingChannel_getFrameLoss()
 *     + StreamingChannel_getActualFrameRate()
 *     + StreamingChannel_getActualDataRate()
 *     + Image_getCamera()
 *     + Image_getPacketCount()
 *     + SVGigE_writeImageToBitmapFile()
 *     + Camera_isCameraFeature()
 *
 *   - function deactivated temporarily
 *     ~ Camera_createLUTwhiteBalance()
 *
 *
 *******************************************************************************
 */

#ifndef SVGigEH
#define SVGigEH

#ifdef __BORLANDC__
  // Generate enums of integer size
#pragma option push -b
#endif

#include <stdio.h>
#include <stdlib.h>

//---------------------------------------------------------------------------

extern "C"
{
  /** Version information
   *  The particular components of the version information will be represented
   *  in the following way
   */
#ifndef SVGigE_VERSION_DEFINED
#define SVGigE_VERSION_DEFINED
  typedef struct
  {
    unsigned char	MajorVersion;
    unsigned char MinorVersion;
    unsigned char	DriverVersion;
    unsigned char	BuildVersion;

  } SVGigE_VERSION;

  /** Version settings
   *  The following version settings apply to current SVGigE SDK:
   */
  #define SVGigE_VERSION_MAJOR			1
  #define SVGigE_VERSION_MINOR			4
  #define SVGigE_VERSION_DRIVER			14
  #define SVGigE_VERSION_BUILD			26
#endif


  /** Camera Container client handle.
   *  A camera container client handle serves as a reference for managing multiple
   *  clients that are connected to a single camera container (which has no notion
   *  about clients). A value of SVGigE_NO_CLIENT serves as an indicator for
   *  an invalid camera container client before obtaining a valid handle.
   */
  typedef int CameraContainerClient_handle;
  #define SVGigE_NO_CLIENT	(CameraContainerClient_handle)-1

  /** Camera handle.
   *  A camera handle serves as a reference for access functions to camera
   *  functionality. A value of SVGigE_NO_CAMERA serves as an indicator for an
   *  invalid camera handle before obtaining a camera from a camera container.
   */
  typedef void * Camera_handle;
  #define SVGigE_NO_CAMERA	(Camera_handle)-1

  /** Streaming channel handle.
   *  A streaming channel handle serves as a reference for image acquisition
   *  functions
   */
  typedef void * StreamingChannel_handle;
  #define SVGigE_NO_STREAMING_CHANNEL	(StreamingChannel_handle)-1

  /** Image handle.
   *  An image handle serves as a reference for picture access and
   *  processing functions
   */
  typedef int Image_handle;

  /** Function returns.
   *  API Functions may return success or error codes by this data type unless
   *  they return specific values
   */
  typedef enum
  {
    SVGigE_SUCCESS                                      =  0,
    SVGigE_ERROR                                        = -1,
    SVGigE_DLL_NOT_LOADED                               = -2,
    SVGigE_DLL_VERSION_MISMATCH                         = -3,
    SVGigE_DRIVER_VERSION_MISMATCH                      = -4,
    SVGigE_WINSOCK_INITIALIZATION_FAILED                = -5,
    SVGigE_CAMERA_CONTAINER_NOT_AVAILABLE               = -6,
    SVGigE_NO_CAMERAS_DETECTED                          = -7,
    SVGigE_CAMERA_NOT_FOUND                             = -8,
    SVGigE_CAMERA_OPEN_FAILED                           = -9,
    SVGigE_CAMERA_COMMUNICATION_FAILED                  = -10,
    SVGigE_CAMERA_REGISTER_ACCESS_DENIED                = -11,
    SVGigE_UNKNOWN_LUT_MODE                             = -12,
    SVGigE_STREAMING_FUNCTION_ALREADY_REGISTERED        = -13,
    SVGigE_STREAMING_NOT_INITIALIZED                    = -14,
    SVGigE_OUT_OF_MEMORY                                = -15,
    SVGigE_INVALID_CALLBACK_INITIALIZATION              = -16,
    SVGigE_INVALID_CALLBACK_FUNCTION_POINTER            = -17,
    SVGigE_IMAGE_NOT_AVAILABLE                          = -18,
    SVGigE_INSUFFICIENT_RGB_BUFFER_PROVIDED             = -19,
    SVGigE_STREAMING_CHANNEL_NOT_AVAILABLE              = -20,
    SVGigE_INVALID_PARAMETERS                           = -21,
    SVGigE_PIXEL_TYPE_NOT_SUPPORTED                     = -22,
    SVGigE_FILE_COULD_NOT_BE_OPENED                     = -23,
    SVGigE_TRANSPORT_LAYER_NOT_AVAILABLE                = -24,
    SVGigE_XML_FILE_NOT_AVAILABLE                       = -25,
    SVGigE_WHITE_BALANCE_FAILED                         = -26,
    SVGigE_DEPRECATED_FUNCTION                          = -27,
    SVGigE_WRONG_DESTINATION_BUFFER_SIZE                = -28,
    SVGigE_INSUFFICIENT_DESTINATION_BUFFER_SIZE         = -29,
    SVGigE_CAMERA_NOT_IN_CURRENT_SUBNET                 = -30,
    SVGigE_CAMERA_MOVED_TO_FOREIGN_SUBNET               = -31,

    // Mapped camera return codes
    SVGigE_SVCAM_STATUS_ERROR                           = -101,
    SVGigE_SVCAM_STATUS_SOCKET_ERROR                    = -102,
    SVGigE_SVCAM_STATUS_ALREADY_CONNECTED		            = -103,
    SVGigE_SVCAM_STATUS_INVALID_MAC					            = -104,
    SVGigE_SVCAM_STATUS_UNREACHABLE					            = -105,
    SVGigE_SVCAM_STATUS_ACCESS_DENIED					          = -106,
    SVGigE_SVCAM_STATUS_BUSY					                  = -107,
    SVGigE_SVCAM_STATUS_LOCAL_PROBLEM					          = -108,
    SVGigE_SVCAM_STATUS_MSG_MISMATCH					          = -109,
    SVGigE_SVCAM_STATUS_PROTOCOL_ID_MISMATCH            = -110,
    SVGigE_SVCAM_STATUS_NOT_ACKNOWLEDGED                = -111,
    SVGigE_SVCAM_STATUS_RECEIVE_LENGTH_MISMATCH         = -112,
    SVGigE_SVCAM_STATUS_ACKNOWLEDGED_LENGTH_MISMATCH    = -113,
    SVGigE_SVCAM_STATUS_NO_ACK_BUFFER_PROVIDED          = -114,
    SVGigE_SVCAM_STATUS_CONNECTION_LOST                 = -115,
    SVGigE_SVCAM_STATUS_TL_NOT_AVAILABLE                = -116,
    SVGigE_SVCAM_STATUS_DRIVER_VERSION_MISMATCH         = -117,
    SVGigE_SVCAM_STATUS_FEATURE_NOT_SUPPORTED           = -118,
    SVGigE_SVCAM_STATUS_PENDING_CHANNEL_DETECTED        = -119,
    SVGigE_SVCAM_STATUS_LUT_NOT_AVAILABLE               = -120,
    SVGigE_SVCAM_STATUS_LUT_SIZE_MISMATCH               = -121,
    SVGigE_SVCAM_STATUS_NO_MATCHING_IP_ADDRESS          = -122,
    SVGigE_SVCAM_STATUS_DISCOVERY_INFO_CHANGED          = -123,
    SVGigE_SVCAM_STATUS_FIRMWARE_UPGRADE_REQUIRED       = -124,
    SVGigE_SVCAM_STATUS_CAMERA_COMMUNICATION_ERROR      = -199,

    // Mapped transport layer return codes
    SVGigE_TL_DLL_NOT_LOADED                            = -201,
    SVGigE_TL_DLL_VERSION_MISMATCH                      = -202,
    SVGigE_TL_DLL_ALIGNMENT_PROBLEM                     = -203,
    SVGigE_TL_OPERATING_SYSTEM_NOT_SUPPORTED            = -204,
    SVGigE_TL_WINSOCK_INITIALIZATION_FAILED						  = -205,
    SVGigE_TL_CAMERA_NOT_FOUND                          = -206,
    SVGigE_TL_CAMERA_OPEN_FAILED                        = -207,
    SVGigE_TL_CAMERA_NOT_OPEN                           = -208,
    SVGigE_TL_CAMERA_UNKNWON_COMMAND                    = -209,
    SVGigE_TL_CAMERA_PAYLOAD_LENGTH_EXCEEDED            = -210,
    SVGigE_TL_CAMERA_PAYLOAD_LENGTH_INVALID             = -211,
    SVGigE_TL_CAMERA_COMMUNICATION_TIMEOUT              = -212,
    SVGigE_TL_CAMERA_ACCESS_DENIED						          = -213,
    SVGigE_TL_CAMERA_CONNECTION_LOST					          = -214,
    SVGigE_TL_STREAMING_FUNCTION_ALREADY_REGISTERED     = -215,
    SVGigE_TL_NO_STREAMING_PORT_FOUND                   = -216,
    SVGigE_TL_OUT_OF_MEMORY                             = -217,
    SVGigE_TL_INVALID_CALLBACK_FUNCTION_POINTER         = -218,
    SVGigE_TL_STREAMING_CHANNEL_NOT_AVAILABLE           = -219,
    SVGigE_TL_STREAMING_CHANNEL_VERSION_MISMATCH        = -220,
    SVGigE_TL_CALLBACK_INVALID_PARAMETER                = -221,
    SVGigE_TL_CALLBACK_IMAGE_DATA_MISSING               = -222,
    SVGigE_TL_OPENING_STREAMING_CHANNEL_FAILED          = -223,
    SVGigE_TL_CHANNEL_CREATION_FAILED                   = -224,
    SVGigE_TL_DRIVER_NOT_INSTALLED                      = -225,
    SVGigE_TL_PENDING_CHANNEL_DETECTED                  = -226,

  } SVGigE_RETURN;

	/** GigE transport layer type.
	 *  The SVGigE functionality is exposed through a transport layer which
	 *  connects to cameras in the network and which delivers images as well
	 *  as signals.
	 *  There are different types of transport layers, e.g. a NDIS filter
	 *  driver which assembles images from particular network packets in an
	 *  efficient way. A transport layer based on Winsock is another kind of
	 *  transport layer which has a higher CPU load footprint but does not have
	 *  a need for a driver being installed.
	 *  It is recommended to use a driver for performance and reliability reasons.
	 */
	typedef enum
	{
		SVGigETL_TypeNone				= 0,
		SVGigETL_TypeFilter			= 1,
		SVGigETL_TypeWinsock		= 2,

	}SVGigETL_Type;

  /** Camera feature information.
   *  A camera can support several items from the following set of camera features.
   */
  typedef enum
  {
    CAMERA_FEATURE_SOFTWARE_TRIGGER                    = 0,  // camera can be triggered by software
    CAMERA_FEATURE_HARDWARE_TRIGGER                    = 1,  // hardware trigger supported as well as trigger polarity
    CAMERA_FEATURE_HARDWARE_TRIGGER_EXTERNAL_EXPOSURE  = 2,  // hardware trigger with internal exposure supported as well as trigger polarity
    CAMERA_FEATURE_FRAMERATE_IN_FREERUNNING_MODE       = 3,  // framerate can be adjusted (in free-running mode)
    CAMERA_FEATURE_EXPOSURE_TIME                       = 4,  // exposure time can be adjusted
    CAMERA_FEATURE_EXPOSURE_DELAY                      = 5,  // exposure delay can be adjusted
    CAMERA_FEATURE_STROBE                              = 6,  // strobe is supported (polarity, duration and delay)
    CAMERA_FEATURE_AUTOGAIN                            = 7,  // autogain is supported
    CAMERA_FEATURE_ADCGAIN                             = 8,  // the ADC's gain can be adjusted
    CAMERA_FEATURE_AOI                                 = 9,  // image acquisition can be done for an AOI (area of interest)
    CAMERA_FEATURE_BINNING                             = 10, // binning is supported
    CAMERA_FEATURE_UPDATE_REGISTER                     = 11, // streaming channel related registers can be pre-set and then updated at once (e.g. for changing an AOI)
    CAMERA_FEATURE_NOT_USED                            = 12, // not in use
    CAMERA_FEATURE_COLORDEPTH_8BPP                     = 13, // a pixel depth of 8bit is supported
    CAMERA_FEATURE_COLORDEPTH_10BPP                    = 14, // a pixel depth of 10bit is supported
    CAMERA_FEATURE_COLORDEPTH_12BPP                    = 15, // a pixel depth of 12bit is supported
    CAMERA_FEATURE_COLORDEPTH_16BPP                    = 16, // a pixel depth of 16bit is supported
    CAMERA_FEATURE_ADCOFFSET                           = 17, // the ADC's offset can be adjusted
    CAMERA_FEATURE_SENSORDATA                          = 18, // the camera's sensor/ADC settings can be adjusted (the factory settings)
    CAMERA_FEATURE_WHITEBALANCE                        = 19, // a LUT for whitebalancing is available
    CAMERA_FEATURE_LUT_10TO8                           = 20, // a LUT from 10 bit to 8 bit is available
    CAMERA_FEATURE_LUT_12TO8                           = 21, // a LUT from 12 bit to 8 bit is available
    CAMERA_FEATURES_FLAGS           									 = 22, // streaming state and image availability can be queried from camera
    CAMERA_FEATURES_READOUT_CONTROL      							 = 23, // time of image read out from camera can be controlled by application
    CAMERA_FEATURES_TAP_CONFIG         								 = 24, // the tap configuration can be changed (switching between one and two taps)
    CAMERA_FEATURES_ACQUISITION        								 = 25, // acquisition can be controlled by start/stop

  } CAMERA_FEATURE;


  /** Look-up table mode.
   *  A camera can be instructed to apply a look-up table. Usually this will
   *  be used for running a gamma correction. However, other goals can also
   *  be implemented by a look-up table, like converting a gray-scale picture 
   *  into a binary black/white picture.
   */
  typedef enum
  {
    LUT_MODE_DISABLED               = 0,
    LUT_MODE_WHITE_BALANCE          = 1,    // 2006-12-20: deactivated, use  
                                            // Camera_setWhiteBalance() instead
    LUT_MODE_ENABLED                = 2,

  } LUT_MODE;

  /** Binning mode.
   *  A camera can be set to one of the following pre-defined binning modes
   */
  typedef enum
  {
    BINNING_MODE_OFF =0,
    BINNING_MODE_HORIZONTAL,
    BINNING_MODE_VERTICAL,
    BINNING_MODE_2x2,

  } BINNING_MODE;

  /** Acquisition mode.
   *  A camera can be set to one of the following acquisition modes
   */
  typedef enum
  {
    ACQUISITION_MODE_NO_ACQUISITION             = 0,
    ACQUISITION_MODE_FREE_RUNNING               = 1,  // 2008-05-06: deprecated, replaced by "fixed frequency"
    ACQUISITION_MODE_FIXED_FREQUENCY            = 1,
    ACQUISITION_MODE_INT_TRIGGER                = 2,  // 2008-05-06: deprecated, replaced by "software trigger"
    ACQUISITION_MODE_SOFTWARE_TRIGGER           = 2,
    ACQUISITION_MODE_EXT_TRIGGER_INT_EXPOSURE   = 3,
    ACQUISITION_MODE_EXT_TRIGGER_EXT_EXPOSURE   = 4,

  } ACQUISITION_MODE;

  /** Acquisition control
   *  A camera can be set to the following acquisition control modes
   */
  typedef enum
  {
    ACQUISITION_CONTROL_STOP            = 0,
    ACQUISITION_CONTROL_START           = 1,
    
  } ACQUISITION_CONTROL;

  /** Trigger polarity.
   *  A camera can be set to positive or negative trigger polarity
   */
  typedef enum
  {
    TRIGGER_POLARITY_POSITIVE =0,
    TRIGGER_POLARITY_NEGATIVE,

  } TRIGGER_POLARITY;

  /** Strobe polarity.
   *  A camera can be set to positive or negative strobe polarity
   */
  typedef enum
  {
    STROBE_POLARITY_POSITIVE =0,
    STROBE_POLARITY_NEGATIVE,

  } STROBE_POLARITY;

  /** Bayer conversion method
   */
  typedef enum
  {
    BAYER_METHOD_NONE = -1,
    BAYER_METHOD_NEAREST = 0,
    BAYER_METHOD_SIMPLE,
    BAYER_METHOD_BILINEAR,
    BAYER_METHOD_HQLINEAR,
    BAYER_METHOD_EDGESENSE,

  } BAYER_METHOD;

  /** Streaming channel callback function.
   *  The user-defined streaming channel callback function will be called each
   *  time when an image acquisition from camera has been finished and the image
   *  is ready for processing
   *
   *  NOTE: The callback function will return a NULL pointer in case an image
   *        could not be completely received over the network, e.g. in the result
   *        of insufficient bandwidth
   */
  typedef SVGigE_RETURN (__stdcall *StreamCallback)(Image_handle Image, void* Context);

  /** Pixel depth defines.
   *  The following pixel depths can be supported by a camera
   */
  typedef enum
  {
    SVGIGE_PIXEL_DEPTH_8     =0,
    //SVGIGE_PIXEL_DEPTH_10    =1, // not supported
    SVGIGE_PIXEL_DEPTH_12    =2,
    SVGIGE_PIXEL_DEPTH_16    =3,

  }SVGIGE_PIXEL_DEPTH;

  /** Pixel type defines.
   *  The pixel type as it is defined in the GigE Vision specification
   */

  // Indicate that pixel is monochrome
  #define GVSP_PIX_MONO                       0x01000000
  #define GVSP_PIX_RGB                        0x02000000

  // Indicate effective number of bits occupied by the pixel (including padding).
  // This can be used to compute amount of memory required to store an image.
  #define GVSP_PIX_OCCUPY8BIT                 0x00080000
  #define GVSP_PIX_OCCUPY12BIT                0x000C0000
  #define GVSP_PIX_OCCUPY16BIT                0x00100000
  #define GVSP_PIX_OCCUPY24BIT                0x00180000

  // Bit masks
  #define GVSP_PIX_COLOR_MASK                 0xFF000000
  #define GVSP_PIX_EFFECTIVE_PIXELSIZE_MASK   0x00FF0000
  #define GVSP_PIX_ID_MASK                    0x0000FFFF

  // Bit shift value
  #define GVSP_PIX_EFFECTIVE_PIXELSIZE_SHIFT  16

  typedef enum
  {
    // Unknown pixel format
    GVSP_PIX_UNKNOWN          = 0x0000,

    // Mono buffer format defines
    GVSP_PIX_MONO8            = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT  | 0x0001),
    GVSP_PIX_MONO10           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0003),
    GVSP_PIX_MONO10_PACKED    = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT | 0x0004),
    GVSP_PIX_MONO12           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0005),
    GVSP_PIX_MONO12_PACKED    = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY12BIT | 0x0006),
    GVSP_PIX_MONO16           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0007),

    // Bayer buffer format defines
    GVSP_PIX_BAYGR8           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT  | 0x0008),
    GVSP_PIX_BAYRG8           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT  | 0x0009),
    GVSP_PIX_BAYGB8           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT  | 0x000A),
    GVSP_PIX_BAYBG8           = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY8BIT  | 0x000B),
    GVSP_PIX_BAYGR10          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x000C),
    GVSP_PIX_BAYRG10          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x000D),
    GVSP_PIX_BAYGB10          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x000E),
    GVSP_PIX_BAYBG10          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x000F),
    GVSP_PIX_BAYGR12          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0010),
    GVSP_PIX_BAYRG12          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0011),
    GVSP_PIX_BAYGB12          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0012),
    GVSP_PIX_BAYBG12          = (GVSP_PIX_MONO | GVSP_PIX_OCCUPY16BIT | 0x0013),

    // Color buffer format defines
    GVSP_PIX_RGB24            = (GVSP_PIX_RGB  | GVSP_PIX_OCCUPY24BIT),

  } GVSP_PIXEL_TYPE;

  /** Signal types
   *  Each image that is delivered to an application by a callback will have a related signal
   *  which informs about the circumstances why a callback was triggered.
   *  
   *  Usually a complete image will be delivered with the SVGigE_SIGNAL_FRAME_COMPLETED signal.
   */
  typedef enum
  {
    SVGigE_SIGNAL_NONE             			    = 0,
    SVGigE_SIGNAL_FRAME_COMPLETED 			    = 1,
    SVGigE_SIGNAL_FRAME_ABANDONED 			    = 2,
    SVGigE_SIGNAL_START_OF_TRANSFER			    = 3,
    SVGigE_SIGNAL_BANDWIDTH_EXCEEDED  	    = 4,
    SVGigE_SIGNAL_OLD_STYLE_DATA_PACKETS  	= 5,
    SVGigE_SIGNAL_TEST_PACKET						    = 6,
    SVGigE_SIGNAL_MESSAGE   						    = 7,
    SVGigE_SIGNAL_CAMERA_CONNECTION_LOST    = 8,

  } SVGigE_SIGNAL_TYPE;


#ifdef __USRDLL__
  #define __usrdllexport__  __declspec(dllexport)
#else
  #define __usrdllexport__
#endif


//------------------------------------------------------------------------------
// GigE DLL
//------------------------------------------------------------------------------

  /** isLoadedGigEDLL
   *  A check is performed if the GigE DLL has already been loaded and if this
   *  is not the case it will be tried to load it.
   */
  bool isLoadedGigEDLL();

  /** isVersionCompliant.
   *  The DLL's version at compile time will be checked against an expected
   *  version at runtime. The calling program knows the runtime version that
   *  it needs for proper functioning and can handle a version mismatch, e.g.
   *  by displaying the expected and the found DLL version to the user.
   *
   *  @param DllVersion a pointer to a version structure for the current DLL version
   *  @param ExpectedVersion a pointer to a version structure with the expected DLL version
   *  @return SVGigE_SUCCESS or an appropriate SVGigE error code
   */
  __usrdllexport__ SVGigE_RETURN
  isVersionCompliantDLL(SVGigE_VERSION *DllVersion,
                        SVGigE_VERSION *ExpectedVersion);

//------------------------------------------------------------------------------
// Camera container
//------------------------------------------------------------------------------

  /** Create camera container.
   *  A handle is obtained that references a management object for discovering
   *  and selecting GigE cameras in the network segments that a computer is
   *  connected to.
   *
	 *  @TransportLayerType the type of the transport layer DLL to be used
   *  @return on success a handle for subsequent camera container function calls
   *         or -1 if creating a camera container failed
   */
  __usrdllexport__ CameraContainerClient_handle
  CameraContainer_create(SVGigETL_Type TransportLayerType);

  /** Delete Camera container.
   *  A previously created camera container reference will be released. After
   *  deleting a camera container all camera specific functions are no longer
   *  available
   *
   *  @param hCameraContainer a handle received from CameraContainer_create()
   */
  __usrdllexport__ SVGigE_RETURN
  CameraContainer_delete(CameraContainerClient_handle hCameraContainer);

  /** Device discovery.
   *  All network segments that a computer is connected to will be serached for
   *  GigE cameras by sending out a network broadcast and subsequently analyzing
   *  camera responses.
   *
   *  @param hCameraContainer a handle received from CameraContainer_create()
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  CameraContainer_discovery(CameraContainerClient_handle hCameraContainer);

  /** Get number of connected cameras.
   *  The maximal index is returned that cameras can be accessed with in the
   *  camera container
   *
   *  @param hCameraContainer a handle received from CameraContainer_create()
   *  @return number of available cameras
   */
  __usrdllexport__ int
  CameraContainer_getNumberOfCameras(CameraContainerClient_handle hCameraContainer);

  /** Get camera.
   *  A camera handle is obtained accordingly to an index that was specified
   *  as an input parameter
   *
   *  @param hCameraContainer a handle received from CameraContainer_create()
   *  @param CameraIndex camera index from zero to one less the number of available cameras
   *  @return a handle for subsequent camera function calls or NULL in case the
   *         index was specified out of range
   */
  __usrdllexport__ Camera_handle
  CameraContainer_getCamera(CameraContainerClient_handle hCameraContainer,
                            int CameraIndex);

  /** Find camera.
   *  A camera handle is obtained accordingly to a search string that will be
   *  matched against the following items:
   *   - MAC address
   *   - IP address
   *   - serial number
   *   - user defined name
   *   If one of those items can be matched a valid handle is returned.
   *   Otherwise a SVGigE_NO_CAMERA return value will be generated
   *
   *  @param hCameraContainer a handle received from CameraContainer_create()
   *  @param CameraItem a string for matching it against the above items
   *  @return a handle for subsequent camera function calls or NULL in case the
   *          index was specified out of range
   */
  __usrdllexport__ Camera_handle
  CameraContainer_findCamera(CameraContainerClient_handle hCameraContainer,
                             char *CameraItem);


//------------------------------------------------------------------------------
// Camera
//------------------------------------------------------------------------------

  /** Force valid network settings
   *  A camera's availability will be evaluated. If it is outside current subnet 
   *  then it will be forced to valid network settings inside current subnet. 
   *  Valid network settings will be reported back to caller.
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_forceValidNetworkSettings(Camera_handle hCamera, 
                                   unsigned int *IPAddress,
                                   unsigned int *SubnetMask);

  /** Open connection to camera.
   *  A TCP/IP control channel will be established.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Timeout the time without traffic or heartbeat after which a camera drops a connection (default: 3.0 sec.)
   *                NOTE: Values between 0.0 to 0.5 sec. will be mapped to the default value (3.0 sec.)
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN 
  Camera_openConnection(Camera_handle hCamera, float Timeout);

  /** Disconnect camera.
   *  The TCP/IP control channel will be closed
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_closeConnection(Camera_handle hCamera);

//---Camera-information---------------------------------------------------------

  /** Get manufacturer name.
   *  The manufacturer name that is obtained from the camera firmware will be
   *  returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return a string containing requested information
   */
  __usrdllexport__ char *
  Camera_getManufacturerName(Camera_handle hCamera);

  /** Get model name.
   *  The model name that is obtained from the camera firmware will be
   *  returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return a string containing requested information
   */
  __usrdllexport__ char *
  Camera_getModelName(Camera_handle hCamera);

  /** Get device version.
   *  The device version that is obtained from the camera firmware will be
   *  returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return a string containing requested information
   */
  __usrdllexport__ char *
  Camera_getDeviceVersion(Camera_handle hCamera);

  /** Get manufacturer specific information.
   *  The manufacturer specific information that is obtained from the camera
   *  firmware will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return a string containing requested information
   */
  __usrdllexport__ char *
  Camera_getManufacturerSpecificInformation(Camera_handle hCamera);

  /** Get serial number.
   *  The (manufacturer-assigned) serial number will be obtained from the camera
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return a string containing requested information
   */
  __usrdllexport__ char *
  Camera_getSerialNumber(Camera_handle hCamera);

  /** Set user-defined name
   *  A user-defined name will be assigned permanently to a camera
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param UserDefinedName the name to be transferred to the camera
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setUserDefinedName(Camera_handle hCamera, char *UserDefinedName);

  /** Get user-defined name
   *  A name that has been assigned to a camera by the user will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return a string containing requested information
   */
  __usrdllexport__ char *
  Camera_getUserDefinedName(Camera_handle hCamera);

  /** Get MAC address.
   *  The MAC address that is obtained from the camera firmware will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return a string containing requested information
   */
  __usrdllexport__ char *
  Camera_getMacAddress(Camera_handle hCamera);

  /** Set IP address.
   *  The camera will get a new persistent IP address assigned. If the camera
   *  is currently unavailable in the subent where it is attached to, then a
   *  temporary IP address will be forced into the camera first. In any case
   *  the camera will have the new IP address assigned as a persistent IP 
   *  which will apply after camera's next reboot.
   *
   *  HINT:
   *  If an IP address is set that is not inside the subnet where the camera
   *  is currently connected to, then the camera becomes unavailable after next
   *  reboot. This can be avoided by having a valid IP address assigned automatically 
   *  by setting both values to zero, IPAddress and NetMask
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param IPAddress a valid IP address or zero (for automatically assigning a valid IP/netmask)
   *  @param NetMask a matching net mask or zero (for automatically assigning a valid IP/netmask)
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setIPAddress(Camera_handle hCamera,
                      unsigned int IPAddress,
                      unsigned int NetMask);

  /** Get IP address.
   *  The IP address that the camera is currently working on will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return success or error code
   */
  __usrdllexport__ char *
  Camera_getIPAddress(Camera_handle hCamera);

  /** Evaluate maximal packet size.
   *  A test will be performed which determines the maximal usable packet size
   *  based on given network hardware. This value will be used when opening a
   *  streaming channel.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
	 *  @param MaximalPacketSize the maximal possible packet size without fragmentation
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN 
  Camera_evaluateMaximalPacketSize(Camera_handle hCamera,
                                   int *MaximalPacketSize); 

	/** Set streaming packet size.
	 *  The packet size is set which will be generated by the camera for streaming
	 *  image data as payload packets to the host
	 *
	 *  WARNING! Explicitly setting network packet size to values above 1500 bytes
	 *           may provide to unpredictable results and also to a system crash.
	 *           Please use "Camera_evaluateMaximalPacketSize" for a save adjustment
	 *           to jumbo packets.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
	 *  @param StreamingPacketSize the packet size used bv the camera for image packets
   *  @return success or error code
	 */
  __usrdllexport__ SVGigE_RETURN
  Camera_setStreamingPacketSize(Camera_handle hCamera,
                                int StreamingPacketSize);

  /** Get imager width.
   *  The imager width will be retrieved from the camera
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param ImagerWidth a reference to the imager width value
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getImagerWidth(Camera_handle hCamera,
                        int *ImagerWidth);

  /** Get imager height.
   *  The imager height will be retrieved from the camera
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param ImagerHeight a reference to the imager height value
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getImagerHeight(Camera_handle hCamera,
                         int *ImagerHeight);

  /** Get size X.
   *  The currently used horizontal picture size X will be retrieved from the camera
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param SizeX a reference to the size X value
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getSizeX(Camera_handle hCamera,
                  int *SizeX);

  /** Get size Y.
   *  The currently used vertical picture size Y will be retrieved from the camera
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param SizeY a reference to the size Y value
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getSizeY(Camera_handle hCamera,
                  int *SizeY);

  /** Get pitch.
   *  The number of bytes in a row will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Pitch a reference to the pitch value
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getPitch(Camera_handle hCamera,
                  int *Pitch);

  /** Get image size.
   *  The number of bytes in an image will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param ImageSize a reference to the image size value
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getImageSize(Camera_handle hCamera,
                      int *ImageSize);

  /** Get pixel clock.
   *  The camera's pixel clock will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param PixelClock a reference to the pixel clock value
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getPixelClock(Camera_handle hCamera,
                       int *PixelClock);

  /** Get pixel type.
   *  The pixel type will be retrieved from the camera
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param PixelType a reference to the pixel type value
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getPixelType(Camera_handle hCamera,
                      GVSP_PIXEL_TYPE *PixelType);

  /** Set pixel depth.
   *  The number of bits for a pixel will be set to 8, 12 or 16 bits. Before this function
   *  is called the camera's feature vector should be queried whether the desired pixel depth
   *  is supported
   *
   *  @see Camera_isCameraFeature()
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param PixelDepth the intended value for pixel depth
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setPixelDepth(Camera_handle hCamera,
                       SVGIGE_PIXEL_DEPTH PixelDepth);

  /** Get pixel depth.
   *  The camera's current setting for pixel depth will be queried.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param PixelDepth an enum for the number of bits in a pixel will be returned
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getPixelDepth(Camera_handle hCamera,
                       SVGIGE_PIXEL_DEPTH *PixelDepth);

  /** Is camera feature.
   *  The camera will be queried whether a feature is available or not.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Feature a feature which availability will be determined
   *  @return a boolean value indicating whether the queried feature is available or not
   */
  __usrdllexport__ bool
  Camera_isCameraFeature(Camera_handle hCamera, CAMERA_FEATURE Feature);

  /** Read XML file.
   *  The camera's XML file will be retrieved and made available for further
   *  processing by an application. The returned pointer to the content of the
   *  XML file will be valid until the function is called again or until
   *  the camera is closed.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param XML a pointer to a zero-terminated string containing the complete XML file
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_readXMLFile(Camera_handle hCamera, char **XML);

//---Main-camera-control-(framerate,exposure,gain)------------------------------

  /** Set framerate.
   *  The camera will be adjusted to a new framerate
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Framerate new framerate in 1/s
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setFrameRate(Camera_handle hCamera,
                      float Framerate);

  /** Get framerate.
   *  The currently programmed framerate will be retrieved
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param ProgrammedFramerate currently programmed framerate in 1/s
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getFrameRate(Camera_handle hCamera,
                      float *ProgrammedFramerate);

  /** Get framerate range.
   *  The currently available framerate range will be retrieved
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param MinFramerate currently available minimal framerate in 1/s
   *  @param MaxFramerate currently available maximal framerate in 1/s
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getFrameRateRange(Camera_handle hCamera,
                           float *MinFramerate,
                           float *MaxFramerate);

  /** Set exposure time.
   *  The camera will be adjusted to a new exposure time
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param ExposureTime new exposure time in microseconds
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setExposureTime(Camera_handle hCamera,
                         float ExposureTime);

  /** Get exposure time.
   *  The currently programmed exposure time will be retrieved
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param ProgrammedExposureTime currently programmed exposure time in microseconds
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getExposureTime(Camera_handle hCamera,
                         float *ProgrammedExposureTime);

  /** Get exposure time range.
   *  The currently available exposure time range will be retrieved.
   *  NOTE: The received values will apply to free-running mode. In triggered mode the
   *        usual exposure time is limited to slightly more than 1 second. Exposure
   *        times above 1 second require changes in internal camera settings. Please
   *        contact SVS VISTEK if the camera needs to run in that exposure time range.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param MinExposureTime currently available minimal exposure time in microseconds
   *  @param MaxExposureTime currently available maximal exposure time in microseconds
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getExposureTimeRange(Camera_handle hCamera,
                              float *MinExposureTime,
                              float *MaxExposureTime);

  /** Set gain.
   *  The camera will be adjusted to a new analog gain
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Gain new analog gain (0..18 dB)
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setGain(Camera_handle hCamera,
                 float Gain);

  /** Get gain.
   *  The currently programmed analog gain will be retrieved
   *  Note: A gain of 1.0 is represented as integer 128 in the appropriate camera
   *  register. Thus the gain can be adjusted only in discrete steps.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param ProgrammedGain currently programmed analog gain (0..18 dB)
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getGain(Camera_handle hCamera,
                 float *ProgrammedGain);

  /** Set offset
   *  The ofset value will be set to the provided value
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Offset the new value for pixel offset (0..255)
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setOffset(Camera_handle hCamera,
                   float Offset);

  /** Get offset
   *  The current offset value will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Offset the currently programmed value for pixel offset (0..255)
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getOffset(Camera_handle hCamera,
                   float *ProgrammedOffset);

//---White-Balance--/--Look-up-table--------------------------------------------

  /** setWhiteBalance.
   *  The provided values will be applied for white balance.
   *
   *  NOTE: The color component strength for Red, Green and Blue can either be
   *        provided by user or they can conveniently be calculated inside an image
   *        callback using the Image_estimateWhiteBalance() function.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Red balanced value for red color
   *  @param Green balanced value for green color
   *  @param Blue balanced value for blue color
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setWhiteBalance(Camera_handle hCamera,
                         float Red,
                         float Green ,
                         float Blue);

  /** getWhiteBalance.
   *  Currently set values for white balance will be returned.
   *  Previously adjusted values will be returned either unchanged or adjusted
   *  if necessary. The returned values will be 100 and above where the color
   *  which got 100 assigned will be transferred unchanged, however two
   *  other color components might be enhanced above 100 for each image.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Red balanced value for red color
   *  @param Green balanced value for green color
   *  @param Blue balanced value for blue color
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getWhiteBalance(Camera_handle hCamera,
                         float *Red,
                         float *Green ,
                         float *Blue);

  /** setLookupTableMode.
   *  The look-up table mode will be switched on or off
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param LUTMode new setting for look-up table mode
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setLookupTableMode(Camera_handle hCamera,
                            LUT_MODE LUTMode);

  /** Get look-up table mode.
   *  The currently programmed look-up table mode will be retrieved
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param ProgrammedLUTMode currently programmed look-up table mode
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getLookupTableMode(Camera_handle hCamera,
                            LUT_MODE *ProgrammedLUTMode);

  /** setLookupTable.
   *  A user-defined lookup table will be uploaded to the camera. The size has to match
   *  the lookup table size that is supported by the camera (1024 for 10to8 or 4096 for 12to8). 
   *
   *  @see Camera_isCameraFeature()
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param LookupTable an array of user-defined lookup table values (bytes)
   *  @param LookupTableSize the size of the user-defined lookup table
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setLookupTable(Camera_handle hCamera,
                        unsigned char *LookupTable,
                        int LookupTableSize);

  /** getLookupTable.
   *  The currently installed lookup table will be downloaded from the camera. The size of the 
   *  reserved download space has to match the lookup table size (1024 for 10to8 or 4096 for 12to8).
   *
   *  @see Camera_isCameraFeature()
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param ProgrammedLookupTable an array for downloading the lookup table from camera
   *  @param LookupTableSize the size of the provided lookup table space
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getLookupTable(Camera_handle hCamera,
                        unsigned char *ProgrammedLookupTable,
                        int LookupTableSize);

  /** Create look-up table.
   *
   *  DEACTIVATED !
   *  2006-12-20: White balance has been re-implemented by the above Camera_setWhiteBalance() 
   *              function and a separate lookup table can be uploaded by setLookupTable().
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_createLUTwhiteBalance(Camera_handle hCamera, float Red, float Green, float Blue);

//----Binning-/-AOI-------------------------------------------------------------

  /** Set binning mode.
   *  The camera's binning mode will be set to the selected value
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param BinningMode the new setting for binning mode
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setBinningMode(Camera_handle hCamera,
                        BINNING_MODE BinningMode);

  /** Get binning mode.
   *  The camera's current binning mode will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param BinningMode the currently programmed setting for binning mode
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getBinningMode(Camera_handle hCamera,
                        BINNING_MODE *ProgrammedBinningMode);

  /** Set area of interest (AOI)
   *  The camera will be switched to partial scan mode and an AOI will be set
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param SizeX the number of pixels in one row
   *  @param SizeY the number of scan lines
   *  @param OffsetX a left side offset of the scanned area
   *  @param OffsetY an upper side offset of the scanned area
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN 
  Camera_setAreaOfInterest(Camera_handle hCamera,
                           int SizeX,
                           int SizeY,
                           int OffsetX,
                           int OffsetY);

  /** Get area of interest (AOI)
   *  The currently set parameters for partial scan will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param SizeX the number of pixels in one row
   *  @param SizeY the number of scan lines
   *  @param OffsetX a left side offset of the scanned area
   *  @param OffsetY an upper side offset of the scanned area
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getAreaOfInterest(Camera_handle hCamera,
                           int *ProgrammedSizeX,
                           int *ProgrammedSizeY,
                           int *ProgrammedOffsetX,
                           int *ProgrammedOffsetY);



//---Multi-Tap-Configuration----------------------------------------------------

  /** Save tap balance settings.
   *  Current settings for tap balance will be saved into a XML file. Usually the
   *  tap balance is adjusted during camera production. Whenever a need exists for
   *  changing those factory settings, e.g. for balancing image sensor characteristics
   *  dependent on gain, particular settings can be determined, saved to files and
   *  later loaded on demand.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Filename a complete path and filename where to save the settings
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_saveTapBalanceSettings(Camera_handle hCamera,
                                char *Filename);

  /** Load tap balance settings.
   *  New settings for tap balance will be loaded from a XML file.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Filename a complete path and filename where to load the settings from
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_loadTapBalanceSettings(Camera_handle hCamera,
                                char *Filename);

  /** Set tap calibration
   *  The provided tab calibration values will be written to camera. The calibration
   *  values apply to a particular tap. Thus adjusting all taps requires running this
   *  function multiple, once for each tap.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param TapID an identification number (0, 1) of the tap
   *  @param Gain the gain value for the ADC as integer
   *  @param Offset the offset value for the ADC as integer
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setTapCalibration(Camera_handle hCamera,
                           unsigned int TapID,
                           unsigned int Gain,
                           unsigned int Offset);

  /** Get tap calibration
   *  Current tab calibration values will be retrieved from camera
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param TapID an identification number (0, 1) of the tap
   *  @param Gain the gain value of the ADC will be returned
   *  @param Offset the offset value of the ADC will be returned
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getTapCalibration(Camera_handle hCamera,
                           unsigned int TapID,
                           unsigned int *Gain,
                           unsigned int *Offset);

  /** Set tap configuration
   *  The camera will be controlled for working with one or two taps
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param TapCount the number of taps (1, 2) to be used by the camera
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setTapConfiguration(Camera_handle hCamera,
                             int TapCount);

  /** Get tap configuration
   *  The camera will be queried whether it is working with one or two taps
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param TapCount the number of taps (1, 2) currently used by the camera is returned
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getTapConfiguration(Camera_handle hCamera,
                             int *TapCount);


//---Acquisition-modes-(trigger)------------------------------------------------

  /** Set acquisition mode and control.
   *  The camera's acquisition mode will be set to the selected value
   *  It can be determined whether camera control should switch to START immediately.
   *  First function always forces a START wheras second function allows to choose
   *  whether acquisition should start immediately or if previous control state
   *  will be preserved.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param AcquisitionMode the new setting for acquisition mode
   *  @param AcquisitionStart whether camera control switches to START immediately
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setAcquisitionMode(Camera_handle hCamera,
                            ACQUISITION_MODE AcquisitionMode);

  /*  */
  __usrdllexport__ SVGigE_RETURN
  Camera_setAcquisitionModeAndStart(Camera_handle hCamera,
                                    ACQUISITION_MODE AcquisitionMode,
                                    bool AcquisitionStart);

  /** Get acquisition mode.
   *  The camera's current acquisition mode will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param AcquisitionMode the currently programmed setting for acquisition mode
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getAcquisitionMode(Camera_handle hCamera,
                            ACQUISITION_MODE *ProgrammedAcquisitionMode);

  /** Set acquisition control.
   *  The camera's acquisition will be controlled (start/stop).
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param AcquisitionControl the new setting for acquisition control
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setAcquisitionControl(Camera_handle hCamera,
                               ACQUISITION_CONTROL AcquisitionControl);

  /** Get acquisition control.
   *  The camera's current acquisition control (start/stop) will be returned.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param AcquisitionControl the currently programmed setting for acquisition control
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getAcquisitionControl(Camera_handle hCamera,
                               ACQUISITION_CONTROL *AcquisitionControl);

  /** Start acquisition cycle.
   *  The camera will be triggerred for starting a new acquisition cycle.
   *  A mandatory pre-requisition for successfully starting an acquisition
   *  cycle by software is to have the camera set to ACQUISITION_MODE_SOFTWARE_TIGGER
   *  before
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_startAcquisitionCycle(Camera_handle hCamera);

  /** Set trigger polarity
   *  The camera's trigger polarity will be set to the selected value
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param TriggerPolarity the new setting for trigger polarity
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setTriggerPolarity(Camera_handle hCamera,
                            TRIGGER_POLARITY TriggerPolarity);

  /** Get trigger polarity
   *  The camera's current trigger polarity will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param TriggerPolarity the currently programmed setting for trigger polarity
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getTriggerPolarity(Camera_handle hCamera,
                            TRIGGER_POLARITY *ProgrammedTriggerPolarity);

  /** Set exposure delay
   *  The camera's exposure delay in micro seconds relative to the trigger
   *  pulse will be set to the provided value. The delay will become active
   *  each time an active edge of an internal or external trigger pulse arrives
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param ExposureDelay the new value for exposure delay in microseconds
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setExposureDelay(Camera_handle hCamera,
                           float ExposureDelay);

  /** Get exposure delay
   *  The camera's current exposure delay will be returned in micro seconds
   *  relative to the trigger pulse
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param ExposureDelay the currently programmed value for exposure delay in microseconds
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getExposureDelay(Camera_handle hCamera,
                          float *ProgrammedExposureDelay);

  /** Set inter-packet delay
   *  A delay between network packets will be introduced and set to a specified
   *  number of ticks.
   *
   *  NOTE: The resulting total image transfer time must not exceed 250 ms.
   *  Otherwise a timeout condition will be reached in the SVGigE driver.
   *  The dependency between inter-packet delay and total image transfer time
   *  depends on pixel clock, image size and has to be determine case by case.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param InterPackedDelay the new value for inter-packet delay (0..1000, relative number)
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setInterPacketDelay(Camera_handle hCamera,
                             float InterPacketDelay);

  /** Get inter-packet delay
   *  The delay between network packets will be returned as a relative number
   *  of ticks.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param InterPackedDelay the currently programmed value for inter-packet delay (0..1000, relative number)
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getInterPacketDelay(Camera_handle hCamera,
                             float *ProgrammedInterPacketDelay);

//---Strobe-control-------------------------------------------------------------

  /** Set strobe polarity
   *  The camera's strobe polarity will be set to the selected value
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param StrobePolarity the new setting for strobe polarity
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setStrobePolarity(Camera_handle hCamera,
                           STROBE_POLARITY StrobePolarity);

  /** Get strobe polarity
   *  The camera's current strobe polarity will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param StrobePolarity the currently programmed setting for strobe polarity
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getStrobePolarity(Camera_handle hCamera,
                           STROBE_POLARITY *ProgrammedStrobePolarity);

  /** Set strobe position
   *  The camera's strobe position in micro seconds relative to the trigger
   *  pulse will be set to the selected value
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param StrobePosition the new value for strobe position in microseconds
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setStrobePosition(Camera_handle hCamera,
                           float StrobePosition);

  /** Get strobe position
   *  The camera's current strobe position will be returned in micro seconds
   *  relative to the trigger pulse
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param StrobePosition the currently programmed value for strobe position in microseconds
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getStrobePosition(Camera_handle hCamera,
                           float *ProgrammedStrobePosition);

  /** Set strobe duration
   *  The camera's strobe duration in micro seconds will be set to the selected
   *  value
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param StrobeDuration the new value for strobe duration in microseconds
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setStrobeDuration(Camera_handle hCamera,
                           float StrobeDuration);

  /** Get strobe duration
   *  The camera's current strobe duration in micro seconds will be returned
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param StrobeDuration the currently programmed value for strobe duration in microseconds
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getStrobeDuration(Camera_handle hCamera,
                           float *ProgrammedStrobeDuration);

//---Special-functions----------------------------------------------------------

  /** Write EEPROM defaults.
   *  The current settings will be made the EEPROM defaults that will be
   *  restored on each camera start-up
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_writeEEPROM(Camera_handle hCamera);

  /** Read EEPROM defaults.
   *  The EEPROM content will be moved to the appropriate camera registers.
   *  This operation will restore the camera settings that were active when
   *  the EEPROM write function was performed
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_readEEPROM(Camera_handle hCamera);

  /** Restore factory defaults.
   *  The camera's registers will be restored to the factory defaults and at
   *  the same time those settings will be written as default to EEPROM
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_restoreFactoryDefaults(Camera_handle hCamera);

//---Timestamps-----------------------------------------------------------------

  /** Stamp timestamp.
   *  A hardware timestamp will be written into the selected camera register.
   *  The timestamp's actual value can be read out by Camera_getTimestamp()
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param TimestampIndex the index of the timestamp to be set by hardware
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_stampTimestamp(Camera_handle hCamera,
                        int TimestampIndex);

  /** Get timestamp.
   *  The value of a selected hardware timestamp will be returned. The index
   *  provided to the function is valid in the range between 0 and 8
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param TimestampIndex the index of the timestamp to be returned
   *  @param Timestamp the timestamp's value in seconds and part of a second
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getTimestamp(Camera_handle hCamera,
                      int TimestampIndex,
                      double *Timestamp);

//---Expert functions-----------------------------------------------------------

  /** Set GigE camera register.
   *  A register of a SVS GigE camera will be directly written to
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param RegisterAddress a valid address of a SVS GigE camera register
   *  @param RegisterValue a value that has to be written to selected register
   *  @param GigECameraAccessKey a valid key for directly accessing a GigE camera
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_setGigECameraRegister(Camera_handle hCamera,
                               unsigned int RegisterAddress,
                               unsigned int RegisterValue,
                               unsigned int GigECameraAccessKey);

  /** Get GigE camera register.
   *  A value from a SVS GigE camera register will be directly read out
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param RegisterAddress a valid address of a GigE camera register
   *  @param RegisterValue the current programmed value will be returned
   *  @param GigECameraAccessKey a valid key for directly accessing a GigE camera
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_getGigECameraRegister(Camera_handle hCamera,
                               unsigned int RegisterAddress,
                               unsigned int *ProgammedRegisterValue,
                               unsigned int GigECameraAccessKey);

  /** Write GigE camera memory.
   *  A block of data will be written to the memory of a SVS GigE camera
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param MemoryAddress a valid memory address in a SVS GigE camera
   *  @param DataBlock a block of data that has to be written to selected memory
   *  @param DataLength the length of the specified DataBlock
   *  @param GigECameraAccessKey a valid key for directly accessing a GigE camera
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_writeGigECameraMemory(Camera_handle hCamera,
                               unsigned int  MemoryAddress,
                               unsigned char *DataBlock,
                               unsigned int  DataLength,
                               unsigned int  GigECameraAccessKey);

  /** Read GigE camera memory.
   *  A block of data will be read from the memory of a SVS GigE camera
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param MemoryAddress a valid memory address in a SVS GigE camera
   *  @param DataBlock an address where the data from selected memory will be written to
   *  @param DataLength the data length to be read from the camera's memory
   *  @param GigECameraAccessKey a valid key for directly accessing a GigE camera
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Camera_readGigECameraMemory(Camera_handle hCamera,
                              unsigned int  MemoryAddress,
                              unsigned char *DataBlock,
                              unsigned int  DataLength,
                              unsigned int  GigECameraAccessKey);

  /** Register for log messages.
   *  Log messages can be requested for various log levels:
   *  0 - logging off
   *  1 - CRITICAL errors that prevent from further operation
   *  2 - ERRORs that prevent from proper functioning
   *  3 - WARNINGs which usually do not affect proper work
   *  4 - INFO for listing camera communication (default)
   *  5 - DIAGNOSTICS for investigating image callbacks
   *  6 - DEBUG for receiving multiple parameters for image callbacks
   *  7 - DETAIL for receiving multiple signals for each image callback
   *
   *  Resulting log messages can be either written into a log file
   *  respectively they can be received by a callback and further
   *  processed by an application.
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param LogLevel one of the above log levels
   *  @param LogFilename a filename where all log messages will be written to or NULL
   *  @param LogCallback a callback function that will receive all log messages or NULL
   *  @param MessageContext a context that will be returned to application with each callback or NULL
   *  @return success or error code
   */
  typedef void (__stdcall *LogMessageCallback)(char *LogMessage, void *MessageContext);

  __usrdllexport__ SVGigE_RETURN
  Camera_registerForLogMessages(Camera_handle hCamera,
                                int LogLevel =4,
                                char *LogFilename =NULL,
                                LogMessageCallback LogCallback =NULL,
                                void *MessageContext =NULL);


//------------------------------------------------------------------------------
// Streaming channel
//------------------------------------------------------------------------------

  /** Create streaming channel.
   *  A UDP streaming channel will be established for image data transfer.
   *  A connection to the camera has to be successfully opened first using
   *  Camera_openConnection() before a streaming channel can be established
   *
   *  @see Camera_openConnection()
   *  @param hStreamingChannel a handle for the streaming channel will be returned
   *  @param hCameraContainer a camera container handle received from CameraContainer_create()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param BufferCount specifies the number of image buffers (default: 3 buffers)
   *                    NOTE: A value of 0 will be mapped to the default value (3 buffers)
   *  @param CallbackFunction user-defined callback function for image processing
   *  @param Context user-defined data that will be returned on each callback
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  StreamingChannel_create(StreamingChannel_handle *hStreamingChannel,
                          CameraContainerClient_handle hCameraContainer,
                          Camera_handle hCamera,
                          int BufferCount,
                          StreamCallback CallbackFunction,
                          void *Context);

  /** Delete streaming channel.
   *  A streaming channel will be closed and all resources will be released that
   *  have been occupied by the streaming channel
   *
   *  @param hStreamingChannel a streaming channel handle received from StreamingChannel_create()
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  StreamingChannel_delete(StreamingChannel_handle hStreamingChannel);

  /** Get frame loss.
   *  The number of lost frames in a streaming channel will be returned
   *
   *  @param hStreamingChannel a streaming channel handle received from StreamingChannel_create()
   *  @ FrameLoss the number of frames that have been lost since the streaming channel was opened
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  StreamingChannel_getFrameLoss(StreamingChannel_handle hStreamingChannel,
                                int *FrameLoss);

  /** Get actual frame rate.
   *  The actual frame rate calculated from received images will be returned
   *
   *  @param hStreamingChannel a streaming channel handle received from StreamingChannel_create()
   *  @param ActualFrameRate the actual frame rate measured based on received images
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  StreamingChannel_getActualFrameRate(StreamingChannel_handle hStreamingChannel,
                                      float *ActualFrameRate);

  /** Get actual data rate.
   *  The actual data rate calculated from received image data will be returned
   *
   *  @param hStreamingChannel a streaming channel handle received from StreamingChannel_create()
   *  @param ActualFrameRate the actual frame rate measured based on received image data
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  StreamingChannel_getActualDataRate(StreamingChannel_handle hStreamingChannel,
                                     float *ActualDataRate);


//------------------------------------------------------------------------------
// Image
//------------------------------------------------------------------------------

  /** Release image.
   *  An image which availability was indicated by a StreamCallback function needs
   *  to be released after processing it by a user application in order to free
   *  the occupied buffer space for a subsequent image acquisition.
   *
   *  After releasing a picture the following access functions must not be
   *  called anymore using the released image handle
   *
   *  @param Image an image handle that was received from the callback function
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Image_release(Image_handle hImage);

  /** Get image pointer.
   *  A pointer to the image data will be returned
   *
   *  @param Image an image handle that was received from the callback function
   *  @return a pointer to the image data
   */
  __usrdllexport__ unsigned char *
  Image_getDataPointer(Image_handle hImage);

  /** Get camera handle
   *  A handle of the camera that captured the image will be returned
   *
   *  @param Image an image handle that was received from the callback function
   *  @return a camera handle
   */
  __usrdllexport__ Camera_handle
  Image_getCamera(Image_handle hImage);

  /** Get buffer index.
   *  The index of the current image buffer will be returned
   *
   *  @param Image an image handle that was received from the callback function
   *  @return a pointer to the image data
   */
  __usrdllexport__ int
  Image_getBufferIndex(Image_handle hImage);

  /** Get image RGB
   *
   *  The image will be converted by a selectable Bayer conversion algorithm into
   *  a RGB image. The caller needs to provide a sufficient buffer.
   *
   *  @param Image an image handle that was received from the callback function
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  Image_getImageRGB(Image_handle hImage,
                    unsigned char *BufferRGB,
                    int BufferLength,
                    BAYER_METHOD BayerMethod);

	/** Get image 12-bit as 8-bit
	 *  A 12-bit image will be converted into a 8-bit image. The caller needs to 
	 *  provide for a sufficient buffer for the 8-bit image.
	 */
  __usrdllexport__ SVGigE_RETURN
	Image_getImage12bitAs8bit(Image_handle hImage,
                            unsigned char *Buffer8bit,
                            int BufferLength);

	/** Get image 12-bit as 16-bit
	 *  A 12-bit image will be converted into a 16-bit image. The caller needs to 
	 *  provide for a sufficient buffer for the 16-bit image.
	 */
  __usrdllexport__ SVGigE_RETURN
	Image_getImage12bitAs16bit(Image_handle hImage,
                             unsigned char *Buffer16bit,
                             int BufferLength);

	/** Get image 16-bit as 8-bit
	 *  A 16-bit image will be converted into a 8-bit image. The caller needs to 
	 *  provide for a sufficient buffer for the 8-bit image.
	 */
  __usrdllexport__ SVGigE_RETURN
	Image_getImage16bitAs8bit(Image_handle hImage,
                            unsigned char *Buffer8bit,
                            int BufferLength);

  /** Get image size.
   *  The number of bytes in the image data field will be returned.
   *
   *  @param Image an image handle that was received from the callback function
   *  @return the number of bytes in the image data field
   */
  __usrdllexport__ int
  Image_getImageSize(Image_handle hImage);

  /** Get image identifier.
   *
   *  An image identifier will be returned as it was assigned by the camera.
   *  Usually the camera will assign an increasing sequence of integer numbers
   *  to subsequent images which will wrap at some point and jump back to 1.
   *  The 0 identifier will not be used as it is defined in the GigE Vision
   *  specification
   *
   *  @param Image an image handle that was received from the callback function
   *  @return an integer number that is unique inside a certain sequence of numbers
   */
  __usrdllexport__ int
  Image_getImageID(Image_handle hImage);

  /** Get image timestamp
   *  The timestamp that was assigned to an image by the camera on image
   *  acquisition will be returned
   *
   *  @param Image an image handle that was received from the callback function
   *  @return a timestamp as it was received from the camera in seconds elapsed after last camera reboot
   */
  __usrdllexport__ double
  Image_getTimestamp(Image_handle hImage);

  /** Get image size X
   *  The horizontal pixel number will be returned as received from the camera
   *
   *  @param Image an image handle that was received from the callback function
   *  @return the image's size X as received from the camera
   */
  __usrdllexport__ int
  Image_getSizeX(Image_handle hImage);

  /** Get image size Y
   *  The vertical pixel number will be returned as received from the camera
   *
   *  @param Image an image handle that was received from the callback function
   *  @return the image's size Y as received from the camera
   */
	__usrdllexport__ int
  Image_getSizeY(Image_handle hImage);

  /** Get image pitch
   *  The number of bytes in a row (pitch) will be returned as received from the camera
   *
   *  @param Image an image handle that was received from the callback function
   *  @return the image's pitch as received from the camera
   */
	__usrdllexport__ int
  Image_getPitch(Image_handle hImage);

  /** Get pixel type
   *  The pixel type as indicated by the camera will be returned
   *
   *  @param Image an image handle that was received from the callback function
   *  @return the pixel type as indicated by the camera
   */
  __usrdllexport__ GVSP_PIXEL_TYPE
  Image_getPixelType(Image_handle hImage);

  /** Get packet loss
   *  The number of lost packets will be returned
   *
   *  @param Image an image handle that was received from the callback function
   *  @return the pixel type as indicated by the camera
   */
	__usrdllexport__ int
  Image_getPacketCount(Image_handle hImage);

  /** Get packet resend
   *  The number of packets that have been resent will be reported
   *
   *  @param Image an image handle that was received from the callback function
   *  @return the pixel type as indicated by the camera
   */
	__usrdllexport__ int
  Image_getPacketResend(Image_handle hImage);

  /** Get signal type
   *  The signal type that is related to a callback will be returned
   *
   *  @param Image an image handle that was received from the callback function
   *  @return the signal type which informs why a callback was triggered
   */
	__usrdllexport__ SVGigE_SIGNAL_TYPE
  Image_getSignalType(Image_handle hImage);

//------------------------------------------------------------------------------
// Common SVGigE functions related to image transfer
//------------------------------------------------------------------------------

  /**
   *  Write image as a bitmap file to disk
   *  An image given by image data, geometry and type will be written to a
   *  specified location on disk.
   *
   *  @param Filename a path and filename for the bitmap file
   *  @param Data a pointer to image data
   *  @param SizeX the width of the image
   *  @param SizeY the height of the image
   *  @param PixelType either GVSP_PIX_MONO8 or GVSP_PIX_RGB24
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  SVGigE_writeImageToBitmapFile(char *Filename, unsigned char *Data, int SizeX, int SizeY, GVSP_PIXEL_TYPE PixelType);


//------------------------------------------------------------------------------
// Common SVGigE functions related to image conversion
//------------------------------------------------------------------------------

  /**
   *  Estimate white balance.
   *  Current image will be investigated for a suitable white balance setting
   *
   *  @see CameraContainer_getCamera()
   *  @param hCamera a camera handle received from CameraContainer_getCamera()
   *  @param Red new value for red color
   *  @param Green new value for green color
   *  @param Blue new value for blue color
   *  @return success or error code
   */
  __usrdllexport__ SVGigE_RETURN
  SVGigE_estimateWhiteBalance(unsigned char *BufferRGB,
                              int BufferLength,
                              float *Red,
                              float *Green ,
                              float *Blue);



//------------------------------------------------------------------------------
// Function return messages (errors)
//------------------------------------------------------------------------------

  /**
   *  Error get message.
   *  If the provided function return code respresents an error condition then
   *  a message will be mapped to the return code which will explain it.
   *
   *  @param ReturnCode a valid function return code
   *  @return a string which will explain the return code
   */
  __usrdllexport__ char *
  Error_getMessage(SVGigE_RETURN ReturnCode);



} // extern "C"

#ifdef __BORLANDC__
#pragma option pop
#endif

#endif


	/** 1394-Based Digital Camera Control Library
	 *  Bayer pattern decoding functions
	 *  Copyright (C) Damien Douxchamps <ddouxchamps@users.sf.net>
	 *
	 *  Written by Damien Douxchamps and Frederic Devernay
	 *
	 *  This library is free software; you can redistribute it and/or
	 *  modify it under the terms of the GNU Lesser General Public
	 *  License as published by the Free Software Foundation; either
	 *  version 2.1 of the License, or (at your option) any later version.
	 *
	 *  This library is distributed in the hope that it will be useful,
	 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
	 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	 *  Lesser General Public License for more details.
	 *
	 *  You should have received a copy of the GNU Lesser General Public
	 *  License along with this library; if not, write to the Free Software
	 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
	 */

#ifndef BAYERH
#define BAYERH

	enum {
	 DC1394_SUCCESS = 0,
	 DC1394_INVALID_BAYER_METHOD
	};

	enum {
	  DC1394_BYTE_ORDER_UYVY=0,
	  DC1394_BYTE_ORDER_YUYV
	};

	enum {
	  DC1394_BAYER_METHOD_NONE  = -1,
	  DC1394_BAYER_METHOD_NEAREST = 0,
	  DC1394_BAYER_METHOD_SIMPLE,
	  DC1394_BAYER_METHOD_BILINEAR,
	  DC1394_BAYER_METHOD_HQLINEAR,
	  DC1394_BAYER_METHOD_EDGESENSE,
	  DC1394_BAYER_METHOD_DOWNSAMPLE,
	};

	#define DC1394_BAYER_METHOD_MIN      DC1394_BAYER_METHOD_NEAREST
	#define DC1394_BAYER_METHOD_MAX      DC1394_BAYER_METHOD_EDGESENSE
	#define DC1394_BAYER_METHOD_NUM     (DC1394_BAYER_METHOD_MAX-DC1394_BAYER_METHOD_MIN+1)

	enum {
	  DC1394_COLOR_FILTER_RGGB = 512,
	  DC1394_COLOR_FILTER_GBRG,
	  DC1394_COLOR_FILTER_GRBG,
	  DC1394_COLOR_FILTER_BGGR
	};

	typedef unsigned short uint16_t;
	typedef unsigned int uint_t;

	int
	dc1394_bayer_decoding_8bit(const unsigned char *bayer, unsigned char *rgb, unsigned int sx, unsigned int sy, unsigned int tile, unsigned int method);

	int
	dc1394_bayer_decoding_16bit(const uint16_t *bayer, uint16_t *rgb, uint_t sx, uint_t sy, uint_t tile, uint_t bits, uint_t method);

#endif


