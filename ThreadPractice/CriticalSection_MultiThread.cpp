//#include <Windows.h>
//#include <iostream>	
//using namespace std;
//
//DWORD WINAPI Func1_Proc(
//	_In_ LPVOID lpParameter
//);
//
//DWORD WINAPI Func2_Proc(
//	_In_ LPVOID lpParameter
//);
//
//int tickets = 100;
////临界区对象
//CRITICAL_SECTION g_cs;
//int main() {
//	HANDLE hand1 = CreateThread(NULL, 0, Func1_Proc, NULL, 0, NULL);
//	HANDLE hand2 = CreateThread(NULL, 0, Func2_Proc, NULL, 0, NULL);
//
//	CloseHandle(hand1);
//	CloseHandle(hand2);
//
//	InitializeCriticalSection(&g_cs);
//
//	Sleep(3000);
//	DeleteCriticalSection(&g_cs);
//
//	system("pause");
//}
//
//
//DWORD WINAPI Func1_Proc(
//	_In_ LPVOID lpParameter
//) {
//	while (TRUE)
//	{
//		Sleep(1);
//		EnterCriticalSection(&g_cs);
//		if (tickets > 0)
//			cout << "Thread1 Sell Ticket " << tickets-- << endl;
//		else break;
//		LeaveCriticalSection(&g_cs);
//	}
//	return 0;
//}
//
//DWORD WINAPI Func2_Proc(
//	_In_ LPVOID lpParameter)
//{
//	while (TRUE)
//	{
//		Sleep(1);
//		EnterCriticalSection(&g_cs);
//		if (tickets > 0)
//			cout << "Thread2 Sell Ticket " << tickets-- << endl;
//		else break;
//		LeaveCriticalSection(&g_cs);
//	}
//	return 0;
//}
