//TWinUtiliy.cpp
#ifdef _AFXDLL
#include <afx.h>
#else
#include <windows.h>
#endif

#include "TWinUtility.h"

TString TWinUtility::GetWindowDir()
{
	TCHAR winDir[MAX_PATH];
	GetWindowsDirectory(winDir,MAX_PATH);
	return TString(winDir);
}
TString TWinUtility::GetSystemDir ()
{
	TCHAR sysDir[MAX_PATH];
	GetSystemDirectory(sysDir,MAX_PATH);
	return TString(sysDir);
}

TString TWinUtility::GetCurrentDir()
{
	TCHAR curDir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH,curDir);
	return TString(curDir);
}
TString TWinUtility::GetExeFileDir()
{
	TCHAR path[MAX_PATH];
	GetModuleFileName(NULL, path, MAX_PATH);
	TString exeDir = path;
	TString::size_type  pos = exeDir.find_last_of(TEXT("\\"));
	return exeDir.substr(0, pos);
}

bool TWinUtility::StartupApp(const TString& appName)
{
	TCHAR szCommandLine[MAX_PATH];
	_tcscpy(szCommandLine, appName.c_str());
	PROCESS_INFORMATION processInfo;
	STARTUPINFO			startupInfo;
	
	memset(&startupInfo, 0, sizeof(STARTUPINFO));
	startupInfo.cb          = sizeof(STARTUPINFO);
	startupInfo.dwFlags     = STARTF_USESHOWWINDOW;
	startupInfo.wShowWindow = SW_SHOWDEFAULT;
	
	BOOL success = CreateProcess(
		NULL,
		szCommandLine,
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS, 
		NULL,
		NULL, 
		&startupInfo, 
		&processInfo);
	
	if(success)
	{
		CloseHandle(processInfo.hThread);
		CloseHandle(processInfo.hProcess);
	}
	return  TRUE == success;
}

bool TWinUtility::ShutDown()
{
	TWinUtility::HoldShutDownPrivilege();
	return (0 != ExitWindowsEx(EWX_SHUTDOWN, 0));
}
bool TWinUtility::Reboot()
{
	TWinUtility::HoldShutDownPrivilege();
	return (0 !=ExitWindowsEx(EWX_REBOOT, 0));
}

bool TWinUtility::LogOff()
{
	TWinUtility::HoldShutDownPrivilege();
	return (0 !=	ExitWindowsEx(EWX_LOGOFF, 0));
}

bool TWinUtility::HoldShutDownPrivilege()
{
	DWORD dwVersion = GetVersion();
	if(dwVersion < 0x80000000)
	{
		HANDLE				hToken;
		TOKEN_PRIVILEGES	tokenPrivilege;
		OpenProcessToken(GetCurrentProcess(),TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken);
		LookupPrivilegeValue(NULL,SE_SHUTDOWN_NAME,&tokenPrivilege.Privileges[0].Luid);
		tokenPrivilege.PrivilegeCount = 1;
		tokenPrivilege.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		AdjustTokenPrivileges(hToken, FALSE, &tokenPrivilege, 0, (PTOKEN_PRIVILEGES)NULL, 0);
	}
	return true;
}
