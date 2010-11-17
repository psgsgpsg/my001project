#ifndef TRECT2D_H
#define TRECT2D_H

#include "TPoint2D.h"

template <typename T>
class TRect2D
{
public:
	TRect2D();
	TRect2D(const TPoint2D<T>& point0,	T width, T height);
	TRect2D(const TPoint2D<T>& point0,	const TPoint2D<T>& point1);
	TRect2D(T x0, T y0, T x1, T y1);
	bool operator==(const TRect2D& rhs) const;
	bool operator!=(const TRect2D& rhs) const;
	
	template <typename U>
	TRect2D(const TRect2D<U> &src)
	{
		Init(	static_cast<T>(src.x0()),
				static_cast<T>(src.y0()),
				static_cast<T>(src.x1()),
				static_cast<T>(src.y1()));
	}

public:
	TPoint2D<T>	Center	() const { return TPoint2D<T>(m_pt0.x() + m_width/2, m_pt0.y() + m_height/2); }
	T			x0		() const { return m_pt0.x();				}
	T			x1		() const { return TRect2DPos(m_pt0.x(), m_width);	}
	T			y0		() const { return m_pt0.y();				}
	T			y1		() const { return TRect2DPos(m_pt0.y(), m_height);	}
	T			Width	() const { return m_width;					}
	T			Height	() const { return m_height;					}
	bool		IsNull  () const { return (m_width<=0 || m_height<=0);	}

public:
	void Translate			(T deltX, T deltY);
	void TranslateLeftTopTo	(const TPoint2D<T>& point);
	void TranslateCenterTo	(const TPoint2D<T>& point);
	bool Within				(T x, T y);
	bool OnBorder			(T x, T y);
	void Expand				(T x, T y);
	void Expand				(T left, T right, T up, T down);
	TRect2D<T> Intersect	(const TRect2D<T>& rect) const;

private:
	void	Init(T x0, T y0, T x1, T y1);
	T		Max(const T& a, const T& b) const { return a>b ? a : b; }
	T		Min(const T& a, const T& b) const { return a<b ? a : b; }

private:
	TPoint2D<T> m_pt0;
	T			m_width;
	T			m_height;
};

typedef TRect2D<float>		FRect;
typedef TRect2D<double>		DRect;
typedef TRect2D<int>		IRect;

template<typename T>
TRect2D<T>::TRect2D()
: m_pt0(0,0),
  m_width(static_cast<T>(0)), 
  m_height(static_cast<T>(0))
{ }

template<typename T>
TRect2D<T>::TRect2D(const TPoint2D<T>& point0, T width, T height)
{
	T x0	= point0.x();
	T y0	= point0.y();
	T x1	= TRect2DPos(x0, width);
	T y1	= TRect2DPos(y0, height);
	Init(x0, y0, x1, y1);
}

template<typename T>
TRect2D<T>::TRect2D(const TPoint2D<T>& point0, const TPoint2D<T>& point1)
{ 
	Init(point0.x(), point0.y(), point1.x(), point1.y());	
}

template<typename T>
TRect2D<T>::TRect2D(T x0, T y0, T x1, T y1)
{
	Init(x0, y0, x1, y1);
}

template<typename T>
bool TRect2D<T>::operator==(const TRect2D& rhs) const
{
	bool isPt0eq	= (m_pt0 == rhs.m_pt0);
	bool isWHeq		= (TPoint2D<T>(m_width, m_height) == TPoint2D<T>(rhs.m_width, rhs.m_height));
	return	(isPt0eq && isWHeq);
}

template<typename T>
bool TRect2D<T>::operator!=(const TRect2D& rhs) const
{
	bool isPt0eq	= (m_pt0 != rhs.m_pt0);
	bool isWHeq		= (TPoint2D<T>(m_width, m_height) != TPoint2D<T>(rhs.m_width, rhs.m_height));
	return	(isPt0eq || isWHeq);
}

template<typename T>
void TRect2D<T>::Init(T x0, T y0, T x1, T y1)
{
	m_pt0	= TPoint2D<T>(Min(x0, x1), Min(y0, y1));
	m_width = TRect2DDist(x1, x0);
	m_height= TRect2DDist(y1, y0);
}

template<typename T>
static T TRect2DPos(T origin, T distance)
{
	distance = (distance>0)? distance-1 : (distance<0) ? distance+1 : 0;
	return origin + distance;
}
template<> double TRect2DPos(double origin, double distance);
template<> float  TRect2DPos(float  origin, float  distance);

template<typename T>
static T TRect2DDist(T pos0, T pos1)
{
	T dist = pos1 - pos0;
	return (dist>=0)? dist+1 : -dist+1;
}
template<> double TRect2DDist(double pos0, double pos1);
template<> float  TRect2DDist(float  pos0, float  pos1);

template<typename T>
void TRect2D<T>::Translate	(T deltX, T deltY)
{
	m_pt0 += TPoint2D<T>(deltX, deltY);
}

template<typename T>
void TRect2D<T>::TranslateLeftTopTo (const TPoint2D<T>& point)
{
	m_pt0  = point;
}

template<typename T>
void TRect2D<T>::TranslateCenterTo  (const TPoint2D<T>& point)
{
	T left	= point.x() - m_width/2;
	T top	= point.y() - m_height/2;
	m_pt0	= TPoint2D<T>(left, top);
}

template<typename T>
bool TRect2D<T>::Within	(T x, T y)
{
	return (x0() <= x) && (x1() >= x) &&
		   (y0() <= y) && (y1() >= y);
}

template<typename T>
bool TRect2D<T>::OnBorder(T x, T y)
{
	bool success = false;
	if(Within(x, y))
	{
		success =  (x==x0() || x==x1() || y==y0() || y==y1());
	}
	return success;
}

template<typename T>
void TRect2D<T>::Expand(T x, T y)
{
	m_pt0		+= TPoint2D<T>(-x, -y);
	m_width		+= 2*x;
	m_height	+= 2*y;
}

template<typename T>
void TRect2D<T>::Expand(T left, T right, T up, T down)
{
	m_pt0		+= TPoint2D<T>(-left, -up); 
	m_width		+= (left + right);
	m_height	+= (up   + down );
}

template<typename T>
TRect2D<T> TRect2D<T>::Intersect(const TRect2D<T>& rect) const
{
	TRect2D<T> intersectRect;
	if((!rect.IsNull()) && (!(this->IsNull())))
	{
		T xx0 = Max(x0(), rect.x0());
		T yy0 = Max(y0(), rect.y0());
		T xx1 = Min(x1(), rect.x1());
		T yy1 = Min(y1(), rect.y1());
		if((xx1 >= xx0) && (yy1 >= yy0))
		{
			intersectRect.Init(xx0, yy0, xx1, yy1);
		}
	}
	return intersectRect;
}

#endif // TRECT2D_H