#include <assert.h>
#include "TImgPainter.h"

TImgPainter::TImgPainter(HDC hDC)
: m_hDC(0),
  m_hOldPen(0)
{
	assert(hDC != NULL);
	m_hDC		= hDC;
	HPEN hPen	= CreatePen(PS_SOLID, 1, RGB(128, 128, 128));
	m_hOldPen	= (HPEN)SelectObject(m_hDC, hPen);
}

TImgPainter::~TImgPainter()
{
	m_hOldPen = (HPEN)SelectObject(m_hDC, m_hOldPen);
	DeleteObject(m_hOldPen);
}

void TImgPainter::Coordinate(const TPaintCoord& coord)
{
	m_paintCoord = coord;
}

void TImgPainter::SetOrigin	(const FPoint& point)
{
	m_paintCoord.SetOrigin(point);
}

void TImgPainter::SetZoom(float scaleX, float scaleY)
{
	m_paintCoord.SetZoom(scaleX, scaleY);
}

const TPaintCoord& TImgPainter::Coordinate() const
{
	return m_paintCoord;
}

void TImgPainter::SetPenParam(COLORREF color, size_t width, int style)
{
	HPEN hPen	= CreatePen(style, static_cast<int>(width), color);
	hPen		= (HPEN)SelectObject(m_hDC, hPen);
	DeleteObject(hPen);
}

void TImgPainter::MoveTo  (const FPoint& point)
{
	IPoint pos = m_paintCoord.ToCanvasSys(point);
	::MoveToEx(m_hDC, pos.x(), pos.y(), NULL);
}

void TImgPainter::LineTo  (const FPoint& point)
{
	IPoint pos = m_paintCoord.ToCanvasSys(point);
	::LineTo(m_hDC, pos.x(), pos.y());
}

void TImgPainter::SetPixel(const FPoint& point, const COLORREF color)
{
	IPoint pos = m_paintCoord.ToCanvasSys(point);
	::SetPixel(m_hDC, pos.x(), pos.y(), color);
}

void TImgPainter::SetText (const FPoint& point, const TString& text)
{
	IPoint pos = m_paintCoord.ToCanvasSys(point);
	::TextOut(m_hDC, pos.x(), pos.y(), text.c_str(), text.size());
}

void TImgPainter::DrawRect (const FRect& rect)
{
	float x0 = rect.x0();
	float y0 = rect.y0();
	float x1 = rect.x1();
	float y1 = rect.y1();

	MoveTo(FPoint(x0, y0));
	LineTo(FPoint(x1, y0));
	LineTo(FPoint(x1, y1));
	LineTo(FPoint(x0, y1));
	LineTo(FPoint(x0, y0));
}

void TImgPainter::Arc (	const FPoint& center, 
						float axisH,
						float axisV,
						const FPoint& beginPos,
						const FPoint& endPos,
						bool  toLink)
{
	FRect boundary(center, 0.0f, 0.0f);
	boundary.Expand(fabs(axisH), fabs(axisV));
	IPoint bound0 = m_paintCoord.ToCanvasSys(FPoint(boundary.x0(), boundary.y0()));
	IPoint bound1 = m_paintCoord.ToCanvasSys(FPoint(boundary.x1(), boundary.y1()));
	IPoint pos0	  = m_paintCoord.ToCanvasSys(center + beginPos*100.0f);
	IPoint pos1   = m_paintCoord.ToCanvasSys(center + endPos*100.0f);
	if(toLink)
	{
		::ArcTo(m_hDC, bound0.x(), bound0.y(), bound1.x(), bound1.y(),
			pos0.x(), pos0.y(), pos1.x(), pos1.y());
	}
	else
	{
		::Arc  (m_hDC, bound0.x(), bound0.y(), bound1.x(), bound1.y(),
			pos0.x(), pos0.y(), pos1.x(), pos1.y());
	}
}
