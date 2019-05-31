//#include <Windows.h>
//#include <iostream>
//using namespace std;
//
//DWORD WINAPI Func1Proc(
//	_In_ LPVOID lpParameter
//);
//DWORD WINAPI Func2Proc(
//	_In_ LPVOID lpParameter
//);
//
//int tickets = 100;
//HANDLE hEvent;
////事件对象
//int main()
//{
//	HANDLE hand1 = CreateThread(NULL, 0, Func1Proc, NULL, 0, NULL);
//	HANDLE hand2 = CreateThread(NULL, 0, Func2Proc, NULL, 0, NULL);
//	CloseHandle(hand1);
//	CloseHandle(hand2);
//
//	/*hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);*/
//	hEvent = CreateEvent(NULL, FALSE, FALSE, "Tickets");
//	if (hEvent) {
//		if (ERROR_ALREADY_EXISTS == GetLastError()) {
//			cout << "只能有一个实例运行" << endl;
//			return 0;
//		}
//	}
//
//	SetEvent(hEvent);
//	Sleep(10);
//	CloseHandle(hEvent);
//	system("pause");
//}
//
//DWORD WINAPI Func1Proc(
//	_In_ LPVOID lpParameter
//) {
//	while (TRUE)
//	{
//		WaitForSingleObject(hEvent, INFINITE);
//		if (tickets > 0) {
//			cout << "Thread1 Sell Ticket" << tickets-- << endl;
//		}
//		else break;
//		SetEvent(hEvent);
//	}
//	return 0;
//}
//
//DWORD WINAPI Func2Proc(
//	_In_ LPVOID lpParameter
//) {
//	while (TRUE)
//	{
//		WaitForSingleObject(hEvent, INFINITE);
//		if (tickets > 0) {
//			cout << "Thread2 Sell Ticket" << tickets-- << endl;
//		}
//		else break;
//		SetEvent(hEvent);
//	}
//	return 0;
//}