// Image row Iterator class
#ifndef TIMGROWITER_H
#define TIMGROWITER_H

#include "PixelType.h"

template<class T>
class TImgRowIter
{
private:
	struct current
	{
		T*  p;      // Pointer to start of row y
		int x;      // x coordinate of start of row
		int y;      // y coordinate of current row
		int bytes;  // Number of bytes to offset to next row
	};

public:
	TImgRowIter () 
	{ m_cur.p = 0; m_cur.x = 0; m_cur.y = 0; m_cur.bytes = 0;}
	TImgRowIter (T* p, int bytes, int x, int y) 
	{ m_cur.p = p; m_cur.bytes = bytes; m_cur.x = x; m_cur.y = y;}

	T* Base()
	{
		return m_cur.p;
	}
	
	TImgRowIter& operator++ ()
	{ 
		m_cur.p = reinterpret_cast<T*>
			(reinterpret_cast<PixelMem*>(m_cur.p) + m_cur.bytes);
		m_cur.y++;
		return *this;
	}
	TImgRowIter operator++ (int)
	{
		TImgRowIter t = *this;
		m_cur.p = reinterpret_cast<T*>
			(reinterpret_cast<PixelMem*>(m_cur.p) + m_cur.bytes);
		m_cur.y++;
		return t;
	}
	
	TImgRowIter& operator-- ()
	{ 
		m_cur.p = reinterpret_cast<T*>
			(reinterpret_cast<PixelMem*>(m_cur.p) - m_cur.bytes);
		m_cur.y--;
		return *this;
	}
	TImgRowIter operator-- (int)
	{
		TImgRowIter t = *this;
		m_cur.p = reinterpret_cast<T*>
			(reinterpret_cast<PixelMem*>(m_cur.p) - m_cur.bytes);
		m_cur.y--;
		return t;
	}
	
	TImgRowIter& operator+= (int s)
	{ 
		m_cur.p = reinterpret_cast<T*>
			(reinterpret_cast<PixelMem*>(m_cur.p) + s*m_cur.bytes);
		m_cur.y += s;
		return *this;
	}
	
	TImgRowIter& operator-= (int s)
	{ 
		m_cur.p = reinterpret_cast<T*>
			(reinterpret_cast<PixelMem*>(m_cur.p) - s*m_cur.bytes);
		m_cur.y -= s;
		return *this;
	}
	
	bool operator== (const TImgRowIter& i) const
	{ return (m_cur.p == i.m_cur.p);}
	bool operator!= (const TImgRowIter& i) const
	{ return (m_cur.p != i.m_cur.p);}
	
	// Default copy constructor and assignment operators ok.
private:
	current m_cur;
};

#endif //TIMGROWITER_H