// TThread.h	: Interface of the TThread class
//
// Copyright (c) 2006 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
//
// Last update Date	: 2007.05.07
//
// Example Begin
/*
// First method:
class myClass :public TThread
{
public:
	  virtual void Thread ()
	  { 
		  std::cout << "with virtual thread!" << std::endl;
	  }
};
// Second method:
void TheadContain(void* param)
{
		  std::cout << "with callback function!" <<std::endl;
}
int main()
{
	myClass my;
	my.Start();
	bool ok = my.Wait();

	TThread threadobj;
	threadobj.SetThreadFunc(TheadContain,NULL);
	threadobj.Start();
	threadobj.Wait();
}
*/
// Example end
//
// Simple thread control tool

#ifndef TTHREAD_H
#define TTHREAD_H

// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

typedef void ThreadFunc(void* param);

class TThread
{
public:
	typedef enum {	CRITICAL = THREAD_PRIORITY_TIME_CRITICAL,
					HIGHEST  = THREAD_PRIORITY_HIGHEST,
					ABOVE	 = THREAD_PRIORITY_ABOVE_NORMAL,
					NORMAL	 = THREAD_PRIORITY_NORMAL,
					BELOW	 = THREAD_PRIORITY_BELOW_NORMAL,
					LOWEST	 = THREAD_PRIORITY_LOWEST,
					IDLE	 = THREAD_PRIORITY_IDLE
				} EnumPriority; 

public:
	TThread(); 
	~TThread ();
public:
	bool	Start	(EnumPriority ePriority = NORMAL);	// Start the thread running
	bool	Stop	();							// Stop the thread. 
	bool	Suspend	();							// Suspend the thread.
	bool	Resume	();							// Resume the thread.
	bool	Wait  (unsigned int ms = -1);		// Wait for thread to complete
	void	Sleep (unsigned int ms);			// Sleep for ms milliseconds
	void	SetThreadFunc(ThreadFunc* proc, void* param);	// Set Callback Function and it's param

	EnumPriority	GetPriority();							// Get thread priority.
	bool			SetPriority	(EnumPriority ePriority);	// Set thread priority.

protected:
	TThread(const TThread& src);
	TThread& operator = (const TThread& src);

	static unsigned int __stdcall StaticThread (void* obj);
	virtual void Thread ();		  // Thread function, Override this in derived classes.

private:
	HANDLE		m_hThread;
	ThreadFunc	*ThreadProc;
	void		*m_param;
	EnumPriority m_ePriority;
public:
	class CritSect
	{
	public:
		CritSect	()	{ InitializeCriticalSection	(&m_critsect);	}
		~CritSect	()	{ DeleteCriticalSection		(&m_critsect);	}
		void Enter	()	{ EnterCriticalSection		(&m_critsect);	}
		void Leave	()	{ LeaveCriticalSection		(&m_critsect);	}
	private:
		CRITICAL_SECTION m_critsect;
	};

	class Lock
	{
	public:
		Lock(CritSect *pLocker)
		{
			m_pLocker = pLocker;
			m_pLocker->Enter();
		}
		~Lock()
		{
			m_pLocker->Leave();
		}
	private:
		CritSect *m_pLocker;
	};

private:	
	CritSect	m_lock;
};

#endif // TTHREAD_H
