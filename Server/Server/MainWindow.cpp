#include "MainWindow.h"
#include "resource.h"
#include <strsafe.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPSTR args, int ncmdshow)
{
	hInst=hInstance;
	auto MainClass = NewWndClass(hInst, (HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), LoadIcon(NULL, IDI_APPLICATION), L"ServerWindow", SoftwareMainProcedure);

	if (!RegisterClassW(&MainClass))
	{
		return -1;
	}
	MSG MainMessage = { 0 };


	
	
	MainWnd = CreateWindow(L"ServerWindow", L"ServerWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 700, 700, NULL, NULL, hInst, NULL);
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
	StaticHwnd = CreateWindow(L"static", L"aboba", WS_VISIBLE | WS_CHILD | ES_CENTER, 5,150,600,120,hwnd,NULL,NULL,NULL,NULL);
}


LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	HDC hdc, hmdc;
	PAINTSTRUCT ps;
	BITMAP bm;
	RECT rectPlace;

	std::thread th;
	switch (msg)
	{
	case WM_CREATE:
		hBitMap = (HBITMAP)LoadImage(hInst, L"lamp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBitMapConnected = (HBITMAP)LoadImage(hInst, L"lightLamp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		MainWidget(hWnd);
		server = new ServerClass();
		if (server->getFailed())
		{
			exit(1);
		}
		th= std::thread(&ServerClass::Startlisten,server);
		th.detach();
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rectPlace);
		hmdc = CreateCompatibleDC(hdc);

		if (server->checkConnection())
		{
			SelectObject(hmdc, hBitMapConnected);
			GetObject(hBitMapConnected, sizeof(bm), (LPSTR)&bm);
		}
		else
		{
			SelectObject(hmdc, hBitMap);
			GetObject(hBitMap, sizeof(bm), (LPSTR)&bm);
		}
		BitBlt(hdc, 0, 0, bm.bmWidth, bm.bmHeight, hmdc, 0, 0, SRCCOPY);
		DeleteDC(hmdc);
		EndPaint(hWnd, &ps);

		break;
	case WM_DESTROY:
		delete server;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	}
}


void changeText()
{
	std::unique_lock<std::mutex> unqlck(server->mtx);
	while (!server->checkRecv())
		server->cv.wait(unqlck);

	SetWindowTextA(StaticHwnd, server->getText());
}