#include "MainWindow.h"



int WINAPI WinMain(HINSTANCE hInst, HINSTANCE prevInst, LPSTR args, int ncmdshow)
{

	auto MainClass = NewWndClass(hInst, (HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), LoadIcon(NULL, IDI_APPLICATION), L"ServerWindow", SoftwareMainProcedure);

	if (!RegisterClassW(&MainClass))
	{
		return -1;
	}
	MSG MainMessage = { 0 };
	hBitMap = LoadBitmap(hInst, L"lamp.png");
	MainWnd = CreateWindow(L"ServerWindow", L"ServerWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 600, 600, NULL, NULL, hInst, NULL);
	while (GetMessage(&MainMessage, NULL, NULL, NULL))
	{
		TranslateMessage(&MainMessage);
		DispatchMessage(&MainMessage);
	}
	return 0;
}



WNDCLASS NewWndClass(HINSTANCE hInst, HBRUSH BGColor, HCURSOR Cursor, HICON Icon, LPCWSTR name, WNDPROC procedure)
{
	WNDCLASS NewClass = { 0 };
	NewClass.hInstance = hInst;
	NewClass.hbrBackground = BGColor;
	NewClass.hCursor = Cursor;
	NewClass.hIcon = Icon;
	NewClass.lpszClassName = name;
	NewClass.lpfnWndProc = procedure;
	return NewClass;
}


void MainWidget(HWND hwnd)
{
}


LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC hdc, hmdc;
	PAINTSTRUCT ps;
	switch (msg)
	{
	case WM_CREATE:
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		hmdc = CreateCompatibleDC(hdc);
		SelectObject(hmdc, hBitMap);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	}
}