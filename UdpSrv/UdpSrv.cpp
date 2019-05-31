#include <iostream>
#include<winsock.h>	
using namespace std;
#pragma comment (lib,"Ws2_32.lib")

#define DEFAULT_PORT 6000

void main() {
	WORD wVersionRequired;
	WSADATA wsaData;
	wVersionRequired= MAKEWORD(2, 2);
	int err = WSAStartup(wVersionRequired, &wsaData);
	if (0 != err) {
		cout << "Init Failure!" << endl;
	}

	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		cout << "Version Error~!" << endl;
	}

	SOCKET sockServ = socket(AF_INET, SOCK_DGRAM, 0);
	if (SOCKET_ERROR == sockServ) {
		cout << "Init Socket Error!" << endl;
	}


	SOCKADDR_IN addrServ;
	addrServ.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(DEFAULT_PORT);

	if (SOCKET_ERROR == bind(sockServ, (SOCKADDR*)&addrServ, sizeof(SOCKADDR)))
	{
		cout << "Bind Socket Error~" << endl;
	}


	/*while (1)
	{
		SOCKADDR_IN addrRecv;
		int len = sizeof(SOCKADDR);
		char lpBuff[100];
		recvfrom(sockServ, lpBuff, 100, 0,(SOCKADDR*)&addrRecv,&len);
		cout << inet_ntoa(addrRecv.sin_addr) << ":" << lpBuff << "" << endl;

		if (SOCKET_ERROR == sendto(sockServ, "Hello Client!", sizeof("Hello Client!") + 1, 0, (SOCKADDR*)&addrRecv, sizeof(SOCKADDR))) {
			cout << "Send Data Error!" << endl;
		}
	}*/
	SOCKADDR_IN addrRecv;
	int len = sizeof(SOCKADDR);
	char lpBuff[100];
	recvfrom(sockServ, lpBuff, 100, 0, (SOCKADDR*)&addrRecv, &len);
	cout << inet_ntoa(addrRecv.sin_addr) << ":" << lpBuff << "" << endl;

	closesocket(sockServ);
	system("pause");
	WSACleanup();
}