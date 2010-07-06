#ifndef SORTOBSERVER_H
#define SORTOBSERVER_H

#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

#include "iCVCImg.h"

class SortObserver
{
public:
	typedef enum {	Mono	= 0,
					Color	= 1,

					MAll	= 100,
					MSub	= 110,
					MPart	= 120,
					MDyn	= 140,
					MResult	= 160,
					MTime	= 161,
					MSobel	= 190,

					CAll	= 200,
					CSub	= 210,
					CPart	= 220,
					CDyn	= 240,
					CResult = 260,
					CTime	= 261,
					CHue	= 270,
					CSobel	= 290
	} ETarget;

public:
	SortObserver();
	~SortObserver();
public:
	bool SpecifyTarget	(size_t target);
	bool ObserverIMG	(ETarget eTarget, IMG img);
	bool ObserverParam	(ETarget eTarget, double param);
	bool NewPeriod		(ETarget eType );
	IMG	   GetImage		( ) const;
	double GetParam		( ) const;
private:
	size_t	m_specTarget;
	IMG		m_targetIMG;
	double	m_targetParam;

private:
	size_t	m_mSub;
	size_t	m_mPart;
	size_t	m_mDyn;
	size_t	m_mSoble;
	size_t	m_cSub;
	size_t	m_cPart;
	size_t	m_cDyn;
	size_t	m_cHue;
	size_t	m_cSoble;
};

#endif // SORTOBSERVER_H