#include "TLog.h"

TLog* TLog::m_pInstance = 0;

TLog::TLog () : m_log(0) {}

TLog& TLog::Instance ()
{
	if (!m_pInstance)
		m_pInstance = new TLog ();
	return *m_pInstance;
}

bool TLog::IsLog	(int mode)
{
	std::bitset<N> test = mode;
	test &= m_log;
	return test.any();
}

void TLog::Set (int mode)
{ 
	m_log = mode;
}

void TLog::Clear (int mode)
{
	std::bitset<N> clear = mode;
	m_log &= clear.flip();
}
