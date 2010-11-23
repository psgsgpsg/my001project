#ifndef TPOINT2D_H
#define TPOINT2D_H

#include <cmath>
#include "TToInteger.h"

template <typename T>
class TPoint2D
{
public:
	TPoint2D()
	: m_x(static_cast<T>(0)), m_y(static_cast<T>(0))
	{ }

	template<typename U, typename V>
	TPoint2D(U x, V y)
	: m_x(static_cast<T>(x)), m_y(static_cast<T>(y))
	{ }

	template<typename U>
	TPoint2D(const TPoint2D<U>& src)
	: m_x(static_cast<T>(src.x())), m_y(static_cast<T>(src.y()))
	{ }

	const T& x() const { return m_x; }
	const T& y() const { return m_y; }

	void x (const T& val) { m_x = val; }
	void y (const T& val) { m_y = val; }

	double Mag	()	const { return (sqrt(double(m_x*m_x + m_y*m_y)));	}
	double Arg	()	const { return atan2((double)m_y, (double)m_x);						}

	void Polar	(double mag, double arg);

	TPoint2D Rotate	(double angle) const;
	TPoint2D Rotate	(const  TPoint2D& center, double angle) const;

public:
	TPoint2D& operator+=(const TPoint2D& rhs);
	TPoint2D  operator+ (const TPoint2D& rhs) const;
	TPoint2D& operator-=(const TPoint2D& rhs);
	TPoint2D  operator- (const TPoint2D& rhs) const;
	TPoint2D& operator*=(const double scale );
	TPoint2D  operator* (const double scale ) const;
	TPoint2D& operator/=(const double scale );
	TPoint2D  operator/ (const double scale ) const;
	TPoint2D  operator- ( )					  const;

	bool	  operator==(const TPoint2D& rhs) const;
	bool	  operator!=(const TPoint2D& rhs) const;

private:
	T m_x;
	T m_y;

};

typedef TPoint2D<float>		FPoint;
typedef TPoint2D<double>	DPoint;
typedef TPoint2D<int>		IPoint;

template<typename T>
TPoint2D<T>& TPoint2D<T>::operator+=(const TPoint2D<T>& rhs)
{
	m_x += rhs.m_x;
	m_y += rhs.m_y;
	return *this;
}

template<typename T>
TPoint2D<T> TPoint2D<T>::operator+(const TPoint2D<T>& rhs) const
{
	TPoint2D<T> point(*this);
	return point += rhs;
}

template<typename T>
TPoint2D<T>& TPoint2D<T>::operator-=(const TPoint2D<T>& rhs)
{
	m_x -= rhs.m_x;
	m_y -= rhs.m_y;
	return *this;
}

template<typename T>
TPoint2D<T> TPoint2D<T>::operator-(const TPoint2D<T>& rhs) const
{
	TPoint2D<T> point(*this);
	return point -= rhs;
}
template<typename T>
TPoint2D<T>& TPoint2D<T>::operator*=(const double scale )
{
	m_x = TPoint2DCvt(m_x * scale, m_x);
	m_y = TPoint2DCvt(m_y * scale, m_y);
	return *this;
}
template<typename T>
TPoint2D<T>	TPoint2D<T>::operator* (const double scale ) const
{
	TPoint2D<T> point(*this);
	return point *= scale;
}

template<typename T>
TPoint2D<T>& TPoint2D<T>::operator/=(const double scale )
{
	m_x = TPoint2DCvt(m_x / scale, m_x);
	m_y = TPoint2DCvt(m_y / scale, m_y);
	return *this;
}
template<typename T>
TPoint2D<T>	TPoint2D<T>::operator/ (const double scale ) const
{
	TPoint2D<T> point(*this);
	return point /= scale;
}

template<typename T>
TPoint2D<T> TPoint2D<T>::operator-( ) const
{
	return TPoint2D<T>(-m_x, -m_y);
}

template<typename T>
bool TPoint2D<T>::operator== (const TPoint2D& rhs) const
{
	return TPoint2DEq(*this, rhs);
}

template<typename T>
bool TPoint2D<T>::operator!=(const TPoint2D& rhs) const
{
	return !TPoint2DEq(*this, rhs);
}

template<typename T>
static bool TPoint2DEq(const TPoint2D<T>& point0, const TPoint2D<T>& point1)
{
	return ( (point0.x()==point1.x()) && (point0.y()==point1.y()) );
}

template<> bool TPoint2DEq(const TPoint2D<float>& point0, const TPoint2D<float>& point1);
template<> bool TPoint2DEq(const TPoint2D<double>& point0, const TPoint2D<double>& point1);

template<typename T> 
static T TPoint2DCvt(const double value, const T flag=0)
{
	return TToInteger::Round(value);
}

template<> double TPoint2DCvt(const double value, const double flag);
template<> float  TPoint2DCvt(const double value, const float  flag);

template<typename T>
TPoint2D<T> TPoint2D<T>::Rotate(double angle) const
{
	double cosA = cos(angle);
	double sinA = sin(angle);

	T x = TPoint2DCvt(m_x*cosA - m_y*sinA, m_x);
	T y = TPoint2DCvt(m_x*sinA + m_y*cosA, m_y);

	return TPoint2D(x, y);
}

template<typename T>
TPoint2D<T> TPoint2D<T>::Rotate(const TPoint2D<T>& center, double angle) const
{
	TPoint2D<T> rot = (*this) - center;
	return center + rot.Rotate(angle);
}

template<typename T>
void TPoint2D<T>::Polar(double mag, double arg)
{
	m_x = TPoint2DCvt(mag*cos(arg), m_x);
	m_y = TPoint2DCvt(mag*sin(arg), m_y);
}

#endif // TPOINT2D_H