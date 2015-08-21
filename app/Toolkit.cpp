// Toolkit.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <atlbase.h>


#define BUFF_SIZE 4096
#define ZHD_API __declspec(dllimport)

#pragma comment(lib, "..\\bin\\sockets.lib")

HANDLE gStopEvent = INVALID_HANDLE_VALUE;

typedef void (*Dispatch)(const int nEventID, const int nCmdID, const char* pszData, const int nLen);

extern "C" ZHD_API void Init(const char* pszHost, int nPort);
extern "C" ZHD_API void End();
extern "C" ZHD_API void Send(const int nEventID, const int nCmdID, const char* pszBuf, int nLen);
extern "C" ZHD_API void SetCallback(Dispatch pDispatch);


void DoDispatchMsg(const int nEventID, const int nCmdID, const char* pszData, const int nLen)
{
	printf("%s, %d\n", pszData, nLen);
	Send(nEventID, nCmdID, pszData, nLen);
}


int _tmain(int argc, _TCHAR* argv[])
{
	int nPort = 0;
	LPTSTR pszHost = NULL;

	if (argc < 3)
	{
		nPort = 6620;
		pszHost = "172.16.21.58";
	}
	else
	{
		pszHost = argv[1];
		nPort = atoi(argv[2]);
	}

	Init(pszHost, nPort);
	SetCallback(DoDispatchMsg);

	// create an thread
	//HANDLE hHandle = (HANDLE)_beginthreadex(NULL, 0, WorkThread, (LPVOID)tcp, 0, NULL);

	// create an event
	gStopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	if (gStopEvent == INVALID_HANDLE_VALUE)
	{
		printf("CreateEvent failed!\n");
		return 1;
	}
	while (true)
	{
		WaitForSingleObject(gStopEvent, INFINITE);
		break;
	}

	return 0;
}

