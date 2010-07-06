// TEvent.h	: Interface of the TEvent class
//
// Copyright (c) 2006 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Create Date	: 2006.12.19
//
#ifndef TEVENT_H
#define TEVENT_H

// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

class TEvent
{
public:
	TEvent(	bool initState		= false,
			bool manualReset	= true,
			LPCTSTR pName		= 0);

	~TEvent();

public:
	bool Wait(unsigned int ms = -1);
	bool SetEvent();		//Sets the event to available 
	bool ResetEvent();		//Sets the event to unavailable 
	bool PulseEvent();		//Sets the event to available, then sets the event to unavailable.
							//if manual-reset, all waiting threads that can be released immediately are released.
							//if auto-reset, releasing a single waiting thread, even if multiple threads are waiting. 

	HANDLE GetHandle() const;	//Get the handle of the Event.

private:
	TEvent(const TEvent &src);
	TEvent &operator= (const TEvent &src);

private:
	HANDLE m_hEvent;
};

inline TEvent::TEvent(bool initState, bool manualReset, LPCTSTR pName)
{
	BOOL bState  = initState	?	TRUE : FALSE;
	BOOL bManual = manualReset	?	TRUE : FALSE;
	m_hEvent = CreateEvent(NULL, bManual, bState, pName);
}

inline TEvent::~TEvent()
{
	if(m_hEvent)
		CloseHandle(m_hEvent);
}

inline bool TEvent::Wait(unsigned int ms)
{
	if(NULL == m_hEvent) return false;  
	DWORD wait		= ( ms == -1) ? INFINITE : ms;
	DWORD status	= WaitForSingleObject(m_hEvent, wait);
	return (status != WAIT_TIMEOUT);
}

inline bool TEvent::SetEvent()
{
	return ::SetEvent(m_hEvent)	  ? true : false;
}

inline bool TEvent::ResetEvent()
{
	return ::ResetEvent(m_hEvent) ? true : false;
}

inline bool TEvent::PulseEvent()
{
	return ::PulseEvent(m_hEvent) ? true : false;
}

inline HANDLE TEvent::GetHandle() const
{
	return m_hEvent;
}
#endif //TEVENT_H
