//#include "./csi/csitypes.h"
#include "math.h"
#include "iCVCImg.h"

#define IMPORT(t) extern "C" t __stdcall

#ifndef CVCEDGE_INCLUDE
  #define CVCEDGE_INCLUDE
  
  typedef BYTE*   NORMPROJECTION;
  typedef DWORD*  PROJECTION;
  typedef void*   PROJECTIONEX;
  typedef void*   EDGERESULTS;

  typedef struct tagEdgeResult
  {
    double x; 
    double y; 
    double Quality;
  } TEdgeResult;
  typedef TEdgeResult *PEdgeResult;

  typedef struct tagLongProjection
  {
    long x;
    long y;
    long Value;
  } TLongProjection;
  typedef TLongProjection *pLongProjection;
  
  typedef struct tagDoubleProjection
  {
    double x;
    double y;
    double Value;
  } TDoubleProjection;
  typedef TDoubleProjection *pDoubleProjection;

  typedef enum 
  {
    POLARITY_DONT_CARE = 0,
    POLARITY_POSITIVE  = 1,
    POLARITY_NEGATIVE  = 2
  } TEdgePolarity;
#endif // CVCEDGE_INCLUDE


IMPORT(BOOL) TFindFirstEdge    (IMG Image, long Index, 
                               long Density, TArea Area, 
                               double Threshold, 
                               BOOL PositiveEdge, 
                               TEdgeResult& Result);
IMPORT(BOOL) TFindEdgePair     (IMG Image, long Index, 
                               long Density, TArea Area,
                               double Threshold1, BOOL PositiveEdge1, TEdgeResult& Result1, 
                               double Threshold2, BOOL PositiveEdge2, TEdgeResult& Result2);
IMPORT(BOOL) TFindAllEdges     (IMG Image, long Index, 
                               long Density, TArea Area, 
                               double Threshold, 
                               BOOL PositiveEdge, 
                               long lMaxEdges,
                               PEdgeResult Result,
                               long& lEdgeCount);

IMPORT(BOOL) TSFindFirstEdge   (IMG Image, long Index, 
                               long Density, TArea Area, 
                               double Threshold, 
                               BOOL PositiveEdge, 
                               TEdgeResult& Result);
IMPORT(BOOL) TSFindEdgePair    (IMG Image, long Index, 
                               long Density, TArea Area,
                               double Threshold1, BOOL PositiveEdge1, TEdgeResult& Result1, 
                               double Threshold2, BOOL PositiveEdge2, TEdgeResult& Result2);
IMPORT(BOOL) TSFindAllEdges    (IMG Image, long Index, 
                               long Density, TArea Area, 
                               double Threshold, 
                               BOOL PositiveEdge, 
                               long lMaxEdges,
                               PEdgeResult Result,
                               long& lEdgeCount);

IMPORT(BOOL) CFindFirstEdge    (IMG Image, long Index, 
                               long Density, TArea Area,
                               double Threshold, 
                               BOOL PositiveEdge, 
                               TEdgeResult& Result);
IMPORT(BOOL) CFindEdgePair     (IMG Image, long Index, 
                               long Density, TArea Area,
                               double Threshold1, BOOL PositiveEdge1, TEdgeResult& Result1, 
                               double Threshold2, BOOL PositiveEdge2, TEdgeResult& Result2);
IMPORT(BOOL) CFindAllEdges     (IMG Image, long Index, 
                               long Density, TArea Area, 
                               double Threshold, 
                               BOOL PositiveEdge, 
                               long lMaxEdges,
                               PEdgeResult Result,
                               long& lEdgeCount);

IMPORT(BOOL) CSFindFirstEdge   (IMG Image, long Index, 
                               long Density, TArea Area,
                               double Threshold, 
                               BOOL PositiveEdge, 
                               TEdgeResult& Result);
IMPORT(BOOL) CSFindEdgePair    (IMG Image, long Index, 
                               long Density, TArea Area,
                               double Threshold1, BOOL PositiveEdge1, TEdgeResult& Result1, 
                               double Threshold2, BOOL PositiveEdge2, TEdgeResult& Result2);
IMPORT(BOOL) CSFindAllEdges    (IMG Image, long Index, 
                               long Density, TArea Area, 
                               double Threshold, 
                               BOOL PositiveEdge, 
                               long lMaxEdges,
                               PEdgeResult Result,
                               long& lEdgeCount);

IMPORT(BOOL) GetProjection    (IMG Image, long Index, 
                               long Density, TArea Area, PROJECTION& P, long MaxLen);
IMPORT(BOOL) GetNormProjection(IMG Image, long Index, 
                               long Density, TArea Area, NORMPROJECTION& P, long MaxLen);
IMPORT(BOOL) WriteProjection  (IMG Image, long Index, long Density, TArea Area);

// returns the size of an area
IMPORT(void)  GetAreaSize     (IMG Image, TArea Area, long lDensity, long *lWidth, long *lHeight);


IMPORT(BOOL)  IsProjection        (PROJECTIONEX Projection);
IMPORT(BOOL)  ShareProjection     (PROJECTIONEX Projection);
IMPORT(BOOL)  ReleaseProjection   (PROJECTIONEX Projection);
IMPORT(BOOL)  GetProjectionEx     (IMG Image, long Index, long Density, TArea Area, PROJECTIONEX& Projection);
IMPORT(BOOL)  GetNormProjectionEx (IMG Image, long Index, long Density, TArea Area, PROJECTIONEX& Projection);
IMPORT(BOOL)  Normalize           (PROJECTIONEX Pin, PROJECTIONEX& Pout);
IMPORT(BOOL)  Filter              (PROJECTIONEX Pin, long *kernel, long lSize, PROJECTIONEX& Pout);
IMPORT(BOOL)  Derivation          (PROJECTIONEX Pin, PROJECTIONEX& Pout);
IMPORT(BOOL)  ZeroCrossings       (PROJECTIONEX P, long lMaxCount, EDGERESULTS& results);
IMPORT(BOOL)  GetLength           (PROJECTIONEX Projection, long& Length);
IMPORT(BOOL)  CopyLongBuffer      (PROJECTIONEX Projection, pLongProjection pBuffer);
IMPORT(BOOL)  CopyDoubleBuffer    (PROJECTIONEX Projection, pDoubleProjection pBuffer);

IMPORT(BOOL)  IsEdgeResults       (EDGERESULTS Results);
IMPORT(BOOL)  ShareEdgeResults    (EDGERESULTS Results);
IMPORT(BOOL)  ReleaseEdgeResults  (EDGERESULTS Results);
IMPORT(long)  EdgeResultsCount    (EDGERESULTS Results);
IMPORT(BOOL)  EdgeResult          (EDGERESULTS Results, long index, double& x, double& y, double& q);
IMPORT(BOOL)  ThresholdResults    (EDGERESULTS In, double Min, double Max, EDGERESULTS& Out);
IMPORT(BOOL)  AppendResults       (EDGERESULTS In1, EDGERESULTS In2, EDGERESULTS& Out);
IMPORT(BOOL)  SortResultsByQuality    (EDGERESULTS In);
IMPORT(BOOL)  SortResultsByAbsQuality (EDGERESULTS In);
IMPORT(BOOL)  SortResultsByPosition   (EDGERESULTS In, double px, double py);

IMPORT(BOOL) OSFindFirstEdge   (IMG Image, long Index, 
                               long Density, TArea Area,
                               double Threshold, 
                               TEdgePolarity Polarity, 
                               TEdgeResult& Result);
IMPORT(BOOL) OSFindEdgePair    (IMG Image, long Index, 
                               long Density, TArea Area,
                               double Threshold1, TEdgePolarity Polarity1, TEdgeResult& Result1, 
                               double Threshold2, TEdgePolarity Polarity2, TEdgeResult& Result2);
IMPORT(BOOL) OSFindAllEdges    (IMG Image, long Index, 
                               long Density, TArea Area, 
                               double Threshold, 
                               TEdgePolarity Polarity, 
                               EDGERESULTS& results);
IMPORT(BOOL) OSFindBestEdge    (IMG Image, long Index, 
                               long Density, TArea Area,
                               TEdgePolarity Polarity, 
                               TEdgeResult& Result);
                               
