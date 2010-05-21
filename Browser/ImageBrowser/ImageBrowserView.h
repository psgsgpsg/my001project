// ImageBrowserView.h : interface of the CImageBrowserView class
//


#pragma once


class CImageBrowserView : public CScrollView
{
protected: // create from serialization only
	CImageBrowserView();
	DECLARE_DYNCREATE(CImageBrowserView)

// Attributes
public:
	CImageBrowserDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CImageBrowserView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	CSize		m_clientSize;
};

#ifndef _DEBUG  // debug version in ImageBrowserView.cpp
inline CImageBrowserDoc* CImageBrowserView::GetDocument() const
   { return reinterpret_cast<CImageBrowserDoc*>(m_pDocument); }
#endif

