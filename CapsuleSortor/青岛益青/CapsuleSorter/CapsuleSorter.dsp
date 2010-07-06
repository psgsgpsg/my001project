# Microsoft Developer Studio Project File - Name="CapsuleSorter" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=CapsuleSorter - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CapsuleSorter.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CapsuleSorter.mak" CFG="CapsuleSorter - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CapsuleSorter - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "CapsuleSorter - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CapsuleSorter - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /I "../ToolCVB" /I "../ToolCVB/Include" /I "../Valve" /I "../Remain" /I "../CapsuleSorter" /I "../ImageCard" /I "../ToolSrc" /I "../ImageCard/SVSGigE\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "COLOR_ORDER_RGB" /D "NEW_REMAIN" /Fr /YX /FD /c
# SUBTRACT CPP /Z<none>
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 ../ImageCard/Genie/lib/*.lib ../ToolCVB/Lib/*.Lib ../ImageCard/Procili/lib/*.lib /nologo /subsystem:windows /machine:I386 /out:"../Bin/CapsuleSorter.exe"
# SUBTRACT LINK32 /incremental:yes /debug

!ELSEIF  "$(CFG)" == "CapsuleSorter - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\Valve" /I "../ToolCVB" /I "../ToolCVB/Include" /I "../ImageCard" /I "../ImageCard/Procili/include" /I "../ImageCard/Genie/include" /I "../ToolSrc" /I "../Valve" /I "../Remain" /I "../CapsuleSorter" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /FD /GZ /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ../ImageCard/Genie/lib/*.lib ../ImageCard/Procili/lib/*.lib winmm.lib ../ToolCVB/Lib/*.Lib /nologo /subsystem:windows /debug /machine:I386 /out:"../Bin/CapsuleSorter.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "CapsuleSorter - Win32 Release"
# Name "CapsuleSorter - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\Remain\ArcTemple.cpp
# End Source File
# Begin Source File

SOURCE=.\CamConfigSetting.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\CapsuleProc.cpp
# End Source File
# Begin Source File

SOURCE=.\CapsuleSorter.cpp
# End Source File
# Begin Source File

SOURCE=.\CapsuleSorter.rc
# End Source File
# Begin Source File

SOURCE=.\CapsuleSorterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ColorButton.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\ColorProcess.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\ConfigInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlBoardSetting.cpp
# End Source File
# Begin Source File

SOURCE=..\Valve\CtrlCardComm.cpp
# End Source File
# Begin Source File

SOURCE=.\cvdisplay.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\HorizonExtend.cpp
# End Source File
# Begin Source File

SOURCE=.\KeyBoardDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\MonoProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\PathSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\SelectHDCfg.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageCard\SimularCard.cpp
# End Source File
# Begin Source File

SOURCE=.\SoftWareConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\SortObserver.cpp
# End Source File
# Begin Source File

SOURCE=.\SpecifyCapsualeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=..\ImageCard\SVSGigE\include\SVGigE.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageCard\SVSGigECard.cpp
# End Source File
# Begin Source File

SOURCE=..\Valve\TBeacon.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TBmpBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TBmpInfo.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TColorChecker.cpp
# End Source File
# Begin Source File

SOURCE=..\ImageCard\TImageCard.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgArchive.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgBuffer.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgDim.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgPainter.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgPlayer.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgProcess.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TIniFile.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TLogManager.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TMfdArchive.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TPaintCoord.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TPoint2D.cpp
# End Source File
# Begin Source File

SOURCE=.\Translation.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TRect2D.cpp
# End Source File
# Begin Source File

SOURCE=..\Remain\TRemain.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TSeriesComm.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TThread.cpp
# End Source File
# Begin Source File

SOURCE=..\Remain\TTransRemain.cpp
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TWinUtility.cpp
# End Source File
# Begin Source File

SOURCE=..\Valve\Valve.cpp
# End Source File
# Begin Source File

SOURCE=.\VertifyDlg.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\Remain\ArcTemple.h
# End Source File
# Begin Source File

SOURCE=.\CamConfigSetting.h
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\CapsuleProc.h
# End Source File
# Begin Source File

SOURCE=.\CapsuleSorter.h
# End Source File
# Begin Source File

SOURCE=.\CapsuleSorterDlg.h
# End Source File
# Begin Source File

SOURCE=.\ColorButton.h
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\ColorProcess.h
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\ConfigInfo.h
# End Source File
# Begin Source File

SOURCE=.\ControlBoardSetting.h
# End Source File
# Begin Source File

SOURCE=..\Valve\CtrlCardComm.h
# End Source File
# Begin Source File

SOURCE=.\cvdisplay.h
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\HorizonExtend.h
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\include\iCVCImg.h
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\include\iCVFoundation.h
# End Source File
# Begin Source File

SOURCE=.\KeyBoardDlg.h
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\MonoProcess.h
# End Source File
# Begin Source File

SOURCE=.\PathSelect.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\PixelType.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SelectHDCfg.h
# End Source File
# Begin Source File

SOURCE=..\ImageCard\SimularCard.h
# End Source File
# Begin Source File

SOURCE=.\SoftWareConfig.h
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\SortObserver.h
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\SortResult.h
# End Source File
# Begin Source File

SOURCE=.\SpecifyCapsualeDlg.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\ImageCard\SVSGigE\include\SVGigE.h
# End Source File
# Begin Source File

SOURCE=..\ImageCard\SVSGigECard.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TAllocTmpl.h
# End Source File
# Begin Source File

SOURCE=..\Valve\TBeacon.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TBmpBoard.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TBmpInfo.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TColorChecker.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TCritSect.h
# End Source File
# Begin Source File

SOURCE=..\ImageCard\TImageCard.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImageTmpl.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgArchive.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgBuffer.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgDim.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgPainter.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgPlayer.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TImgProcess.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TIniFile.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TLogManager.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TMfdArchive.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TPaintCoord.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TPoint2D.h
# End Source File
# Begin Source File

SOURCE=.\Translation.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TRect2D.h
# End Source File
# Begin Source File

SOURCE=..\Remain\TRemain.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TSeriesComm.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TSIMDChecker.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TThread.h
# End Source File
# Begin Source File

SOURCE=..\Remain\TTransRemain.h
# End Source File
# Begin Source File

SOURCE=..\ToolSrc\TWinUtility.h
# End Source File
# Begin Source File

SOURCE=..\ToolCVB\TypeDefine.h
# End Source File
# Begin Source File

SOURCE=..\Valve\Valve.h
# End Source File
# Begin Source File

SOURCE=.\VertifyDlg.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BALL8A~1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\BALL8B~1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\BALL8C~1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\BALL8D~1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\BALL8E~1.ICO
# End Source File
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\capsule0b.ico
# End Source File
# Begin Source File

SOURCE=.\res\CapsuleSorter.ico
# End Source File
# Begin Source File

SOURCE=.\res\CapsuleSorter.rc2
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon5.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon6.ico
# End Source File
# Begin Source File

SOURCE=.\res\light1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\yellow1.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
# Section CapsuleSorter : {648A5600-2C6E-101B-82B6-000000000014}
# 	2:21:DefaultSinkHeaderFile:mscomm.h
# 	2:16:DefaultSinkClass:CMSComm
# End Section
# Section CapsuleSorter : {289A0004-32DD-11D3-98F3-0060082C35E6}
# 	2:5:Class:CCVdisplay
# 	2:10:HeaderFile:cvdisplay.h
# 	2:8:ImplFile:cvdisplay.cpp
# End Section
# Section CapsuleSorter : {E6E17E90-DF38-11CF-8E74-00A0C90F26F8}
# 	2:5:Class:CMSComm
# 	2:10:HeaderFile:mscomm.h
# 	2:8:ImplFile:mscomm.cpp
# End Section
# Section CapsuleSorter : {289A0006-32DD-11D3-98F3-0060082C35E6}
# 	2:21:DefaultSinkHeaderFile:cvdisplay.h
# 	2:16:DefaultSinkClass:CCVdisplay
# End Section
