// TLogManager.cpp: implementation of the TLogManager class.
//
//////////////////////////////////////////////////////////////////////

#include "TLogManager.h"
#include <fstream>
#include <sstream>
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TLogManager::TLogManager()
{
	m_timeCount.Reset();
}

TLogManager::~TLogManager()
{

}

void	TLogManager::AddInfo(const char* info)
{
	std::string	infoTxt = info;
	double time = m_timeCount.msec();
	m_dataSet.push_back(std::make_pair(time, infoTxt));
}

void	TLogManager::Save()
{
	const std::string folderPath = "C:\\Documents and Settings\\Administrator\\My Documents\\";
	SYSTEMTIME	time;
	GetLocalTime(&time);
	std::ostringstream	os;
	os<<time.wYear<<"-"<<\
		time.wMonth<<\
		"-"<<time.wDay<<\
		"-"<<time.wHour<<\
		"-"<<time.wMinute<<\
		"-"<<time.wSecond;
	std::string filePath = folderPath +os.str().c_str()+".log";
	HANDLE	hFile = CreateFile(filePath.c_str(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, 0);
	std::ofstream ofs(filePath.c_str());
	std::list<Node>::iterator	it = m_dataSet.begin();
	for(size_t i = 0; i < m_dataSet.size(); i++)
	{
		ofs<<it->first<<"          ";
		ofs<<it->second<<std::endl;
		it++;
 	}
	ofs.close();
}

std::string TLogManager::Time2String(SYSTEMTIME &time)
{
	std::ostringstream os;
	os<<time.wYear<<"-"\
		<<time.wMonth<<"-"\
		<<time.wDay<<"-"\
		<<time.wHour<<"-"\
		<<time.wMinute<<"-"\
		<<time.wSecond<<"-"\
		<<time.wMilliseconds;
	return os.str();
}
