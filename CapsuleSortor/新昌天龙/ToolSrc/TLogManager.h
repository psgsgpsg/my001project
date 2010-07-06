#ifndef TLOGMANAGER_H
#define TLOGMANAGER_H

#pragma warning(disable: 4786)

#pragma once

#ifdef _AFXDLL
#include "afx.h"
#else
#include "windows.h"
#endif
#include "TTimeDiff.h"
#include <list>
#include <utility>
#include <string>

typedef	std::pair<double, std::string > Node;

class TLogManager  
{
public:
	TLogManager();
	~TLogManager();

	void	AddInfo(const char* info);
	void	Save();
private:
	std::string Time2String(SYSTEMTIME &time);
private:
	std::list<Node>		m_dataSet;
	TTimeDiff			m_timeCount;
};

#endif 
