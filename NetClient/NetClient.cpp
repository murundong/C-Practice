#include<stdio.h>	
#include<iostream>
#include<winsock.h>
using namespace std;
#define DEFAULT_PORT 6000
#pragma comment (lib,"Ws2_32.lib")

void main() {
	WORD wVersionRequtested;
	wVersionRequtested = MAKEWORD(2, 2);
	WSADATA wsaData;

	if (0 != WSAStartup(wVersionRequtested, &wsaData)) {
		cout << "Init Failure!" << endl;
	}
	if (HIBYTE(wsaData.wVersion) != 2 || LOBYTE(wsaData.wVersion) != 2) {
		cout << "Version Error!" << endl;
	}

	
	
	//SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN addr_client;
	/*addr_client.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");*/
	addr_client.sin_addr.S_un.S_addr = inet_addr("192.168.8.137");
	/*addr_client.sin_addr.S_un.S_addr = inet_addr("139.196.121.176");*/
	addr_client.sin_family = AF_INET;
	addr_client.sin_port = htons(DEFAULT_PORT);

	int len = sizeof(SOCKADDR);
	char sendBuff[100];
	char rcvBuff[100];
	char tempBuff[100];

	while (1)
	{
		cout << "Please input:";
		cin >> sendBuff;
		sendto(sockClient, sendBuff, sizeof(sendBuff) + 1, 0, (SOCKADDR*)&addr_client, len);
		recvfrom(sockClient, rcvBuff, 100, 0, (SOCKADDR*)&addr_client, &len);
		if ('q' == rcvBuff[0]) {
			sendto(sockClient, "q", sizeof("q") + 1, 0, (SOCKADDR*)&addr_client, len);
			cout << "Chat End! \n" << endl;
			break;
		}
		sprintf_s(tempBuff, "[%s]:%s", inet_ntoa(addr_client.sin_addr), rcvBuff);
		cout << tempBuff << endl;
	}
	closesocket(sockClient);

	WSACleanup();
	system("pause");
}