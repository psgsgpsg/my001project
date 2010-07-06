#ifndef _SAPACQDEVICE_H_
#define _SAPACQDEVICE_H_

//	SapAcqDevice.h : header file
//

#include "SapClassBasicDef.h"

class SAPCLASSBASIC_CLASS SapFeature;
class SAPCLASSBASIC_CLASS SapAcqDeviceCallbackInfo;

typedef void (*SapAcqDeviceCallback)(SapAcqDeviceCallbackInfo*);

//
// SapAcqDevice class declaration
//
class SAPCLASSBASIC_CLASS SapAcqDevice : public SapXferNode
{
public:
   // Enumerations
   enum UpdateFeatureMode
   {
      UpdateFeatureAuto   = CORACQDEVICE_VAL_UPDATE_FEATURE_MODE_AUTO,
      UpdateFeatureManual = CORACQDEVICE_VAL_UPDATE_FEATURE_MODE_MANUAL
   };

   // Various values
   enum MiscValues
   {
      MaxConfigNameSize = 64,
      MaxModeNameSize = 64,
   };
public:
	// Constructor/Destructor
	SapAcqDevice(SapLocation location = SapLocation::ServerSystem, BOOL readOnly = FALSE);
	SapAcqDevice(SapLocation location, const char *configFile);
	SapAcqDevice(const SapAcqDevice &acqDevice);
	virtual ~SapAcqDevice();

	SapAcqDevice &operator= (const SapAcqDevice &acqDevice);

	// Creation/destruction
	virtual BOOL Create();
	virtual BOOL Destroy();

   // General Parameters
	const char *GetConfigFile()              const { return m_ConfigFile; }
   BOOL GetReadOnly()                       const { return m_ReadOnly ; }
   UpdateFeatureMode GetUpdateFeatureMode() const { return m_UpdateFeatureMode; }
   const char *GetLabel()                   const { return m_Label; }
   const char *GetConfigName()              const { return m_ConfigName; }
   const char *GetModeName()                const { return m_ModeName; }

	BOOL SetConfigFile(const char *configFile);
   BOOL SetReadOnly(BOOL readOnly);
   BOOL SetUpdateFeatureMode(UpdateFeatureMode mode);
   BOOL SetConfigName(const char *configName);
   BOOL SetModeName(const char *modeName);

   // Feature Access Functions
   BOOL GetFeatureCount(int *featureCount);
   BOOL GetFeatureNameByIndex(int featureIndex, char *featureName, int featureNameSize);
   BOOL GetFeatureIndexByName(const char *featureName, int *featureIndex);
   BOOL IsFeatureAvailable(const char *featureName, BOOL *isAvailable);

   BOOL GetFeatureInfo(const char *featureName, SapFeature *feature);
   BOOL GetFeatureInfo(int featureIndex, SapFeature *feature);

   BOOL GetFeatureValue(const char *featureName, UINT32 *featureValue);
   BOOL GetFeatureValue(const char *featureName, UINT64 *featureValue);
   BOOL GetFeatureValue(const char *featureName, float *featureValue);
   BOOL GetFeatureValue(const char *featureName, double *featureValue);
   BOOL GetFeatureValue(const char *featureName, BOOL *featureValue);
   BOOL GetFeatureValue(const char *featureName, char *featureString, int featureStringSize);
   BOOL GetFeatureValue(const char *featureName, SapBuffer *featureBuffer);
   BOOL GetFeatureValue(const char *featureName, SapLut *featureLut);

   BOOL GetFeatureValue(int featureIndex, UINT32 *featureValue);
   BOOL GetFeatureValue(int featureIndex, UINT64 *featureValue);
   BOOL GetFeatureValue(int featureIndex, float *featureValue);
   BOOL GetFeatureValue(int featureIndex, double *featureValue);
   BOOL GetFeatureValue(int featureIndex, BOOL *featureValue);
   BOOL GetFeatureValue(int featureIndex, char *featureString, int featureStringSize);
   BOOL GetFeatureValue(int featureIndex, SapBuffer *featureBuffer);
   BOOL GetFeatureValue(int featureIndex, SapLut *featureLut);

   BOOL SetFeatureValue(const char *featureName, UINT32 featureValue);
   BOOL SetFeatureValue(const char *featureName, UINT64 featureValue);
   BOOL SetFeatureValue(const char *featureName, float featureValue);
   BOOL SetFeatureValue(const char *featureName, double featureValue);
   BOOL SetFeatureValue(const char *featureName, BOOL featureValue);
   BOOL SetFeatureValue(const char *featureName, const char *featureString);
   BOOL SetFeatureValue(const char *featureName, SapBuffer *featureBuffer);
   BOOL SetFeatureValue(const char *featureName, SapLut *featureLut);

   BOOL SetFeatureValue(int featureIndex, UINT32 featureValue);
   BOOL SetFeatureValue(int featureIndex, UINT64 featureValue);
   BOOL SetFeatureValue(int featureIndex, float featureValue);
   BOOL SetFeatureValue(int featureIndex, double featureValue);
   BOOL SetFeatureValue(int featureIndex, BOOL featureValue);
   BOOL SetFeatureValue(int featureIndex, const char *featureString);
   BOOL SetFeatureValue(int featureIndex, SapBuffer *featureBuffer);
   BOOL SetFeatureValue(int featureIndex, SapLut *featureLut);

   BOOL UpdateFeaturesFromDevice();
   BOOL UpdateFeaturesToDevice();

   BOOL LoadFeatures(const char *configFile);
   BOOL SaveFeatures(const char *configFile);

   // Event Management Functions
   BOOL GetEventCount(int *eventCount);
   BOOL GetEventNameByIndex(int eventIndex, char *eventName, int eventNameSize);
   BOOL GetEventIndexByName(const char *eventName, int *eventIndex);
   BOOL IsEventAvailable(const char *eventName, BOOL *isAvailable);

   BOOL RegisterCallback(const char *eventName, SapAcqDeviceCallback callback, void *context);
   BOOL RegisterCallback(int eventIndex, SapAcqDeviceCallback callback, void *context);
   BOOL UnregisterCallback(const char *eventName);
   BOOL UnregisterCallback(int eventIndex);
   BOOL IsCallbackRegistered(const char *eventName, BOOL *isRegistered);
   BOOL IsCallbackRegistered(int eventIndex, BOOL *isRegistered);

   // Command Management Functions
   BOOL GetCommandCount(int *commandCount);
   BOOL GetCommandNameByIndex(int commandIndex, char *commandName, int commandNameSize);
   BOOL GetCommandIndexByName(const char *commandName, int *commandIndex);
   BOOL IsCommandAvailable(const char *commandName, BOOL *isAvailable);
   BOOL SendCommand(const char *commandName, void *inData, int inDataSize, void *outData, int outDataSize);
   BOOL SendCommand(int commandIndex, void *inData, int inDataSize, void *outData, int outDataSize);

protected:
   // Utility methods
   void Construct(const char *configFile, BOOL readOnly, UpdateFeatureMode featureMode);
   BOOL SetLabel(BOOL refreshResourceName);
   BOOL SetXferParams();
   BOOL GetFeatureValue(const char *featureName, void *featureValue, int featureValueSize);
   BOOL GetFeatureValue(int featureIndex, void *featureValue, int featureValueSize);
   BOOL SetFeatureValue(const char *featureName, const void *featureValue, int featureValueSize);
   BOOL SetFeatureValue(int featureIndex, const void *featureValue, int featureValueSize);
   BOOL IsTransferFeature(const char *featureName);
   BOOL IsTransferFeature(int featureIndex);
   static SAPSTATUS CCONV OnCallback(void *context, COREVENTINFO eventInfo);

protected:
	// Parameters
   char m_ConfigFile[MAX_PATH];           // Configuration (.ccf) file name
   BOOL m_ReadOnly;                       // Read-only usage flag (only feature retrieval allowed)
   UpdateFeatureMode m_UpdateFeatureMode; // Auto vs. manual update mode when writing feature values
   char m_Label[MaxLabelSize];            // Name of the acquisition resource
   char m_ConfigName[MaxConfigNameSize];  // Configuration name saved to CCF file (as 'Vic Name')
   char m_ModeName[MaxModeNameSize];      // Mode name saved to CCF file (as 'Camera Name')

   // Feature management
   int m_FeatureCount;                    // Number of available features
   static char *m_ValueChangeEventName;   // Name of event for feature value change
   int m_ValueChangeEventIndex;           // Index of event for feature value change

   // Event management
   int m_EventCount;                   // Number of available events
   SapAcqDeviceCallback *m_Callback;   // Application callback functions
   void **m_Context;                   // Application context for each callback function

   // Command management
   int m_CommandCount;                 // Number of available commands

   // Feature names needed to set transfer parameters, and their indexes
   static char *m_WidthFeatureName;
   static char *m_HeightFeatureName;
   static char *m_FormatFeatureName;
   static char *m_BitsPerPixelFeatureName;

   int m_WidthFeatureIndex;
   int m_HeightFeatureIndex;
   int m_FormatFeatureIndex;
   int m_BitsPerPixelFeatureIndex;

   // Feature corresponding to user name of acquisition device, and its index
   static char *m_UserNameFeature;
   int m_UserNameIndex;
}; 


//
// SapAcqDeviceCallbackInfo class declaration
//
class SAPCLASSBASIC_CLASS SapAcqDeviceCallbackInfo : public SapManager
{
public:
   // Attributes
   SapAcqDevice *GetAcqDevice() const { return m_AcqDevice; }
   void *GetContext()           const { return m_Context; }
   COREVENTINFO GetEventInfo()  const { return m_EventInfo; }  // Direct Access (internal)

   // Common Parameters
   BOOL GetEventCount(int *eventCount);
   BOOL GetEventIndex(int *eventIndex);
   BOOL GetHostTimeStamp(UINT64 *hostTimeStamp);
   BOOL GetAuxiliaryTimeStamp(UINT64 *auxTimeStamp);

   // Custom Parameters
   BOOL GetCustomData(void **customData);
   BOOL GetCustomSize(int *customSize);

   // Unnamed Parameters
   BOOL GetGenericParam0(int *paramValue);
   BOOL GetGenericParam1(int *paramValue);
   BOOL GetGenericParam2(int *paramValue);
   BOOL GetGenericParam3(int *paramValue);

   // Event-specific Parameters
   BOOL GetFeatureIndex(int *featureIndex);  // Used by "FeatureChanged" event

private:
   SapAcqDeviceCallbackInfo(SapAcqDevice *acqDevice, void *context, COREVENTINFO eventInfo);
	~SapAcqDeviceCallbackInfo();

private:
   SapAcqDevice* m_AcqDevice;
	void *m_Context;
   COREVENTINFO m_EventInfo;

   // Classes which can directly access private data members
   friend class SapAcqDevice;
};

#endif	// _SAPACQDEVICE_H_
