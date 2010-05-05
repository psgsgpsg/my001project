; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVideoAnalysisDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VideoAnalysis.h"

ClassCount=3
Class1=CVideoAnalysisApp
Class2=CVideoAnalysisDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_VIDEOANALYSIS_DIALOG

[CLS:CVideoAnalysisApp]
Type=0
HeaderFile=VideoAnalysis.h
ImplementationFile=VideoAnalysis.cpp
Filter=N

[CLS:CVideoAnalysisDlg]
Type=0
HeaderFile=VideoAnalysisDlg.h
ImplementationFile=VideoAnalysisDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_PRGC_SNR

[CLS:CAboutDlg]
Type=0
HeaderFile=VideoAnalysisDlg.h
ImplementationFile=VideoAnalysisDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VIDEOANALYSIS_DIALOG]
Type=1
Class=CVideoAnalysisDlg
ControlCount=16
Control1=IDC_STATIC,static,1342177287
Control2=IDC_STATIC,button,1342177287
Control3=IDC_STATIC,static,1342308865
Control4=IDC_STATIC,static,1342308865
Control5=IDC_STATIC,static,1342308865
Control6=IDC_STATIC,static,1342308865
Control7=IDCANCEL,button,1342242816
Control8=IDC_PRGC_SNR,msctls_progress32,1342177281
Control9=IDC_PRGC_LGHT,msctls_progress32,1342177281
Control10=IDC_PRGC_GAIN,msctls_progress32,1342177281
Control11=IDC_PRGC_GRAY,msctls_progress32,1342177281
Control12=IDC_EDT_SNR,edit,1484857473
Control13=IDC_EDT_LGHT,edit,1484857473
Control14=IDC_EDT_GAIN,edit,1484857473
Control15=IDC_EDT_GRAY,edit,1484857473
Control16=IDC_LOADIMAGE,button,1342242816

