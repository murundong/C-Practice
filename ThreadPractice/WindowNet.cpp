//#include <winsock2.h>
//#include <ws2tcpip.h>
//#pragma comment(lib, "IPHLPAPI.lib")
//
//#include <iphlpapi.h>
//
//#include <stdio.h>
//#include <stdlib.h>
//
//#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
//#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
//
//static DWORD last_In = 0;
//static DWORD last_Out = 0;
//
//void PrintTotalNetworkTraffic();
//int main()
//{
//	PrintTotalNetworkTraffic();
//	system("pause");
//	return 0;
//}
//
//
//
//void PrintTotalNetworkTraffic() {
//	while (TRUE)
//	{
//		DWORD dwSize = 0;
//		DWORD dwRetVal = 0;
//
//		MIB_IFTABLE *pIfTable;
//		MIB_IFROW *pIfRow;
//		// Allocate memory for our pointers.
//		pIfTable = (MIB_IFTABLE *)MALLOC(sizeof(MIB_IFTABLE));
//		if (pIfTable == NULL) {
//			printf("Error allocating memory needed to call GetIfTable\n");
//			return;
//		}
//		// Make an initial call to GetIfTable to get the
//		// necessary size into dwSize
//		dwSize = sizeof(MIB_IFTABLE);
//		if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) {
//			FREE(pIfTable);
//			pIfTable = (MIB_IFTABLE *)MALLOC(dwSize);
//			if (pIfTable == NULL) {
//				printf("Error allocating memory needed to call GetIfTable\n");
//				return;
//			}
//		}
//		if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) {
//			if (pIfTable->dwNumEntries > 10) {
//				pIfRow = (MIB_IFROW *)& pIfTable->table[6];
//				if (last_In == 0 || last_Out == 0) {
//					printf("Start¡­¡­ \n");
//				}
//				else {
//					printf("In: [:%d]\t %.2f kb/s", (pIfRow->dwInOctets - last_In), (pIfRow->dwInOctets - last_In) / 1024.0f);
//					printf("\tOut: [:%d] \t %.2f kb/s", (pIfRow->dwInOctets - last_In), (pIfRow->dwOutOctets - last_Out) / 1024.0f);
//					printf("\n");
//				}
//				last_In = pIfRow->dwInOctets;
//				last_Out = pIfRow->dwOutOctets;
//			}
//		}
//		Sleep(1000);
//	}
//}