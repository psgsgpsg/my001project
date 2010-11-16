#ifndef _SAPFEATURE_H_
#define _SAPFEATURE_H_

//	SapFeature.h : header file
//

#include "SapClassBasicDef.h"

//
// SapFeature class declaration
//
class SAPCLASSBASIC_CLASS SapFeature : public SapManager
{
public:
   // Enumerations
   enum Type   // Data Types
   {
      TypeUndefined = CORFEATURE_VAL_TYPE_UNDEFINED,
      TypeInt32     = CORFEATURE_VAL_TYPE_INT32,
      TypeInt64     = CORFEATURE_VAL_TYPE_INT64,
      TypeFloat     = CORFEATURE_VAL_TYPE_FLOAT,
      TypeDouble    = CORFEATURE_VAL_TYPE_DOUBLE,
      TypeBool      = CORFEATURE_VAL_TYPE_BOOL,
      TypeEnum      = CORFEATURE_VAL_TYPE_ENUM,
      TypeString    = CORFEATURE_VAL_TYPE_STRING,
      TypeBuffer    = CORFEATURE_VAL_TYPE_BUFFER,
      TypeLut       = CORFEATURE_VAL_TYPE_LUT
   };

   enum AccessMode   // Access Modes
   {
      AccessUndefined = CORFEATURE_VAL_ACCESS_MODE_UNDEFINED,
      AccessRW        = CORFEATURE_VAL_ACCESS_MODE_RW,      // Read and Write
      AccessRO        = CORFEATURE_VAL_ACCESS_MODE_RO,      // Read Only
      AccessWO        = CORFEATURE_VAL_ACCESS_MODE_WO,      // Write Only
      AccessNP        = CORFEATURE_VAL_ACCESS_MODE_NP,      // Not present
      AccessNE        = CORFEATURE_VAL_ACCESS_MODE_NE       // Not enabled
   };

   enum Representation  // Data Representations (for float and integer)
   {
      RepresentationUndefined   = CORFEATURE_VAL_REPRESENTATION_UNDEFINED,
      RepresentationLinear      = CORFEATURE_VAL_REPRESENTATION_LINEAR,
      RepresentationLogarithmic = CORFEATURE_VAL_REPRESENTATION_LOGARITHMIC,
      RepresentationBoolean     = CORFEATURE_VAL_REPRESENTATION_BOOLEAN
   };

   enum Sign   // Data Sign
   {
      SignUndefined = CORFEATURE_VAL_SIGN_UNDEFINED,
      Signed        = CORFEATURE_VAL_SIGN_SIGNED,
      Unsigned      = CORFEATURE_VAL_SIGN_UNSIGNED
   };

   enum WriteMode    // Write mode when access mode is read-write or write only
   {
      WriteUndefined    = CORFEATURE_VAL_WRITE_MODE_UNDEFINED,       // Undefined
      WriteAlways       = CORFEATURE_VAL_WRITE_MODE_ALWAYS,          // Always writable
      WriteNotAcquiring = CORFEATURE_VAL_WRITE_MODE_NOT_ACQUIRING,   // Writable when not acquiring
      WriteNotConnected = CORFEATURE_VAL_WRITE_MODE_NOT_CONNECTED    // Writable when not connected
   };

   enum Visibility   // Visibility Types
   {
      VisibilityUndefined = CORFEATURE_VAL_VISIBILITY_UNDEFINED,  // Undefined
      VisibilityBeginner  = CORFEATURE_VAL_VISIBILITY_BEGINNER,	// Always visible
      VisibilityExpert    = CORFEATURE_VAL_VISIBILITY_EXPERT,     // Visible for experts or Gurus
      VisibilityGuru      = CORFEATURE_VAL_VISIBILITY_GURU,       // Visible for Gurus
      VisibilityInvisible = CORFEATURE_VAL_VISIBILITY_INVISIBLE   // Not Visible
   };

public:
	// Constructor/Destructor
	SapFeature(SapLocation location = SapLocation::ServerSystem);
	SapFeature(const SapFeature &feature);
	virtual ~SapFeature();

	SapFeature &operator= (const SapFeature &feature);

	// Creation/destruction
	virtual BOOL Create();
	virtual BOOL Destroy();

   // Attributes
   SapLocation GetLocation() const { return m_Location; }
   CORHANDLE GetHandle()	  const { return m_Handle; }

	BOOL SetLocation(SapLocation location);

   // General Parameters
   BOOL GetName(char *name, int nameSize);
   BOOL GetType(Type *type);
   BOOL IsStandard(BOOL *isStandard);
   BOOL GetAccessMode(AccessMode *accessMode);
   BOOL GetPollingTime(int *pollingTime);
   BOOL GetToolTip(char *tooltip, int tooltipSize);
   BOOL GetDisplayName(char *displayName, int displayNameSize);
   BOOL GetRepresentation(Representation *representation);
   BOOL GetSign(Sign *sign);
   BOOL GetSiUnit(char *unit, int unitSize);
   BOOL GetCategory(char *category, int categorySize);
   BOOL GetWriteMode(WriteMode *writeMode);
   BOOL IsSavedToConfigFile(BOOL *savedToConfigFile);
   BOOL GetSiToNativeExp10(int *exponent);
   BOOL GetVisibility(Visibility *visibility);

   BOOL SetSavedToConfigFile(BOOL savedToConfigFile);

   // Integer/float-specific Parameters
   BOOL GetMin(UINT32 *minValue);
   BOOL GetMin(UINT64 *minValue);
   BOOL GetMin(float *minValue);
   BOOL GetMin(double *minValue);

   BOOL GetMax(UINT32 *maxValue);
   BOOL GetMax(UINT64 *maxValue);
   BOOL GetMax(float *maxValue);
   BOOL GetMax(double *maxValue);

   BOOL GetInc(UINT32 *incValue);
   BOOL GetInc(UINT64 *incValue);
   BOOL GetInc(float *incValue);
   BOOL GetInc(double *incValue);

   // Enumeration-specific Parameters
   BOOL GetEnumCount(int *enumCount);
   BOOL GetEnumString(int enumIndex, char *enumString, int enumStringSize);
   BOOL GetEnumValue(int enumIndex, int *enumValue);
   BOOL IsEnumEnabled(int enumIndex, BOOL *enabled);
   BOOL GetEnumStringFromValue(int enumValue, char *enumString, int enumStringSize);
   BOOL GetEnumValueFromString(const char *enumString, int *enumValue);

protected:
   // Utility methods
   void Construct(SapLocation location, CORFEATURE handle, BOOL isValid);
   BOOL GetMin(void *minValue, int minValueSize);
   BOOL GetMax(void *maxValue, int maxValueSize);
   BOOL GetInc(void *incValue, int incValueSize);

   void SetValid(BOOL isValid = TRUE) { m_IsValid = isValid; }

protected:
   SapLocation m_Location;
   CORFEATURE m_Handle;

   // TRUE after a call to SapAcqDevice::GetFeatureInfo, means that the feature has meaningful content
   BOOL m_IsValid;

   // Classes which can directly access protected data members
   friend class SapAcqDevice;
}; 

#endif	// _SAPFEATURE_H_
