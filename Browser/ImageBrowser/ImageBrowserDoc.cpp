// ImageBrowserDoc.cpp : implementation of the CImageBrowserDoc class
//

#include "stdafx.h"
#include "ImageBrowser.h"

#include "ImageBrowserDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageBrowserDoc

IMPLEMENT_DYNCREATE(CImageBrowserDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageBrowserDoc, CDocument)
END_MESSAGE_MAP()


// CImageBrowserDoc construction/destruction

CImageBrowserDoc::CImageBrowserDoc()
{
	// TODO: add one-time construction code here
	m_rawImage = NULL;
}

CImageBrowserDoc::~CImageBrowserDoc()
{
}

BOOL CImageBrowserDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CImageBrowserDoc serialization

void CImageBrowserDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CImageBrowserDoc diagnostics

#ifdef _DEBUG
void CImageBrowserDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageBrowserDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CImageBrowserDoc commands

BOOL CImageBrowserDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  Add your specialized creation code here
	if (!m_rawImage)
	{
		cvReleaseImage(&m_rawImage);
	}

	char imagePath[256];
	memset(imagePath, 0x00, sizeof(imagePath));
	WideCharToMultiByte(CP_ACP, 0, lpszPathName, -1, imagePath, sizeof(imagePath),
		NULL, NULL);
	m_rawImage = cvLoadImage(imagePath, CV_LOAD_IMAGE_ANYDEPTH | CV_LOAD_IMAGE_ANYCOLOR);

	return TRUE;
}

IplImage* CImageBrowserDoc::GetImage(void)
{
	return m_rawImage;
}
