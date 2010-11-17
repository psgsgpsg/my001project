#ifndef _SAPFLATFIELD_H_
#define _SAPFLATFIELD_H_

// SapFlatField.h : header file
//

#include "SapClassBasicDef.h"

// Default ratios
const float SapDefFlatFieldPixelRatio				=99.00f;    // 99 percent of all pixels
const float SapDefFlatFieldDefectRatio				= 1.00f;    // Up to 1 percent of defective pixels
const float SapDefFlatFieldAvgFactorBlack       = 0.25f;		// 0.25 * maximum pixel value
const float SapDefFlatFieldAvgFactorWhite       = 0.80f;		// 0.80 * maximum pixel value
const float SapDefFlatFieldDeviationMax         = 0.25f;		// 0.25 * maximum pixel value

// Forward declarations
class SAPCLASSBASIC_CLASS SapFlatFieldInfo;
class SAPCLASSBASIC_CLASS SapFlatFieldStats;
class SAPCLASSBASIC_CLASS SapFlatFieldDefects;

//
// SapFlatField class declaration
//
class SAPCLASSBASIC_CLASS SapFlatField : public SapManager
{
public:
   // Various constants
   enum MiscValues
   {
      DefNumLinesAverage = 128,

		DefGainMin    = 0,
		DefGainMax    = 255,
      DefGainDivisor= 128,

		DefOffsetMin= 0,
		DefOffsetMax= 255,

      DefDeviationFactorMax = 4,				

      InvalidFlatFieldNumber= -1
   };

	enum CorrectionType
	{
		CorrectionTypeField,
		CorrectionTypeLine,
		CorrectionTypeInvalid= -1
	};

public:
	// Constructor/Destructor
   SapFlatField(SapAcquisition *pAcq, int numFlatField = 1);
   SapFlatField(SapBuffer *pBuffer, int numFlatField = 1);
   SapFlatField(const SapFlatField &flatField);
   virtual ~SapFlatField();

	SapFlatField &operator= (const SapFlatField &flatField);

	// Creation/destruction
   virtual BOOL Create();
   virtual BOOL Destroy();

public:
	// Access to implementation
   SapAcquisition *GetAcquisition() const { return m_pAcq; }
   SapBuffer *GetBuffer()           const { return m_pBuffer; }
   SapBuffer *GetBufferOffset()     const { return m_pBufferOffset; }
   SapBuffer *GetBufferGain()       const { return m_pBufferGain; }

   BOOL IsEnabled()          const { return m_IsEnabled; }
	BOOL IsPixelReplacement() const { return m_bPixelReplacement;}
   BOOL IsSoftware()         const { return !m_IsHardware || !m_IsHardwareEnabled; }

   CorrectionType GetCorrectionType()   const { return m_CorrectionType; }
	int GetDeviationMaxBlack()           const { return m_DeviationMaxBlack;}
	int GetDeviationMaxWhite()           const { return m_DeviationMaxWhite;}
   int GetGainDivisor()                 const { return m_GainDivisor; }
   int GetNumLinesAverage()             const { return m_NumLinesAverage; }
	int GetVerticalOffset()              const { return m_VerticalOffset; }
	SapAcquisition::VideoType GetVideoType()	const { return m_VideoType; }
   int GetNumFlatField()                const { return m_NumFlatField; }
   int GetIndex()                       const { return m_Index; }

	virtual BOOL SetAcquisition(SapAcquisition *pAcq);
   virtual BOOL SetBuffer(SapBuffer *pBuffer);
   virtual BOOL SetCorrectionType( CorrectionType correctionType);
	virtual BOOL SetDeviationMaxBlack( int deviationMax);
	virtual BOOL SetDeviationMaxWhite( int deviationMax);
   virtual BOOL SetGainDivisor(int gainDivisor);
   virtual BOOL SetGainMinMax(int gainMin, int gainMax);
   virtual BOOL SetOffsetMinMax(int offsetMin, int offsetMax);
   virtual BOOL SetNumLinesAverage(int numLinesAverage);
	virtual BOOL SetVerticalOffset( int verticalOffset);
	virtual BOOL SetVideoType( SapAcquisition::VideoType videoType);
   virtual BOOL SetNumFlatField( int numFlatField);

   virtual BOOL SetIndex( int index);
   virtual void Next() { m_Index++; if (m_Index == m_NumFlatField) m_Index = 0; }

   // Load/save gain and offset buffers from/to disk files
	virtual BOOL Load( const char *fileName);
	virtual BOOL Save( const char *fileName);

   // Load gain and offset buffers from buffer objects
	virtual BOOL Load( SapBuffer* pBufferGain, SapBuffer* pBufferOffset);

   // Clear/compute values for gain and offset buffers
   virtual BOOL Clear();
   virtual BOOL ComputeOffset(SapBuffer *pBuffer);
   virtual BOOL ComputeGain(SapBuffer *pBuffer, SapFlatFieldDefects *pDefects, BOOL bUseImageMaxValue = TRUE);

   // Execution control
	virtual BOOL EnablePixelReplacement( BOOL enable= TRUE);
   virtual BOOL Enable(BOOL enable = TRUE, BOOL useHardware= TRUE);
   virtual BOOL Execute(SapBuffer *pBuffer);
	virtual BOOL Execute(SapBuffer *pBuffer, int bufIndex);

   // Buffer statictics
   virtual BOOL GetAverage(SapBuffer *pBuffer, SapFlatFieldStats *pStats);
   virtual BOOL GetStats(SapBuffer *pBuffer, SapFlatFieldStats *pStats);
   
   // Obsolete methods
   BOOL IsHardware() const { return m_IsHardware; }
   virtual BOOL Load( const char *gainFile, const char *offsetFile);
   virtual BOOL Save( const char *gainFile, const char *offsetFile);
   virtual BOOL ComputeGain(SapBuffer *pBuffer, int deviationMax, SapFlatFieldDefects *pDefects, BOOL bUseImageMaxValue = TRUE);
   virtual BOOL ComputeGain(SapBuffer *pBuffer, int blackDeviationMax, int whiteDeviationMax, SapFlatFieldDefects *pDefects, BOOL bUseImageMaxValue = TRUE);
   virtual BOOL GetAverage(SapBuffer *pBuffer, int *pAverage, int *pStdDeviation = NULL, int iComponent= 0);

protected:
	// Utility methods
	virtual BOOL CreateRemaining();
   virtual void Construct(SapAcquisition *pAcq, SapBuffer *pBuffer, int numFlatField);

   virtual BOOL IsFileCompatible(const char *filename, SapBuffer *pBuffer) const;
	virtual BOOL IsBufferCompatible( SapBuffer *pBufferSrc, SapBuffer *pBufferDst) const;
	virtual BOOL IsFormatCompatible(SapFormat format) const;
	virtual BOOL GetPixelDepth( int* pixelDepth) const;

   virtual BOOL SetAcqFlatField(SapBuffer *pBufferGain, SapBuffer *pBufferOffset);

   virtual BOOL FFCAverageLines(void *pBuffer, int verticalOffset, int width, int height, SapFormat format, void *pBufferAverage);
   virtual BOOL FFCComputeOffset(void *pBuffer, int width, int height, SapFormat format, void *pBufferOffset);
   virtual BOOL FFCExecute(void *pBuffer, int width, int height, SapFormat format, void *pBufferOffset, void *pBufferGain, int isLineScan);

   virtual BOOL FFCComputeGain(void *pBuffer, int width, int height, SapFormat format, void *pBufferOffset, float pixelRatio, int offsetDeviationMax, int whiteDeviationMax, void *pBufferGain, SapFlatFieldDefects *pDefects, int iLineOffset= 0, int iLineStep= 1, int iColumnOffset= 0, int iColumnStep= 1, int bitDepth = -1);
   virtual BOOL FFCGetAverage(void *pBuffer, int width, int height, SapFormat format, int *pAverage, int *pStdDeviation, int iLineOffset= 0, int iLineStep= 1, int iColumnOffset= 0, int iColumnStep= 1);
   virtual BOOL FFCGetStats(void *pBuffer, int width, int height, SapFormat format, void *pBufferOffset, float pixelRatio, SapFlatFieldStats *pStats, int iLineOffset= 0, int iLineStep= 1, int iColumnOffset= 0, int iColumnStep= 1, int iComponent= 0);
	virtual BOOL FFCGetHistogram(void *pBuffer, int width, int height, SapFormat format, void* pBufferOffset, int **ppHistogram, int *pNumBins, int iLineOffset= 0, int iLineStep= 1, int iColumnOffset= 0, int iColumnStep= 1);

	virtual BOOL FFCHistPeaks( int* pHistogram, int numBins, int vicinity, SapDataPoint** ppPeaks, int *pnPeaks);

protected:
   SapAcquisition *m_pAcq;
   SapBuffer      *m_pBuffer;
   SapBuffer      *m_pBufferOffset;
   SapBuffer      *m_pBufferGain;

   BOOL m_IsEnabled;
   BOOL m_FlatFieldEnable;       // Current value for 'enable' parameter
   BOOL m_IsHardware;
	BOOL m_IsHardwareEnabled;
   int  *m_FlatFieldNumbers;     // Flat-field numbers returned from acquisition device
   int  m_GainDivisor;
	int  m_GainMin;
	int  m_GainMax;
	int  m_OffsetMin;
	int  m_OffsetMax;
   int  m_NumLinesAverage;
	int  m_VerticalOffset;
	int  m_DeviationMaxBlack;
	int  m_DeviationMaxWhite;
	BOOL m_bPixelReplacement;
   int  m_NumFlatField;
   int  m_Index;

	SapAcquisition::VideoType m_VideoType;
   CorrectionType m_CorrectionType;
   SapXferParams m_XferParams;
};

//
// SapFlatFieldDefects class declaration
//
class SAPCLASSBASIC_CLASS SapFlatFieldDefects
{
public:
   SapFlatFieldDefects()
   {
      m_NumDefects = 0;
      m_NumClusters = 0;
      m_DefectRatio = 0.0f;
   }
   ~SapFlatFieldDefects() {}

   int GetNumDefects()    const { return m_NumDefects; }
   int GetNumClusters()   const { return m_NumClusters; }
   float GetDefectRatio() const { return m_DefectRatio; }

protected:
   int   m_NumDefects;
   int   m_NumClusters;
   float m_DefectRatio;

   // Classes which can directly access protected methods and members
   friend class SapFlatField;
};

//
// SapFlatFieldStats class declaration
//
class SAPCLASSBASIC_CLASS SapFlatFieldStats
{
public:
   enum MiscValues
   {
		MaxComponent= 4
   };

	SapFlatFieldStats()
   {
		m_NumComponents= 1;

      memset( m_Average, 0, sizeof( m_Average));
      memset( m_StdDeviation, 0, sizeof( m_StdDeviation));
      memset( m_PeakPosition, 0, sizeof( m_PeakPosition));
      memset( m_Low, 0, sizeof( m_Low));
      memset( m_High, 0, sizeof( m_High));
      memset( m_NumPixels, 0, sizeof( m_NumPixels));
		memset( m_PixelRatio, 0, sizeof( m_PixelRatio));
   }
   ~SapFlatFieldStats() {}

	int GetAverage( int iComponent= 0)			const { return m_Average[ iComponent]; }
	int GetStdDeviation( int iComponent= 0)	const { return m_StdDeviation[ iComponent]; }
	int GetPeakPosition( int iComponent= 0)	const { return m_PeakPosition[ iComponent]; }
	int GetLow( int iComponent= 0)				const { return m_Low[ iComponent]; }
	int GetHigh( int iComponent= 0)				const { return m_High[ iComponent]; }
	int GetNumPixels( int iComponent= 0)		const { return m_NumPixels[ iComponent]; }
	float GetPixelRatio( int iComponent= 0)	const { return m_PixelRatio[ iComponent]; }
	int GetNumComponents()							const { return m_NumComponents;}

protected:
	int m_NumComponents;
	int m_Average[ MaxComponent];
	int m_StdDeviation[ MaxComponent];
	int m_PeakPosition[ MaxComponent];
	int m_Low[ MaxComponent];
	int m_High[ MaxComponent];
	int m_NumPixels[ MaxComponent];
	float m_PixelRatio[ MaxComponent];

	// Classes which can directly access protected methods and members
	friend class SapFlatField;
};

#endif
