#include "MainWindow.h"
#include "resource.h"
#include <strsafe.h>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPSTR args, int ncmdshow)
{
	hInst=hInstance;
	auto MainClass = NewWndClass(hInst, (HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), LoadIcon(NULL, IDI_APPLICATION), "ServerWindow", SoftwareMainProcedure);

	if (!RegisterClassA(&MainClass))
	{
		return -1;
	}
	MSG MainMessage = { 0 };


	
	
	MainWnd = CreateWindowA("ServerWindow", "ServerWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 700, 700, NULL, NULL, hInst, NULL);
	while (GetMessage(&MainMessage, NULL, NULL, NULL))
	{
		TranslateMessage(&MainMessage);
		DispatchMessage(&MainMessage);
	}
	return 0;
}



WNDCLASS NewWndClass(HINSTANCE hInst, HBRUSH BGColor, HCURSOR Cursor, HICON Icon, LPCSTR name, WNDPROC procedure)
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
	StaticHwnd = CreateWindowA("static", "startup message", WS_VISIBLE | WS_CHILD | ES_CENTER, 5,150,600,120,hwnd,NULL,NULL,NULL);
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
		hBitMap = (HBITMAP)LoadImage(hInst, "lamp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		hBitMapConnected = (HBITMAP)LoadImage(hInst, "lightLamp.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		MainWidget(hWnd);
		server = new ServerClass(hWnd);
		if (server->getFailed())
		{
			exit(1);
		}
		th= std::thread(&ServerClass::Startlisten,server);
		th.detach();
		break;
	case WM_COMMAND:
		switch (wp)
		{
		case onGetText:
			changeText();
			break;
		default:
			break;
		}


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

	SetWindowTextA(StaticHwnd, server->getText());
}