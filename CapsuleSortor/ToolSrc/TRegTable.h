// TRegTable.h	: Interface of the TRegTable class
//
// Copyright (c) 2008 zhao_fsh  Zibo Creative Computor CO.,LTD
//////////////////////////////////////////////////////////////////////////
// Simple Register Table operation tool
// Create Date	: 2008.04.19
// Upate  Date  : 2008.04.19

// We want this to be compatible with MFC and non-MFC apps
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

#include <winreg.h>
#include "TString.h"

class TRegTable
{
public:
	TRegTable();
	~TRegTable();

public:
	bool OpenKey	(HKEY hKey, const TString& subkey);
	bool CreateKey	(HKEY hKey, const TString& subkey);
	bool CloseKey	();
	bool SetValue	(const TString& valName, DWORD data);
	bool SetValue	(const TString& valName, const TString& data);
	DWORD	GetDWORD(const TString& valName);
	TString GetStr	(const TString& valName);

private:
	HKEY	m_hKey;
	bool	m_isOpen;
};