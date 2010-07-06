// TIniFile.h	: Interface of the TIniFile class
//
// Copyright (c) 2006 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
// Simple *.ini file operation tool
// Create Date	: 2006.10.18
// Upate  Date  : 2008.04.19, use TString Type.

#ifndef TINIFILE_H
#define TINIFILE_H

#include "TString.h"

class TIniFile
{
public:
	enum EnumPath{ eWinDir,  eCurDir,	eExeDir, eOther};

public:
	// Set the .ini file name, return true if the file exist, else return false;
	static bool		SetFileName	(const TString &fileName,EnumPath ePath);
	static TString	GetPathName	(EnumPath ePath);
	// Set and Get the int data
	static int		GetInt(const TString &section, const TString &key, int defaultVal = 0);
	static void		SetInt(const TString &section, const TString &key, int value);

	// Set and Get the float data
	static float	GetFloat(const TString &section, const TString &key);
	static void		SetFloat(const TString &section, const TString &key, float value);

	// Set and Get the string data
	static TString	GetStr(const TString &section, const TString &key);
	static void		SetStr(const TString &section, const TString &key, const TString &value);

private:
	TIniFile();
	static TString	m_fileName;
};
#endif  //TINIFILE_H

