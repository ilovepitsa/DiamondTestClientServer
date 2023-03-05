#define WIN32_LEAN_AND_MEAN
#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <Windows.h>
#include "ClientClass.h"


HWND MainWnd;
HWND ConnectButtonHWND;
HWND SendButtonHWND;
HWND EditHWND;
ClientClass* client;

#define OnConnectButtonClicked 1
#define OnSendButtonClicked 2



HINSTANCE hInst;


HFONT font;
LPDRAWITEMSTRUCT Item;
LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);
WNDCLASS NewWndClass(HINSTANCE hInst, HBRUSH BGColor, HCURSOR Cursor, HICON Icon, LPCWSTR name, WNDPROC procedure);
void MainWidget(HWND hwnd);
int len;
char* buf;
char* toSend;
PAINTSTRUCT ps;