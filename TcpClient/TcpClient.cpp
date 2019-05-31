#include <iostream>
#include <winsock.h>
using namespace std;

#define DEFAULT_PORT 6000
#pragma comment (lib,"Ws2_32.lib")
void main() {
	WORD wVersionRequested;
	WSAData wsaData;
	wVersionRequested = MAKEWORD(2, 2);

	int err = WSAStartup(wVersionRequested, &wsaData);
	if (0 != err) {
		cout << "Init Failure!" << err << endl;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		cout << "Version Error~!" << endl;
	}

	SOCKET sockClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (INVALID_SOCKET == sockClient) {
		cout << "Init Socket Error~!" << endl;
	}


	SOCKADDR_IN sockAddr;
	sockAddr.sin_addr.S_un.S_addr = inet_addr( "127.0.0.1");
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(DEFAULT_PORT);

	if (SOCKET_ERROR == connect(sockClient, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR))) {
		cout << "Connect Failure!" << endl;
	}

	char rcvBuff[100];
	recv(sockClient, rcvBuff, 100, 0);
	cout << "接收到的消息："<< rcvBuff << endl;

	send(sockClient, "Hello 服务端,from client!", sizeof("Hello service,from client!") + 1, 0);

	closesocket(sockClient);

	WSACleanup();
	system("pause");
}