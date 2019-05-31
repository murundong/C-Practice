#include<iostream>
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>
#include <IPHlpApi.h>
#include<Psapi.h>	
#include<tlhelp32.h>
using namespace std;
#pragma comment (lib,"Iphlpapi.lib")
#pragma comment (lib,"Ws2_32.lib")
#pragma comment (lib,"toolhelp.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


void GetTcpConnectionTable();
void GetTcpModuleTable();
int	 GetPerConnection();
PCHAR ProcessPidToName(HANDLE hProcessSnap, DWORD ProcessId, PCHAR ProcessName);
void main() {

	GetTcpConnectionTable();

	system("pause");
}

void GetTcpConnectionTable() {
		HANDLE hProcessSnap;
		char szProcessName[MAX_PATH];
		MIB_TCPTABLE_OWNER_PID* pMtop;
		MIB_TCPROW_OWNER_PID * pMtopRow;
		DWORD dwSize = 0;
		DWORD dwRes = 0;


		ULONG perResult, rodSize = 0;
		PUCHAR ros = 0, rod = 0;
		rodSize = sizeof(TCP_ESTATS_DATA_ROD_v0);
		PTCP_ESTATS_DATA_ROD_v0 dataRod = { 0 };

		pMtop = (MIB_TCPTABLE_OWNER_PID*)MALLOC(sizeof(MIB_TCPTABLE_OWNER_PID));
		if (pMtop == NULL) {
			cout << "Error allocating memory!" << endl;
			return;
		}
		dwSize = sizeof(MIB_TCPTABLE_OWNER_PID);
		dwRes = GetExtendedTcpTable(pMtop, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_CONNECTIONS, 0);
		if (ERROR_INSUFFICIENT_BUFFER == dwRes) {
			FREE(pMtop);
			pMtop = (MIB_TCPTABLE_OWNER_PID*)MALLOC(dwSize);
			if (pMtop == NULL) {
				cout << "Error allocating memory!" << endl;
				return;
			}
		}

		if ((dwRes = GetExtendedTcpTable(pMtop, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_PID_CONNECTIONS, 0)) == NO_ERROR) {
			hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			for (int i = 0; i < pMtop->dwNumEntries; i++)
			{
				pMtopRow = (MIB_TCPROW_OWNER_PID*)&pMtop->table[i];
				if (pMtopRow->dwOwningPid != 12452) continue;
				printf("pid(%s):%d\t[%d]", ProcessPidToName(hProcessSnap, pMtopRow->dwOwningPid, szProcessName), pMtopRow->dwOwningPid, pMtopRow->dwState);
				PMIB_TCPROW pRow = (MIB_TCPROW*)&pMtop->table[i];

				if (rodSize != 0) {
					rod = (PUCHAR)malloc(rodSize);
					if (rod == NULL) {
						free(rod);
						printf("rod out of Memory!£»");
						return;
					}
				}
				perResult = GetPerTcpConnectionEStats(pRow, TcpConnectionEstatsData, NULL, 0, 0, NULL, 0, 0, rod, 0, rodSize);
				if (perResult == NO_ERROR) {
					dataRod = (PTCP_ESTATS_DATA_ROD_v0)rod;
					printf("[In]:%d\t[Out]:%d\n", dataRod->DataBytesIn, dataRod->DataBytesOut);
				}
			}
			/*CloseToolhelp32Snapshot(hProcessSnap);*/
		}
		else {
			cout << "False" << dwRes << endl;
	}

}

void GetTcpModuleTable() {
	MIB_TCPTABLE_OWNER_MODULE* pMtop;
	MIB_TCPROW_OWNER_MODULE  * pMtopRow;
	DWORD dwSize = 0;
	DWORD dwRes = 0;


	pMtop = (MIB_TCPTABLE_OWNER_MODULE*)MALLOC(sizeof(MIB_TCPTABLE_OWNER_MODULE));
	if (pMtop == NULL) {
		cout << "Error allocating memory!" << endl;
		return;
	}
	dwSize = sizeof(MIB_TCPTABLE_OWNER_MODULE);
	dwRes = GetExtendedTcpTable(pMtop, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_MODULE_CONNECTIONS, 0);
	if (ERROR_INSUFFICIENT_BUFFER == dwRes) {
		FREE(pMtop);
		pMtop = (MIB_TCPTABLE_OWNER_MODULE*)MALLOC(dwSize);
		if (pMtop == NULL) {
			cout << "Error allocating memory!" << endl;
			return;
		}
	}

	if ((dwRes = GetExtendedTcpTable(pMtop, &dwSize, FALSE, AF_INET, TCP_TABLE_OWNER_MODULE_CONNECTIONS, 0)) == NO_ERROR) {

		for (int i = 0; i < pMtop->dwNumEntries; i++)
		{
			pMtopRow = (MIB_TCPROW_OWNER_MODULE *)&pMtop->table[i];
			printf("pid:%d\t[%d]", pMtopRow->dwOwningPid, pMtopRow->dwState);
			printf("Local:%s:%d\t", inet_ntoa(*(in_addr*)&pMtopRow->dwLocalAddr), htons(pMtopRow->dwLocalPort));
			printf("Remote:%s:%d\n", inet_ntoa(*(in_addr*)&pMtopRow->dwRemoteAddr), htons(pMtopRow->dwRemotePort));
		}
	}
	else {
		cout << "False" << dwRes << endl;
	}
}

int	 GetPerConnection() {
	//Declare and initialize variables
	PMIB_TCPTABLE pTcpTable;
	PMIB_TCPROW row;
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;
	char szLocalAddr[128];
	char szRemoteAddr[128];
	ULONG rosSize = 0, rodSize = 0;
	ULONG winStatus;
	PUCHAR ros = NULL, rod = NULL;
	PTCP_ESTATS_DATA_ROD_v0 dataRod = { 0 };
	struct in_addr IpAddr;
	int i;
	pTcpTable = (MIB_TCPTABLE *)MALLOC(sizeof(MIB_TCPTABLE));
	if (pTcpTable == NULL) {
		printf("Error allocating memory\n");
		return 1;
	}
	dwSize = sizeof(MIB_TCPTABLE);
	rodSize = sizeof(TCP_ESTATS_DATA_ROD_v0);
	// Make an initial call to GetTcpTable to
	// get the necessary size into the dwSize variable
	if ((dwRetVal = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_BASIC_ALL, 0)) ==
		ERROR_INSUFFICIENT_BUFFER) {
		FREE(pTcpTable);
		pTcpTable = (MIB_TCPTABLE *)MALLOC(dwSize);
		if (pTcpTable == NULL) {
			printf("Error allocating memory\n");
			return 1;
		}
	}
	// Make a second call to GetTcpTable to get
	// the actual data we require
	if ((dwRetVal = GetExtendedTcpTable(pTcpTable, &dwSize, TRUE, AF_INET, TCP_TABLE_BASIC_ALL, 0)) == NO_ERROR) {
		row = pTcpTable->table;
		printf("\tNumber of entries: %d\n", (int)pTcpTable->dwNumEntries);
		for (i = 0; i < (int)pTcpTable->dwNumEntries; i++) {
			printf("\n\tTCP[%d] State: %ld - ", i,
				pTcpTable->table[i].dwState);
			switch (pTcpTable->table[i].dwState) {
			case MIB_TCP_STATE_CLOSED:
				printf("CLOSED\n");
				break;
			case MIB_TCP_STATE_LISTEN:
				printf("LISTEN\n");
				break;
			case MIB_TCP_STATE_SYN_SENT:
				printf("SYN-SENT\n");
				break;
			case MIB_TCP_STATE_SYN_RCVD:
				printf("SYN-RECEIVED\n");
				break;
			case MIB_TCP_STATE_ESTAB:
				printf("ESTABLISHED\n");
				break;
			case MIB_TCP_STATE_FIN_WAIT1:
				printf("FIN-WAIT-1\n");
				break;
			case MIB_TCP_STATE_FIN_WAIT2:
				printf("FIN-WAIT-2 \n");
				break;
			case MIB_TCP_STATE_CLOSE_WAIT:
				printf("CLOSE-WAIT\n");
				break;
			case MIB_TCP_STATE_CLOSING:
				printf("CLOSING\n");
				break;
			case MIB_TCP_STATE_LAST_ACK:
				printf("LAST-ACK\n");
				break;
			case MIB_TCP_STATE_TIME_WAIT:
				printf("TIME-WAIT\n");
				break;
			case MIB_TCP_STATE_DELETE_TCB:
				printf("DELETE-TCB\n");
				break;
			default:
				printf("UNKNOWN dwState value\n");
				break;
			}
			IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwLocalAddr;
			strcpy_s(szLocalAddr, sizeof(szLocalAddr), inet_ntoa(IpAddr));
			printf("\tTCP[%d] Local Addr: %s\n", i, szLocalAddr);
			printf("\tTCP[%d] Local Port: %d \n", i,
				ntohs((u_short)pTcpTable->table[i].dwLocalPort));
			IpAddr.S_un.S_addr = (u_long)pTcpTable->table[i].dwRemoteAddr;
			strcpy_s(szRemoteAddr, sizeof(szRemoteAddr), inet_ntoa(IpAddr));
			printf("\tTCP[%d] Remote Addr: %s\n", i, szRemoteAddr);
			printf("\tTCP[%d] Remote Port: %d\n", i,
				ntohs((u_short)pTcpTable->table[i].dwRemotePort));
			if (rodSize != 0) {
				rod = (PUCHAR)malloc(rodSize);
				if (rod == NULL) {
					printf("Out of Memory");
				}
			}

			winStatus = GetPerTcpConnectionEStats(row, TcpConnectionEstatsData, NULL, 0, 0, NULL, 0, 0, rod, 0, rodSize);
			if (winStatus == NO_ERROR)
			{
				dataRod = (PTCP_ESTATS_DATA_ROD_v0)rod;
				printf("\tData\n");
				printf("\tBytes Out: %lu\n", dataRod->DataBytesOut);
				printf("\tBytes In : %lu\n", dataRod->DataBytesIn);
			}
			++row;

		}
	}
	else {
		printf("\tGetTcpTable failed with %d\n", dwRetVal);
		FREE(pTcpTable);
		return 1;
	}
	if (pTcpTable != NULL) {
		FREE(pTcpTable);
		pTcpTable = NULL;
	}
	system("PAUSE");
	return 0;
}


PCHAR ProcessPidToName(HANDLE hProcessSnap, DWORD ProcessId, PCHAR ProcessName)
{
	PROCESSENTRY32 processEntry;
	strcpy(ProcessName, "..");
	if (!Process32First(hProcessSnap, &processEntry)) {
		return ProcessName;
	}
	do {
		if (processEntry.th32ProcessID == ProcessId)
		{
			strcpy(ProcessName, processEntry.szExeFile);
			return ProcessName;
		}
	} while (Process32Next(hProcessSnap, &processEntry));
	return ProcessName;
}