// TIniFile.cpp	: implement of the TIniFile class
//
// Copyright (c) 2006 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
// Simple .ini file operation tool
// Create Date	: 2006.10.18
// Upate  Date  : 2008.04.19, use TString Type.

// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

#include "TIniFile.h"

#include <stdlib.h>
#include <stdio.h>

TString	TIniFile::m_fileName;

int	TIniFile::GetInt(const TString &section, const TString &key, int defaultVal)
{
	TCHAR result[MAX_PATH];
	_stprintf_s(result, "%d", defaultVal);
	GetPrivateProfileString(section.c_str(),key.c_str(),
							result, result,	MAX_PATH,
							m_fileName.c_str() );
	return _ttoi(result);
}

void	TIniFile::SetInt(const TString &section, const TString &key, int value)
{
	TCHAR val[MAX_PATH];
	_stprintf_s( val, TEXT("%d"), value);
	WritePrivateProfileString(	section.c_str(),key.c_str(),
								val,	m_fileName.c_str());
}
float	TIniFile::GetFloat(const TString &section, const TString &key)
{
	TCHAR	result[MAX_PATH];
	TCHAR*	stopPos;
	GetPrivateProfileString(section.c_str(),key.c_str(),
							TEXT("0.0"), result,	MAX_PATH,
							m_fileName.c_str() );
	return static_cast<float>(_tcstod(result, &stopPos));
}

void	TIniFile::SetFloat(const TString &section, const TString &key, float value)
{
	TCHAR val[MAX_PATH];
	_stprintf_s( val, TEXT("%.6f"), value);
	WritePrivateProfileString(	section.c_str(),key.c_str(),
								val,	m_fileName.c_str());
}
TString TIniFile::GetStr(const TString &section, const TString &key)
{
	TCHAR result[MAX_PATH];
	GetPrivateProfileString(section.c_str(),key.c_str(),
							TEXT(""), result,		MAX_PATH,
							m_fileName.c_str() );
	return result;
}

void	TIniFile::SetStr(const TString &section, const TString &key, const TString &value)
{
	WritePrivateProfileString(  section.c_str(),key.c_str(),
								value.c_str(),m_fileName.c_str());
}
TString TIniFile::GetPathName(EnumPath ePath)
{
	TCHAR		path[MAX_PATH];
	TString	pathName;

	switch(ePath)
	{
	case eWinDir:
		GetWindowsDirectory(path,MAX_PATH);
		pathName = path;
		break;

	case eCurDir:
		GetCurrentDirectory(MAX_PATH,path);
		pathName = path;
		break;

	case eExeDir:
		{
			GetModuleFileName(NULL, path, MAX_PATH);
			pathName = path;
			TString::size_type  pos = pathName.find_last_of(TEXT("\\"));
			pathName = pathName.substr(0, pos);
		}
		break;

	case eOther:
	default:
		;
	}
	return pathName;
}
bool 	TIniFile::SetFileName(const TString &fileName,EnumPath ePath)
{
	TString pathName = GetPathName(ePath);

	if(pathName.empty())
	{	m_fileName = fileName;}
	else
	{	m_fileName = pathName + TEXT("\\") + fileName;	}

	// test if the file exist 
	WIN32_FIND_DATA FindFileData;
	HANDLE hHandle = FindFirstFile( m_fileName.c_str(), &FindFileData);
	if (hHandle == INVALID_HANDLE_VALUE)
	{	return false; 	}
	else
	{	FindClose(hHandle); return true; }
}