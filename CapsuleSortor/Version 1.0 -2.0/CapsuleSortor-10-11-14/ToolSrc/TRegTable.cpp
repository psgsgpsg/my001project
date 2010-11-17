#include "TRegTable.h"

TRegTable::TRegTable() : m_hKey(NULL), m_isOpen(false)
{	}

TRegTable::~TRegTable()
{	}

bool TRegTable::OpenKey(HKEY hKey, const TString& subkey)
{
	CloseKey();
	LONG result = RegOpenKeyEx(hKey, subkey.c_str(), 0, KEY_ALL_ACCESS, &m_hKey);
	return m_isOpen = (ERROR_SUCCESS == result);
}

bool TRegTable::CreateKey(HKEY hKey, const TString& subkey)
{
	CloseKey();
	DWORD	newKey = REG_CREATED_NEW_KEY;	
	LONG	result = RegCreateKeyEx(
		hKey,					// handle to an open key
		subkey.c_str(),			// address of subkey name
		0,						// reserved
		NULL,					// address of class string
		REG_OPTION_VOLATILE,	// special options flag
		KEY_ALL_ACCESS,			// desired security access
		NULL,					// address of key security structure
		&m_hKey,				// address of buffer for opened handle
		&newKey					// address of disposition value buffer
		);
	return m_isOpen = (ERROR_SUCCESS == result);
}

bool TRegTable::CloseKey()
{
	if(m_isOpen)
	{
		RegCloseKey(m_hKey);
		m_isOpen = false;
	}
	return true;
}

bool TRegTable::SetValue(const TString& valName, DWORD data)
{
	if(m_isOpen)
	{
		LONG  result = RegSetValueEx(
			m_hKey,					// handle to key to set value for
			valName.c_str(),		// name of the value to set
			0,						// reserved
			REG_DWORD,				// flag for value type
			(CONST BYTE *) &data,	// address of value data
			4						// size of value data
			);
		return (ERROR_SUCCESS == result);
	}
	return false;
}

bool TRegTable::SetValue(const TString& valName, const TString& data)
{
	DWORD count = (data.length() + 1) * sizeof(TCHAR);
	if(m_isOpen)
	{
		LONG  result = RegSetValueEx(
			m_hKey,						// handle to key to set value for
			valName.c_str(),			// name of the value to set
			0,							// reserved
			REG_SZ,						// flag for value type
			(CONST BYTE *)data.c_str(),	// address of value data
			count						// size of value data
			);
		return (ERROR_SUCCESS == result);
	}
	return false;
}

DWORD TRegTable::GetDWORD(const TString& valName)
{
	DWORD data = -1;
	if(m_isOpen)
	{
		DWORD buffer	= -1;
		DWORD type		= REG_DWORD;
		DWORD bufSize	= 4;
		if( ERROR_SUCCESS == RegQueryValueEx(
									m_hKey,				// handle to key to query
									valName.c_str(),	// address of name of value to query
									0,					// reserved
									&type,				// address of buffer for value type
									(LPBYTE)&buffer,	// address of data buffer
									&bufSize)			// address of data buffer size
		  )
		{	data = buffer;	}
	}
	return data;
}

TString TRegTable::GetStr(const TString& valName)
{
	TString data;
	const  int charCount = 64;
	static TCHAR buffer[charCount];
	if(m_isOpen)
	{
		DWORD type		= REG_SZ;
		DWORD bufSize	= charCount * sizeof(TCHAR);
		if( ERROR_SUCCESS == RegQueryValueEx(
										m_hKey,				// handle to key to query
										valName.c_str(),	// address of name of value to query
										0,					// reserved
										&type,				// address of buffer for value type
										(LPBYTE)buffer,		// address of data buffer
										&bufSize)			// address of data buffer size
		  )
		{	data = buffer;	}
	}
	return data;
}
