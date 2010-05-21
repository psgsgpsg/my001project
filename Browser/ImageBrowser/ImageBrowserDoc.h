// ImageBrowserDoc.h : interface of the CImageBrowserDoc class
//
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"

#pragma once


class CImageBrowserDoc : public CDocument
{
protected: // create from serialization only
	CImageBrowserDoc();
	DECLARE_DYNCREATE(CImageBrowserDoc)

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// Implementation
public:
	virtual ~CImageBrowserDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
public:
	IplImage*	m_rawImage;
public:
	IplImage* GetImage(void);
};


