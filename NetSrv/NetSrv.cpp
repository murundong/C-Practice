#include <stdio.h>	
#include <iostream>	
#include <winsock.h>	
using namespace std;
#define DEFAULT_PORT 6000
#pragma comment (lib,"Ws2_32.lib")
void main() {

	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);
	WSAData wsaData;
	if (0 != WSAStartup(wVersionRequested, &wsaData)) {
		cout << "Init Failure!" << endl;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2) {
		cout << "Version Error!" << endl;
	}

	/*SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);*/
	SOCKET sockSrv = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN addr_in;
	addr_in.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(DEFAULT_PORT);

	if (SOCKET_ERROR == bind(sockSrv, (SOCKADDR*)&addr_in, sizeof(SOCKADDR))) {
		cout << "Bind Error!" << endl;
	}

	char sendBuff[100];
	char rcvBuff[100];
	char tempBuff[200];

	SOCKADDR_IN addr_from;
	int len = sizeof(SOCKADDR);

	while (1)
	{
		recvfrom(sockSrv, rcvBuff, 100, 0, (SOCKADDR*)&addr_from, &len);
		if ('q' == rcvBuff[0]) {
			sendto(sockSrv, "q", sizeof("q") + 1, 0, (SOCKADDR*)&addr_from, len);
			cout << "Chat End! \n" << endl;
			break;
		}
		sprintf_s(tempBuff, "[%s]:%s", inet_ntoa(addr_from.sin_addr),rcvBuff);
		cout << tempBuff << endl;
		cout << "Please input:" ;
		cin >> sendBuff;
		sendto(sockSrv,sendBuff,sizeof(sendBuff)+1,0, (SOCKADDR*)&addr_from, len);
	}

	closesocket(sockSrv);
	WSACleanup();

	system("pause");
}