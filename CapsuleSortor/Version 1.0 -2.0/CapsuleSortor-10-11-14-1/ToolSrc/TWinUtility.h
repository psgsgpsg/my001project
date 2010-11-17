//TWinUtiliy.h

#include "TString.h"

class TWinUtility
{
public:
	static TString GetWindowDir	();
	static TString GetSystemDir ();
	static TString GetCurrentDir();
	static TString GetExeFileDir();
	static bool    StartupApp	(const TString& appName);
	static bool	   ShutDown();
	static bool	   Reboot();
	static bool	   LogOff();

private:
	static bool	   HoldShutDownPrivilege();
};