//#include <windows.h>
//#include <iostream>	
//#include <time.h>	
//using namespace std;
//
//DWORD WINAPI Fun1Proc(
//	_In_ LPVOID lpParameter
//);
//DWORD WINAPI Fun2Proc(
//	_In_ LPVOID lpParameter
//);
//int tickets = 100;
//HANDLE hMutex;
//int main() {
//	HANDLE hand1 = CreateThread(NULL, 0, Fun1Proc, NULL, 0, NULL);
//	HANDLE hand2 = CreateThread(NULL, 0, Fun2Proc, NULL, 0, NULL);
//	CloseHandle(hand1);
//	CloseHandle(hand2);
//	hMutex = CreateMutex(NULL, false, "Tickets");
//	if (hMutex) {
//		if (ERROR_ALREADY_EXISTS == GetLastError()) {
//			cout << "只能有一个实例运行！" << endl;
//			return 0;
//		}
//	}
//	Sleep(10);
//	system("pause");
//
//}
//
//
//DWORD WINAPI Fun1Proc(
//	_In_ LPVOID lpParameter
//)
//{
//	while (true)
//	{
//		WaitForSingleObject(hMutex, INFINITE);
//		if (tickets > 0)
//			cout << "Thread1 sell Ticket:" << tickets-- << endl;
//		else
//			break;
//		ReleaseMutex(hMutex);
//	}
//	return 0;
//}
//
//DWORD WINAPI Fun2Proc(
//	_In_ LPVOID lpParameter
//)
//{
//	while (true)
//	{
//		WaitForSingleObject(hMutex, INFINITE);
//		if (tickets > 0)
//			cout << "Thread2 sell Ticket:" << tickets-- << endl;
//		else
//			break;
//		ReleaseMutex(hMutex);
//	}
//	return 0;
//}