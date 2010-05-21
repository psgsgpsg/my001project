// ImageBrowserView.cpp : implementation of the CImageBrowserView class
//

#include "stdafx.h"
#include "ImageBrowser.h"

#include "ImageBrowserDoc.h"
#include "ImageBrowserView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImageBrowserView

IMPLEMENT_DYNCREATE(CImageBrowserView, CScrollView)

BEGIN_MESSAGE_MAP(CImageBrowserView, CScrollView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CImageBrowserView construction/destruction

CImageBrowserView::CImageBrowserView()
{
	// TODO: add construction code here
	m_clientSize=CSize(2500,1800);
}

CImageBrowserView::~CImageBrowserView()
{
}

BOOL CImageBrowserView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

// CImageBrowserView drawing

void CImageBrowserView::OnDraw(CDC* pDC)
{
	CImageBrowserDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
	
	IplImage*	imagePointer = pDoc->GetImage();
	
	if (imagePointer)
	{
		m_clientSize = CSize(imagePointer->width, imagePointer->height);
		BITMAPINFO	bi;
		memset(&bi, 0x00, sizeof(bi));
		bi.bmiHeader.biBitCount = imagePointer->nChannels * imagePointer->depth;
		bi.bmiHeader.biHeight = imagePointer->height;
		bi.bmiHeader.biWidth = imagePointer->width;
		bi.bmiHeader.biPlanes = 1;
		bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bi.bmiHeader.biSizeImage = imagePointer->imageSize;

		RECT	rt;
		GetClientRect(&rt);
		
		BitBlt(pDC->GetSafeHdc(), rt.left, rt.top, imagePointer->width, imagePointer->height, GetDC()->GetSafeHdc(), 0, 0, SRCCOPY);
		
	//	StretchDIBits(pDC->GetSafeHdc(), rt.left, rt.top, rt.right - rt.left, rt.bottom - rt.top,
	//		0, 0, imagePointer->width, imagePointer->height, imagePointer->imageData, &bi, DIB_RGB_COLORS, SRCCOPY);
	}
	

}

void CImageBrowserView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, m_clientSize);
}


// CImageBrowserView printing

BOOL CImageBrowserView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageBrowserView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageBrowserView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CImageBrowserView diagnostics

#ifdef _DEBUG
void CImageBrowserView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageBrowserView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageBrowserDoc* CImageBrowserView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageBrowserDoc)));
	return (CImageBrowserDoc*)m_pDocument;
}
#endif //_DEBUG


// CImageBrowserView message handlers
