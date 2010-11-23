
#include "SortObserver.h"

SortObserver::SortObserver()
:	m_specTarget(0),
	m_targetIMG(0),
	m_targetParam(0),
	m_mSub	(MSub),
	m_mPart	(MPart),
	m_mDyn	(MDyn),
	m_mSoble(MSobel),

	m_cSub	(CSub),
	m_cPart	(CPart),
	m_cDyn	(CDyn),
	m_cHue	(CHue),
	m_cSoble(CSobel)
{ }

SortObserver::~SortObserver()
{
	ReleaseImage(m_targetIMG);
}

bool SortObserver::SpecifyTarget(size_t target)
{
	m_specTarget = target;
	return true;
}

SortObserver& SortObserver::TheSortObserver()
{
	static SortObserver observer;
	return observer;
}

bool SortObserver::ObserverIMG(ETarget eTarget, IMG img)
{
	bool	success		= false;
	bool	isImage		= true;
	size_t	target		= 0;
	switch(eTarget)
	{
	case MAll:
		target = MAll;
		break;
	case MBin:
		target = MBin;
		break;
	case MEnh:
		target = MEnh;
		break;
	case MAnd:
		target = MAnd;
		break;
	case CAll:
		target = CAll;
		break;

	case MSub:
		target = m_mSub++;
		break;

	case MPart:
		target = m_mPart++;
		break;

	case MDyn:
		target = m_mDyn++;
		break;
		
	case MSobel:
		target = m_mSoble++;
		break;

	case CSub:
		target = m_cSub++;
		break;

	case CPart:
		target = m_cPart++;
		break;

	case CDyn:
		target = m_cDyn++;
		break;

	case CSobel:
		target = m_cSoble++;
		break;

	default:
		isImage = false;
	}
	if(isImage && (m_specTarget==target))
	{
		ReleaseImage(m_targetIMG);
		CreateDuplicateImage(img, m_targetIMG);
		//m_targetIMG = img;
		//ShareImage(m_targetIMG);
		success = true;
	}
	return success;
}

bool SortObserver::ObserverParam(ETarget eTarget, double param)
{
	bool success	= false;
	size_t target	= eTarget;
	if( (m_specTarget==target) || (m_specTarget==m_cHue) )
	{
		m_targetParam = param;
		success = true;
	}
	(eTarget==CHue) ? m_cHue++ : m_cHue;
	return success;
}

bool SortObserver::NewPeriod(ETarget eType)
{
	bool success = true;
	switch(eType)
	{
	case Mono:
		m_mSub	=	(MSub);
		m_mPart	=	(MPart);
		m_mDyn	=	(MDyn);
		m_mSoble=	(MSobel);
		break;
	case Color:
		m_cSub	=	(CSub);
		m_cPart	=	(CPart);
		m_cDyn	=	(CDyn);
		m_cHue	=	(CHue);
		m_cSoble=	(CSobel);
		break;
	default:
		success = false;
	}
	return success;
}

IMG	SortObserver::GetImage( ) const
{
	return m_targetIMG;
}

double SortObserver::GetParam( ) const
{
	return m_targetParam;
}
