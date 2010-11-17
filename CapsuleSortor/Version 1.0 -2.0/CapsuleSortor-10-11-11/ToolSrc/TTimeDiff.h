// TTimeDiff.h	: interface and implement of the TTimeDiff class
//
// High-resolution Simple elapsed time class
//
// Copyright (c) 2006 zhao_fsh  Zibo Creative Computor CO.,LTD
//
//////////////////////////////////////////////////////////////////////////
//  example:
//
//	TTimeDiff td;
//	... ...
//	double msec = td.msec();
//	double usec = td.usec();
//  ... ...
//  td.Reset();
//  ... ... 
//  double timediff = td.usec();
//  
//////////////////////////////////////////////////////////////////////////
//
// Simple elapsed time tool
// Date	: 2006.10.18

#ifndef TTIMEDIFF_H
#define TTIMEDIFF_H

// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

#ifndef LONGLONG
typedef __int64 LONGLONG;
#endif

class TTimeDiff
{
public:
  TTimeDiff();  // Record the current time
  // default destructors
  // default copy destructors
  // default operator=()

public:
  double usec() const;  // Return elapsed time, in microseconds.
  double msec() const;  // Return elapsed time, in milliseconds.
  double sec() const;   // Return elapsed time, in seconds

  void Reset();			// Reset the current time

private:
  LONGLONG m_start;     // Starting time

};

inline TTimeDiff::TTimeDiff () : m_start (0)
{
	Reset();
}


inline double TTimeDiff::usec () const
{
  return sec() * 1000000;
}

inline double TTimeDiff::msec () const
{
  return sec() * 1000;
}

inline double TTimeDiff::sec () const
{
  LARGE_INTEGER t, freq;
  QueryPerformanceCounter (&t);
  QueryPerformanceFrequency (&freq);

  return (double(t.QuadPart - m_start)) / freq.QuadPart;
}

inline void TTimeDiff::Reset ()
{
  LARGE_INTEGER t;
  QueryPerformanceCounter (&t);

  m_start = t.QuadPart;
}

#endif  // TTIMEDIFF_H