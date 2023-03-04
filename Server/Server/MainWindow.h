#define WIN32_LEAN_AND_MEAN
#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <Windows.h>
#include "ServerClass.h"




HWND MainWnd;
HBITMAP hBitMap;
HBITMAP hBitMapConnected;
HINSTANCE hInst;
HWND StaticHwnd;
ServerClass* server;


LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWndClass(HINSTANCE hInst, HBRUSH BGColor, HCURSOR Cursor, HICON Icon, LPCWSTR name, WNDPROC procedure);
void MainWidget(HWND hwnd);