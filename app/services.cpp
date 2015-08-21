// services.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "svc.h"

HANDLE ghExitEvent = NULL;


void DisplayUsage()
{
	printf("Description:\n");
	printf("\tCommand-line tool that controls a service.\n\n");
	printf("Usage:\n");
	printf("\tsvccontrol [command] [service_name]\n\n");
	printf("\t[command]\n");
	printf("\t  start\n");
	printf("\t  dacl\n");
	printf("\t  stop\n");
}


int _tmain(int argc, _TCHAR* argv[])
{
	char buf[64] = {0};
	//ServiceStart();
	//return 0;

	while(gets_s(buf))
	{
		if (strncmp("a", buf, 1) == 0)
		{
			SvcInstall();
			//SvcUpdateDesc();
			printf("Service install\n");
		}
		else if (strncmp("u", buf, 1) == 0)
		{
			SvcUnInstall();
			printf("Service UnInstall\n");
		}
		else if (strncmp("e", buf, 1) == 0)
		{
			ServiceStart();
			printf("Service Start\n");
		}
		else if (strncmp("s", buf, 1) == 0)
		{
			ServiceStop();
			printf("Service Stop\n");
		}
		else if (strncmp("q", buf, 1) == 0)
		{
			break;
		}
	}

	printf("Service exit.....\n");

	return 0;
}

