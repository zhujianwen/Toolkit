#ifndef __SERVICE_H__
#define __SERVICE_H__


#include <Windows.h>


BOOL SvcIsInstalled();
BOOL SvcInstall();
BOOL SvcUnInstall();
void LogEvent();

void WINAPI SvcCtrlHandler(DWORD dwCtrl);
void WINAPI SvcMain(DWORD dwArgc, LPTSTR *lpszArgv);
void WINAPI ServiceContrl();
void WINAPI ServiceStart();
void WINAPI ServiceStop();
void WINAPI SvcUpdateDesc();

BOOL WINAPI StopDependentServices();

void ReportSvcStatus(DWORD dwCurrentState, DWORD dwWin32ExitCode, DWORD dwWaitHint);
void SvcInit(DWORD dwArgc, LPTSTR *lpszArgv);
void SvcReportEvent(LPTSTR szFunction);

void WINAPI service_main();
void WINAPI ServiceHandler(DWORD dwCtrl);


#endif	// __SERVICE_H__