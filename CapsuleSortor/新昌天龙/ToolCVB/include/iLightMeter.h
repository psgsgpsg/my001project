/*******************************************************************************
                                   MW-Design
--------------------------------------------------------------------------------

  File     : iLightMeter.h
  Author   : Manfred Wuetschner
  Date     : June.26.2000
  Library  : ----------------
  Hardware : ----------------
  Compiler : MSVC 6.0
  Text     : functions of LightMeter.dll.
  Revision : 
             December.12.2000 mw
             new function LMGetSingleHistogramEntry
             September.15.2002 mw
             several changes

*******************************************************************************/
#ifndef __ILIGHTMETER_INCLUDED__
  #define __ILIGHTMETER_INCLUDED__

#ifdef __cplusplus
  #define IMPORT(t) extern "C" t __stdcall
#else
  #define IMPORT(t) t __stdcall
#endif

#include "iCVCImg.h"

//LightMeter Handle
typedef long LMH;

/*******************************************************************************
exported functions
*******************************************************************************/
IMPORT(STDMETHODIMP) LMCreate(LMH *LMHandle);
IMPORT(STDMETHODIMP) LMDestroy(LMH LMHandle);
IMPORT(STDMETHODIMP) LMGetLastMessageString(LMH LMHandle, LPTSTR lpString, long lMaxStringLength);
IMPORT(STDMETHODIMP) LMGetStopWatch(LMH LMHandle, long lIndex, double *pdStopWatch);
IMPORT(STDMETHODIMP) LMExecute(LMH LMHandle);
IMPORT(STDMETHODIMP) LMGetImage(LMH LMHandle, IMG *Image);
IMPORT(STDMETHODIMP) LMSetImage(LMH LMHandle, IMG Image);
IMPORT(STDMETHODIMP) LMGetArea(LMH LMHandle, long lPlaneIndex, TArea *pArea);
IMPORT(STDMETHODIMP) LMSetArea(LMH LMHandle, long lPlaneIndex, TArea Area);
IMPORT(STDMETHODIMP) LMGetUseCSFlag(LMH LMHandle, long lPlaneIndex, long *plUseCSFlag);
IMPORT(STDMETHODIMP) LMSetUseCSFlag(LMH LMHandle, long lPlaneIndex, long lUseCSFlag);
IMPORT(STDMETHODIMP) LMGetResetCSFlag(LMH LMHandle, long *plResetCSFlag);
IMPORT(STDMETHODIMP) LMSetResetCSFlag(LMH LMHandle, long lResetCSFlag);
IMPORT(STDMETHODIMP) LMGetEntireImageFlag(LMH LMHandle, long lPlaneIndex, long *plEntireImageFlag);
IMPORT(STDMETHODIMP) LMSetEntireImageFlag(LMH LMHandle, long lPlaneIndex, long lEntireImageFlag);
IMPORT(STDMETHODIMP) LMGetDataBuffer(LMH LMHandle, long lPlaneIndex, LPVOID *plpvDataBuffer);
IMPORT(STDMETHODIMP) LMGetDataBufferEntrySize(LMH LMHandle, long lPlaneIndex, long *plDataBufferEntrySize);
IMPORT(STDMETHODIMP) LMGetDataBufferEntries(LMH LMHandle, long lPlaneIndex, unsigned long *pulDataBufferEntries);
IMPORT(STDMETHODIMP) LMGetProcessFlag(LMH LMHandle, long lPlaneIndex, long *plProcessFlag);
IMPORT(STDMETHODIMP) LMSetProcessFlag(LMH LMHandle, long lPlaneIndex, long lProcessFlag);
IMPORT(STDMETHODIMP) LMGetDimension(LMH LMHandle, long *plDimension);
IMPORT(STDMETHODIMP) LMGetDataType(LMH LMHandle, long lPlaneIndex, long *plDataType);
IMPORT(STDMETHODIMP) LMGetDensity(LMH LMHandle, long lPlaneIndex, long *plDensity);
IMPORT(STDMETHODIMP) LMSetDensity(LMH LMHandle, long lPlaneIndex, long lDensity);
IMPORT(STDMETHODIMP) LMGetStatisticTotal(LMH LMHandle, long lPlaneIndex, double *pdTotal);
IMPORT(STDMETHODIMP) LMGetStatisticMean(LMH LMHandle, long lPlaneIndex, double *pdMean);
IMPORT(STDMETHODIMP) LMGetStatisticMode(LMH LMHandle, long lPlaneIndex, double *pdMode);
IMPORT(STDMETHODIMP) LMGetStatisticMin(LMH LMHandle, long lPlaneIndex, double *pdMin);
IMPORT(STDMETHODIMP) LMGetStatisticMax(LMH LMHandle, long lPlaneIndex, double *pdMax);
IMPORT(STDMETHODIMP) LMGetStatisticVariance(LMH LMHandle, long lPlaneIndex, double *pdVariance);
IMPORT(STDMETHODIMP) LMGetStatisticStdDev(LMH LMHandle, long lPlaneIndex, double *pdStdDev);
IMPORT(STDMETHODIMP) LMGetStatisticPixelInLimits(LMH LMHandle, long lPlaneIndex, unsigned long ulLowerLimit, unsigned long ulUpperLimit, double *pdPixelInLimits);
IMPORT(STDMETHODIMP) LMGetSingleHistogramEntry(LMH LMHandle, long lPlaneIndex, long lEntryIndex, double *pdEntry);

#endif  //__ILIGHTMETER_INCLUDED__