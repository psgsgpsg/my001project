// TAllocTemp.h : Interface and Implement of the TAlloc class
//
// Copyright (c) 2006 PACS Group  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
// Module : Common Tool
// Create Date	: 2006.12.13
//
// A tool to allocate huge memery;

#ifndef TALLOCTEMP_H
#define TALLOCTEMP_H

#include "TCritSect.h"

#define NEEDS_UINTPTR_T
#ifdef  NEEDS_UINTPTR_T
typedef unsigned int uintptr_t;
#endif

typedef enum {
	eNoAlign	= 0,
	e2ByteAlign = 2,
	e4ByteAlign = 4,
	e8ByteAlign = 8,
	e16ByteAlign = 16
} eAlignment;

template <class T>
class TAlloc
{
public:


public:
	TAlloc();
	explicit TAlloc(size_t size, eAlignment align = eNoAlign);
	~TAlloc();

	TAlloc            (const TAlloc& src);
	TAlloc& operator= (const TAlloc& src);

	static TAlloc& NullObject();

public:
	size_t		Size	() const;
	size_t		Ref		() const;
	bool		IsNull	() const;

	const T*	Base	() const;
	T*			Base	();

	const T&	operator[](size_t index) const;
	T&			operator[](size_t index);

	TAlloc<T>	Duplicate() const;
	void		SetZero ();

private:
	TCritSect&	 LockObject() const;

private:
	class TAllocRef;

	TAllocRef*		Clone() const;	//
	TAllocRef*		m_pMem;		// pointer to our allocated memory
	static TAlloc*	m_sNull;	// our null object

private:
	class TAllocRef
	{
	public:
		TAllocRef(size_t size, eAlignment align)
			: m_pRaw(0), m_pData(0), m_size(size), m_ref(0), m_align(align)
		{
			Allocate();
			AddRef();
		}
		
		~TAllocRef()
		{
			DeAllocate();
		}

		operator	   T*	()			{ return m_pData; }
		operator const T*	() const	{ return m_pData; }
		
		TCritSect& LockObject() const	{ return m_locker; }

		size_t		 Size	() const	{ return m_size; }
		size_t		 Ref	() const	{ return m_ref;  }
		eAlignment	 Align	() const	{ return m_align;}

		void AddRef()
		{
			TLocker lock( LockObject() );
			m_ref++;
		}
		
		void SubRef()
		{
			{
				TLocker lock( LockObject() );
				--m_ref;
			}
			if( m_ref == 0)
				delete this;
		}

		void Allocate()
		{
			if(0 == m_size)
			{
				m_pData = new T [1];
				m_pRaw	= 0;
				return;
			}
			
			if(m_align < 2)
			{
				m_pData = new T [m_size];
				m_pRaw	= 0;
			}
			else
			{
				m_pRaw	= new char [sizeof(T) * m_size + (m_align -1)];
				m_pData	= AlignPointer (m_pRaw);
			}
		}

		void DeAllocate()
		{
			if(m_pRaw)
				delete [] m_pRaw;
			else
				delete [] m_pData;
			
			m_pRaw	= 0;
			m_pData = 0;
		}

	private:
		TAllocRef			 (const TAllocRef &src);
		TAllocRef& operator= (const TAllocRef &src);

		T* AlignPointer(void *raw)
		{
			T* p = reinterpret_cast<T*> (
				(reinterpret_cast<uintptr_t>(raw) + m_align -1) & ~(m_align -1) );
			return p;
		}

	private:
		char*		 m_pRaw;
		T*			 m_pData;
		size_t		 m_size;
		size_t		 m_ref;
		eAlignment	 m_align;

		mutable TCritSect m_locker;
	};
};

template <class T>
TAlloc<T>* TAlloc<T>::m_sNull = 0;

template <class T>
TAlloc<T>& TAlloc<T>::NullObject()
{
  if (!m_sNull)
    m_sNull = new TAlloc (0);
  return *m_sNull;
}

template<class T>
TAlloc<T>::TAlloc() : m_pMem(0)
{
  // Point ourself to sNull.
  m_pMem = NullObject().m_pMem;
  m_pMem->AddRef();
}

template <class T>
TAlloc<T>::TAlloc (size_t size, eAlignment align)
: m_pMem(0)
{
	m_pMem = new TAlloc::TAllocRef(size, align);
}

template <class T>
TAlloc<T>::~TAlloc ()
{
	m_pMem->SubRef();
}

template<class T>
TAlloc<T>::TAlloc (const TAlloc& src)
{
	TLocker lock (src.LockObject() );
	m_pMem = src.m_pMem;
	m_pMem ->AddRef();
}

template<class T>
TAlloc<T>& TAlloc<T>::operator= (const TAlloc& src)
{
	// Make sure we don't copy ourself!
	if (m_pMem == src.m_pMem)
		return *this;
	
	// Remove reference from existing object.
	m_pMem ->SubRef ();

	{
		TLocker lock (src.LockObject() );
		m_pMem = src.m_pMem;
		m_pMem ->AddRef ();
	}

	return *this;
}

template <class T>
typename TAlloc<T>::TAllocRef* TAlloc<T>::Clone() const
{
	size_t size = m_pMem->Size();
	TAllocRef* copy = new TAllocRef(size, m_pMem->Align() );

	// Shallow copy
	T* src = *m_pMem;
	T* dst = *copy;
	memcpy(dst, src, size * sizeof(T) );

  return copy;
}


template<class T>
TAlloc<T> TAlloc<T>::Duplicate() const
{
	TAlloc<T>  dup;
	if(!IsNull())
	{
		// Duplicate our existing memory
		TAllocRef* copy = Clone();
		dup.m_pMem = copy;
	}
	return dup;
}

template<class T>
void TAlloc<T>::SetZero()
{
	T* pBase = Base();
	memset(pBase, 0, sizeof(T)*Size());
}

template <class T>
inline unsigned int TAlloc<T>::Size () const { return m_pMem -> Size(); }

template <class T>
inline unsigned int TAlloc<T>::Ref	 () const { return m_pMem -> Ref();  }

template <class T>
inline bool TAlloc<T>::IsNull()   const {	return (m_pMem == NullObject().m_pMem); }

template <class T>
inline const T* TAlloc<T>::Base() const {	return *m_pMem; }

template <class T>
inline T* TAlloc<T>::Base() { return *m_pMem; }

template <class T>
inline const T&	TAlloc<T>::operator[](size_t index) const
{
	return *(Base() + index);
}

template <class T>
inline T& TAlloc<T>::operator[](size_t index)
{
	return *(Base() +index);
}

template <class T>
inline TCritSect& TAlloc<T>::LockObject() const
{
	return m_pMem->LockObject();
}

#endif //TALLOCTEMP_H