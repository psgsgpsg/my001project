// TThread.cpp	: implement of the TThread class
//
// Copyright (c) 2006 zhao_fsh  Zibo Creative Computor CO.,LTD

//////////////////////////////////////////////////////////////////////////
//  程序说明：
//	使用TThread即可直接使用，亦可作为基类使用。
//  1. 直接使用时，使用SetThreadFunc(ThreadFunc* proc, void* param);指定工作
//  线程回调函数即可，（回调函数是全局函数，或类的静态成员函数）。回调函数的
//	声明为: void xxxxx(void *param); 其中xxxxx为函数名，可以自己任意命名。
//  example:
//	void exThreadFunc(void* param)
//  {
//		int *value = (int*)(param);
//		cout << *value << endl;
//  }
//  int i = 3;
//	TThread myThread;
//	myThread.SetSetThreadFunc(exThreadFunc, &i);
//	myThread.Start();
//
//  2.基类使用时，重载虚函数Thread ().
//  example:
//  class myThreadClass : public TThread
//  {	
//		public:
//			virtual	void Thread ()	{   cout << 3 << endl; }
//  }
//
//	myThreadClass myThread;
//	myThread.Start();
//
//////////////////////////////////////////////////////////////////////////
//
// Simple thread control tool
// Last update Date	: 2006.08.31

#include "TThread.h"

#include <process.h>    // _beginthread(), _endthread()

TThread::TThread() : m_hThread(NULL), ThreadProc(0), m_param(0), m_ePriority(NORMAL)
{	}

TThread::~TThread ()
{ 
	if (m_hThread != NULL)  Stop();
}

bool TThread::Start (EnumPriority ePriority) 
{
	Lock lock(&m_lock);
	if(NULL == m_hThread)
	{
		unsigned threadID = 0;
		m_hThread = (HANDLE)_beginthreadex (0,
			0,
			StaticThread,
			this,
			CREATE_SUSPENDED,
			&threadID);
		if (m_hThread != NULL)
		{
			SetPriority(ePriority);
			ResumeThread (m_hThread);
		}
	}
    return (m_hThread != NULL);
}

bool TThread::Stop () 
{
	Lock lock(&m_lock);
	if(m_hThread != NULL)
	{
		TerminateThread (m_hThread, -1);
		::CloseHandle(m_hThread);
		m_hThread = NULL;
	}
    return true;
}
bool TThread::Suspend()
{
	return 0xFFFFFFFF != SuspendThread(m_hThread);
}
bool TThread::Resume()
{
	return 0xFFFFFFFF != ResumeThread(m_hThread);
}
bool TThread::SetPriority(EnumPriority ePriority)
{
	BOOL success = FALSE;
	if(NULL != m_hThread)
	{
		Suspend();
		success = SetThreadPriority(m_hThread, static_cast<int>(ePriority));
		Resume();
	}

	if(TRUE == success)
	{
		m_ePriority = ePriority;
		return true;
	}
	else
	{
		return false;
	}
}

TThread::EnumPriority TThread::GetPriority()
{
	return m_ePriority;
}
			
bool TThread::Wait (unsigned int ms)
{
	if(NULL == m_hThread) return true;  //no create thread or thread is over
    DWORD wait		= (ms == -1) ? INFINITE : ms;
    DWORD status	= WaitForSingleObject(m_hThread, wait);
    return (status != WAIT_TIMEOUT);
}

void TThread::Sleep (unsigned int ms)
{ 
	::Sleep(ms);
}

void TThread::SetThreadFunc(ThreadFunc* proc, void* param)
{
	ThreadProc	= proc;
	m_param		= param;	
}

unsigned int __stdcall TThread::StaticThread (void* obj)
{
    // Call the overriden thread function
    TThread* t = reinterpret_cast<TThread*>(obj);
    t->Thread ();
	
	// Close the thread handle
	{
		TThread::Lock lock( &(t->m_lock) );
		if(t->m_hThread != NULL)
		{
			::CloseHandle(t->m_hThread);
			t->m_hThread = NULL; 
		}
	}
    return 0;
}
void TThread::Thread ()
{
	if(ThreadProc)
	{
		ThreadProc(m_param);
	}
}

