//#include <stdio.h>
//#include <windows.h>
//#include <tlhelp32.h>
//#include <Iphlpapi.h>
//#include<time.h>
//#pragma comment(lib, "ws2_32.lib")
//#pragma comment(lib,"Iphlpapi.lib")
//#define HOSTNAMELEN 256
//#define PORTNAMELEN 256
//#define ADDRESSLEN HOSTNAMELEN+PORTNAMELEN
//
//#define FLAG_ALL_ENDPOINTS 1
//#define FLAG_SHOW_NUMBERS 2
//
//typedef struct {
//	DWORD dwNumEntries;
//	MIB_TCPROW_OWNER_MODULE table[100];
//} D_MIB_TCPTABLE_OWNER_MODULE, *D_PMIB_TCPTABLE_OWNER_MODULE;
//
//typedef struct {
//	DWORD dwNumEntries;
//	MIB_UDPROW_OWNER_MODULE table[100];
//} D_MIB_UDPTABLE_OWNER_MODULE, *D_PMIB_UDPTABLE_OWNER_MODULE;
//
//
//static char TcpState[][32] = {
//	"???",
//	"CLOSED",
//	"LISTENING",
//	"SYN_SENT",
//	"SYN_RCVD",
//	"ESTABLISHED",
//	"FIN_WAIT1",
//	"FIN_WAIT2",
//	"CLOSE_WAIT",
//	"CLOSING",
//	"LAST_ACK",
//	"TIME_WAIT",
//	"DELETE_TCB"
//};
//
//PCHAR GetPort(unsigned int port, char* pPort)
//{
//	wsprintf(pPort, "%d", htons((WORD)port));
//	return pPort;
//}
//
//PCHAR GetIp(unsigned int ipaddr, char* pIP)
//{
//	in_addr nipaddr;
//	nipaddr.S_un.S_addr = ipaddr;
//	strcpy(pIP, inet_ntoa(nipaddr));
//	return pIP;
//}
//
//PCHAR ProcessPidToName(HANDLE hProcessSnap, DWORD ProcessId, PCHAR ProcessName)
//{
//	PROCESSENTRY32 processEntry;
//	strcpy(ProcessName, "..");
//	if (!Process32First(hProcessSnap, &processEntry)) {
//		return ProcessName;
//	}
//	do {
//		if (processEntry.th32ProcessID == ProcessId)
//		{
//			strcpy(ProcessName, processEntry.szExeFile);
//			return ProcessName;
//		}
//	} while (Process32Next(hProcessSnap, &processEntry));
//	return ProcessName;
//}
//
//
//int main(int argc, char* argv[])
//{
//	int nRetCode;
//	DWORD i;
//	WSADATA WSAData;
//	HANDLE hProcessSnap;
//	D_MIB_TCPTABLE_OWNER_MODULE TCPExTable;
//	D_MIB_UDPTABLE_OWNER_MODULE UDPExTable;
//	DWORD UdpCount = sizeof(D_MIB_TCPTABLE_OWNER_MODULE);
//	DWORD TcpCount = sizeof(D_MIB_UDPTABLE_OWNER_MODULE);
//	char szProcessName[MAX_PATH];
//	char szLocalName[HOSTNAMELEN], szRemoteName[HOSTNAMELEN];
//	char szRemotePort[PORTNAMELEN], szLocalPort[PORTNAMELEN];
//	char szLocalAddress[ADDRESSLEN], szRemoteAddress[ADDRESSLEN];
//	struct tm *Time;
//
//
//	if (WSAStartup(MAKEWORD(1, 1), &WSAData))
//	{
//		printf("WSAStartup error!\n");
//		return 0;
//	}
//
//	nRetCode = GetExtendedTcpTable(&TCPExTable, &TcpCount, TRUE, AF_INET, TCP_TABLE_OWNER_MODULE_ALL, 0);
//	if (nRetCode)
//	{
//		printf("GetExtendedTcpTable!\n");
//		return 0;
//	}
//	nRetCode = GetExtendedUdpTable(&UDPExTable, &UdpCount, TRUE, AF_INET, UDP_TABLE_OWNER_MODULE, 0);
//	if (nRetCode)
//	{
//		printf("GetExtendedUdpTable.\n");
//		return -1;
//	}
//	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
//	if (hProcessSnap == INVALID_HANDLE_VALUE)
//	{
//		printf("CreateToolhelp32Snapshot Error!\n");
//	}
//
//	printf("%-6s%-20s%-20s%-11s%s\n", "协议", "本地地址", "外部地址", "连接状态", "进程");
//	for (i = 0; i < TCPExTable.dwNumEntries; i++)
//	{
//		wsprintf(szLocalAddress, "%s:%s", GetIp(TCPExTable.table[i].dwLocalAddr, szLocalName), GetPort(TCPExTable.table[i].dwLocalPort, szLocalPort));
//		wsprintf(szRemoteAddress, "%s:%s", GetIp(TCPExTable.table[i].dwRemoteAddr, szRemoteName), GetPort(TCPExTable.table[i].dwRemotePort, szRemotePort));
//
//		printf("%-6s%-20s%-20s%-11s%s:%d\n", "TCP",
//			szLocalAddress, szRemoteAddress, TcpState[TCPExTable.table[i].dwState], ProcessPidToName(hProcessSnap, TCPExTable.table[i].dwOwningPid, szProcessName),
//			TCPExTable.table[i].dwOwningPid);
//
//	}
//	for (i = 0; i < UDPExTable.dwNumEntries; i++)
//	{
//		sprintf(szLocalAddress, "%s:%s",
//			GetIp(UDPExTable.table[i].dwLocalAddr, szLocalName),
//			GetPort(UDPExTable.table[i].dwLocalPort, szLocalPort));
//		sprintf(szRemoteAddress, "%s", "*:*");
//		printf("%-6s%-20s%-33s%s:%d\n", "UDP",
//			szLocalAddress, szRemoteAddress,
//			ProcessPidToName(hProcessSnap, UDPExTable.table[i].dwOwningPid, szProcessName),
//			UDPExTable.table[i].dwOwningPid
//		);
//
//	}
//	WSACleanup();
//	getchar();
//	system("pause");
//	return 0;
//}