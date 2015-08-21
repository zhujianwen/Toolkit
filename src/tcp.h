#ifndef __ZHD_TCP_H__
#define __ZHD_TCP_H__

#ifdef _WIN32
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#endif


#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

#ifdef _WIN32
#define boolean BOOLEAN
#else
typedef int boolean;
#endif

#ifndef true
#define true 1
#endif

#ifndef false
#define false 0
#endif


#ifdef _WIN32
#define snprintf sprintf_s
#endif


typedef struct tcp_socket rdpTcp;

struct tcp_socket
{
	int sockfd;
	int port;
	char ip_address[32];
	boolean bConnect;
#ifdef _WIN32
	WSAEVENT wsa_event;
#endif
};


void tcp_init();
void tcp_end();
boolean tcp_connect(rdpTcp* tcp, const char* hostname, unsigned short port);
boolean tcp_disconnect(rdpTcp* tcp);
int tcp_read(rdpTcp* tcp, char* data, int length);
int tcp_write(rdpTcp* tcp, char* data, int length);
boolean tcp_set_blocking_mode(rdpTcp* tcp, boolean blocking);
boolean tcp_set_keep_alive_mode(rdpTcp* tcp);

rdpTcp* tcp_new();
void tcp_free(rdpTcp* tcp);


#endif	// __ZHD_TCP_H__