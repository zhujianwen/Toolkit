// sockets.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "tcp.h"
#include <process.h>


#ifdef WIN32
    #ifndef __MINGW__
        #ifdef SOCKETS_EXPORTS
            #define ZHD_API __declspec(dllexport)
        #else
            #define ZHD_API __declspec(dllimport)
        #endif
    #else
        #define ZHD_API
    #endif
#else
    #define ZHD_API __attribute__ ((visibility("default")))
#endif

#define NO_BUSY_WAITING


typedef void (*Dispatch)(const int nEventID, const int nCmdID, const char* pszData, const int nLen);


extern "C" ZHD_API void Init(const char* pszHost, int nPort);
extern "C" ZHD_API void End();
extern "C" ZHD_API void Send(const int nEventID, const int nCmdID, const char* pszBuf, int nLen);
extern "C" ZHD_API void SetCallback(Dispatch pDispatch);


//////////////////////////////////////////////////////////////////////////
// 
tcp_socket* g_tcp = NULL;
Dispatch gOnDispatch = NULL;

unsigned int __stdcall WorkThread(LPVOID lpParam);


//////////////////////////////////////////////////////////////////////////
// DLL export methods
extern "C" ZHD_API void Init(const char* pszHost, int nPort)
{
	tcp_init();

	g_tcp = new tcp_socket();
	g_tcp->sockfd = -1;
	g_tcp->bConnect = false;
	g_tcp->port = nPort;
	snprintf(g_tcp->ip_address, "%s", pszHost);

	HANDLE hHandle = (HANDLE)_beginthreadex(NULL, 0, WorkThread, NULL, 0, NULL);
}

extern "C" ZHD_API void End()
{
	tcp_disconnect(g_tcp);
	tcp_end();
	delete g_tcp;
	g_tcp = NULL;
}

extern "C" ZHD_API void Send(const int nEventID, const int nCmdID, const char* pszBuf, int nLen)
{
	char buf[4096] = {0};
	memcpy(buf, pszBuf, nLen);
	tcp_write(g_tcp, buf, nLen);
}

extern "C" ZHD_API void SetCallback(Dispatch pDispatch)
{
	gOnDispatch = pDispatch;
}


//////////////////////////////////////////////////////////////////////////
// work thread
unsigned int __stdcall WorkThread(LPVOID lpParam)
{
	int nResult = 0;
	bool bConnect = false;
	char buf[4096];
	char data[1024];
	extern Dispatch gOnDispatch;

	while (1)
	{
		if (!bConnect)
		{
			if (!tcp_connect(g_tcp, g_tcp->ip_address, g_tcp->port))
			{
				memset(buf, 0, 4096);
				strcpy_s(buf, "connect to server failed!");
				gOnDispatch(5, 5, buf, strlen(buf));
				Sleep(1000);
				continue;
			}
			memset(buf, 0, 4096);
			strcat_s(buf, "GET  HTTP/1.0\r\n");
			strcat_s(buf, "User-Agent: NTRIP ZHDGPS-iRTK-RTK/1.0.0\r\n");
			strcat_s(buf, "Accept: */*\r\n");
			strcat_s(buf, "Connection: close\r\n");
			strcat_s(buf, "Authorization: Basic \r\n\r\n");
			//strcpy_s(buf, "connect to server success!");
			tcp_write(g_tcp, buf, strlen(buf));
			gOnDispatch(5, 5, buf, strlen(buf));
			bConnect = true;
		}

		memset(buf, 0, 4096);
		nResult = tcp_read(g_tcp, buf, 4096);
		if (nResult < 0)
		{
			memset(buf, 0, 4096);
			strcpy_s(buf, "read data failed, begin to connect!");
			gOnDispatch(5, 5, buf, strlen(buf));
			bConnect = false;
			Sleep(10000);
			continue;
		}
		//if (nResult > 28)
		//{
		//	unsigned short *uType = (unsigned short *)(&buf[21]);
		//	memset(data, 0, 1024);
		//	memcpy(data, &buf[25], nResult-25-3);
		//	gOnDispatch(5, *uType, data, nResult);
		//}
		gOnDispatch(5, 5, buf, nResult);
	}

	return nResult;
}