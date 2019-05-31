#include <iostream>	
#include <winsock.h>	

using namespace std;
#pragma comment (lib,"Ws2_32.lib")

#define DEFAULT_PORT 6000


void main() {
	WORD wVersionRequested;
	wVersionRequested = MAKEWORD(2, 2);
	WSAData wsaData;

	if (0 != WSAStartup(wVersionRequested, &wsaData)) {
		cout << "Init Failure!" << endl;
	}

	if (HIBYTE(wsaData.wVersion) != 2 && LOBYTE(wsaData.wVersion) != 2) {
		cout << "Version Error!" << endl;
	}

	SOCKET sockClient = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (SOCKET_ERROR == sockClient) {
		cout << "Init Socket Error!" << endl;
	}

	SOCKADDR_IN addrClient;
	/*addrClient.sin_addr.S_un.S_addr = inet_addr( "127.0.0.1");*/
	addrClient.sin_addr.S_un.S_addr = inet_addr("139.196.121.176");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(DEFAULT_PORT);

	int err = sendto(sockClient, "Hello UdpSrv,From Client!", sizeof("Hello UdpSrv,From Client!") + 1, 0, (SOCKADDR*)&addrClient, sizeof(addrClient));
	if (SOCKET_ERROR == err) {
		cout << "Send data to service Error!" << endl;
	}

	/*SOCKADDR_IN addrRecv;
	int len = sizeof(SOCKADDR);
	char lpBuff[100];
	recvfrom(sockClient, lpBuff, 100, 0, (SOCKADDR*)&addrRecv, &len);
	cout << inet_ntoa(addrRecv.sin_addr) << ":" << lpBuff << "" << endl;*/

	closesocket(sockClient);
	WSACleanup();
	system("pause");
}