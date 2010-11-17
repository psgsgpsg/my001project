//
// Coreco API - Acquisition device control functions
//

#ifndef _CAPIACQDEVICE_H_
#define _CAPIACQDEVICE_H_

#include "capidef.h"
#include "capieventinfo.h"


/****************************************************************************
*  Acquisition Device Module Parameters
****************************************************************************/

#define CORACQDEVICE_PRM_FIRST         0
#define CORACQDEVICE_PRM( n, size)     ((((n) + CORACQDEVICE_PRM_FIRST) << 16) | (size))

#define CORACQDEVICE_PRM_LABEL					   CORACQDEVICE_PRM( 0, 128)
#define CORACQDEVICE_PRM_UPDATE_FEATURE_MODE    CORACQDEVICE_PRM( 1, 4)
#define CORACQDEVICE_PRM_CONFIG_NAME            CORACQDEVICE_PRM( 2, 64)
#define CORACQDEVICE_PRM_MODE_NAME              CORACQDEVICE_PRM( 3, 64)


/****************************************************************************
*  Acquisition Device Module Values
****************************************************************************/

// Current feature update mode, default is auto
#define CORACQDEVICE_VAL_UPDATE_FEATURE_MODE_AUTO     0
#define CORACQDEVICE_VAL_UPDATE_FEATURE_MODE_MANUAL   1


/****************************************************************************
*	Application programming interface
****************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif

// General Functions
CORSTATUS CORAPIFUNC CorAcqDeviceResetModule(CORSERVER server);
CORSTATUS CORAPIFUNC CorAcqDeviceGetCount(CORSERVER server, PUINT32 count);
CORSTATUS CORAPIFUNC CorAcqDeviceGetHandle(CORSERVER server, UINT32 index, PCORACQDEVICE acqDevice);
CORSTATUS CORAPIFUNC CorAcqDeviceGetHandleReadOnly(CORSERVER server, UINT32 index, PCORACQDEVICE acqDevice);
CORSTATUS CORAPIFUNC CorAcqDeviceRelease(CORACQDEVICE acqDevice);
CORSTATUS CORAPIFUNC CorAcqDeviceReset(CORACQDEVICE acqDevice);

// Module capability and parameter access functions
CORSTATUS CORAPIFUNC CorAcqDeviceGetCap(CORACQDEVICE acqDevice, UINT32 cap, void *value);
CORSTATUS CORAPIFUNC CorAcqDeviceGetPrm(CORACQDEVICE acqDevice, UINT32 prm, void *value);
CORSTATUS CORAPIFUNC CorAcqDeviceSetPrm(CORACQDEVICE acqDevice, UINT32 prm, UINT32 value);
CORSTATUS CORAPIFUNC CorAcqDeviceSetPrmEx(CORACQDEVICE acqDevice, UINT32 prm, const void *value);

// Feature Access Functions
CORSTATUS CORAPIFUNC CorAcqDeviceGetFeatureCount(CORACQDEVICE acqDevice, PUINT32 count);
CORSTATUS CORAPIFUNC CorAcqDeviceGetFeatureNameByIndex(CORACQDEVICE acqDevice, UINT32 index, PSTR name, UINT32 nameSize);
CORSTATUS CORAPIFUNC CorAcqDeviceGetFeatureIndexByName(CORACQDEVICE acqDevice, PCSTR name, PUINT32 index);
CORSTATUS CORAPIFUNC CorAcqDeviceIsFeatureAvailable(CORACQDEVICE acqDevice, PCSTR name, PUINT32 isAvailable);

CORSTATUS CORAPIFUNC CorAcqDeviceGetFeatureInfoByName(CORACQDEVICE acqDevice, PCSTR name, CORFEATURE feature);
CORSTATUS CORAPIFUNC CorAcqDeviceGetFeatureInfoByIndex(CORACQDEVICE acqDevice, UINT32 index, CORFEATURE feature);

CORSTATUS CORAPIFUNC CorAcqDeviceGetFeatureValueByName(CORACQDEVICE acqDevice, PCSTR name, void *value, UINT32 valueSize);
CORSTATUS CORAPIFUNC CorAcqDeviceGetFeatureValueByIndex(CORACQDEVICE acqDevice, UINT32 index, void *value, UINT32 valueSize);
CORSTATUS CORAPIFUNC CorAcqDeviceGetFeatureDataByName(CORACQDEVICE acqDevice, PCSTR name, CORHANDLE dataDest);
CORSTATUS CORAPIFUNC CorAcqDeviceGetFeatureDataByIndex(CORACQDEVICE acqDevice, UINT32 index, CORHANDLE dataDest);

CORSTATUS CORAPIFUNC CorAcqDeviceSetFeatureValueByName(CORACQDEVICE acqDevice, PCSTR name, const void *value, UINT32 valueSize);
CORSTATUS CORAPIFUNC CorAcqDeviceSetFeatureValueByIndex(CORACQDEVICE acqDevice, UINT32 index, const void *value, UINT32 valueSize);
CORSTATUS CORAPIFUNC CorAcqDeviceSetFeatureDataByName(CORACQDEVICE acqDevice, PCSTR name, CORHANDLE dataSrc);
CORSTATUS CORAPIFUNC CorAcqDeviceSetFeatureDataByIndex(CORACQDEVICE acqDevice, UINT32 index, CORHANDLE dataSrc);

CORSTATUS CORAPIFUNC CorAcqDeviceUpdateFeaturesToDevice(CORACQDEVICE acqDevice);
CORSTATUS CORAPIFUNC CorAcqDeviceUpdateFeaturesFromDevice(CORACQDEVICE acqDevice);

CORSTATUS CORAPIFUNC CorAcqDeviceLoadFeatures(CORACQDEVICE acqDevice, PCSTR fileName);
CORSTATUS CORAPIFUNC CorAcqDeviceSaveFeatures(CORACQDEVICE acqDevice, PCSTR fileName);

// Event Management Functions
CORSTATUS CORAPIFUNC CorAcqDeviceGetEventCount(CORACQDEVICE acqDevice, PUINT32 count);
CORSTATUS CORAPIFUNC CorAcqDeviceGetEventNameByIndex(CORACQDEVICE acqDevice, UINT32 index, PSTR name, UINT32 nameSize);
CORSTATUS CORAPIFUNC CorAcqDeviceGetEventIndexByName(CORACQDEVICE acqDevice, PCSTR name, PUINT32 index);
CORSTATUS CORAPIFUNC CorAcqDeviceIsEventAvailable(CORACQDEVICE acqDevice, PCSTR name, PUINT32 isAvailable);

CORSTATUS CORAPIFUNC CorAcqDeviceRegisterCallbackByName(CORACQDEVICE acqDevice, PCSTR name, PCOREVENTINFOCALLBACK callback, void *context);
CORSTATUS CORAPIFUNC CorAcqDeviceRegisterCallbackByIndex(CORACQDEVICE acqDevice, UINT32 index, PCOREVENTINFOCALLBACK callback, void *context);
CORSTATUS CORAPIFUNC CorAcqDeviceUnregisterCallbackByName(CORACQDEVICE acqDevice, PCSTR name);
CORSTATUS CORAPIFUNC CorAcqDeviceUnregisterCallbackByIndex(CORACQDEVICE acqDevice, UINT32 index);
CORSTATUS CORAPIFUNC CorAcqDeviceIsCallbackRegisteredByName(CORACQDEVICE acqDevice, PCSTR name, PUINT32 isRegistered);
CORSTATUS CORAPIFUNC CorAcqDeviceIsCallbackRegisteredByIndex(CORACQDEVICE acqDevice, UINT32 index, PUINT32 isRegistered);

// Command Management Functions
CORSTATUS CORAPIFUNC CorAcqDeviceGetCommandCount(CORACQDEVICE acqDevice, PUINT32 count);
CORSTATUS CORAPIFUNC CorAcqDeviceGetCommandNameByIndex(CORACQDEVICE acqDevice, UINT32 index, PSTR name, UINT32 nameSize);
CORSTATUS CORAPIFUNC CorAcqDeviceGetCommandIndexByName(CORACQDEVICE acqDevice, PCSTR name, PUINT32 index);
CORSTATUS CORAPIFUNC CorAcqDeviceIsCommandAvailable(CORACQDEVICE acqDevice, PCSTR name, PUINT32 isAvailable);

CORSTATUS CORAPIFUNC CorAcqDeviceSendCommandByName(CORACQDEVICE acqDevice, PCSTR name, void *inData, UINT32 inDataSize, void *outData, UINT32 outDataSize);
CORSTATUS CORAPIFUNC CorAcqDeviceSendCommandByIndex(CORACQDEVICE acqDevice, UINT32 index, void *inData, UINT32 inDataSize, void *outData, UINT32 outDataSize);

#ifdef __cplusplus
}
#endif

#endif   // _CAPIACQDEVICE_H_
