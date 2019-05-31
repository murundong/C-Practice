#include<Windows.h>

HWND g_hWnd;
HHOOK g_hMouse;
HHOOK g_hKey;

HINSTANCE g_hInstance;

LRESULT CALLBACK MouseProc(
	_In_ int    nCode,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	return 1;
}


LRESULT CALLBACK KeyboardProc(
	_In_ int    code,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
) {
	if (VK_F2 == wParam) {
		::SendMessage(g_hWnd, WM_CLOSE, 0, 0);
		UnhookWindowsHookEx(g_hMouse);
		UnhookWindowsHookEx(g_hKey);
	}
	else {
		return 1;
	}
}


BOOL WINAPI DllMain(
	_Out_ HINSTANCE hInstance,
	_In_  ULONG     ulReason,
	LPVOID    Reserved
) {
	g_hInstance = hInstance;
	return	true;
}

_declspec(dllexport) void  SetHook(HWND hwnd) {
	g_hWnd = hwnd;
	/*g_hMouse =SetWindowsHookEx(WH_MOUSE, MouseProc, GetModuleHandle("HandDLL.dll"), 0);
	g_hKey =SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, GetModuleHandle("HandDLL.dll"), 0);*/
	g_hMouse = SetWindowsHookEx(WH_MOUSE, MouseProc, g_hInstance, 0);
	g_hKey = SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, g_hInstance, 0);
}