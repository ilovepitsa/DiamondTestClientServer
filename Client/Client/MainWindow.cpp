#include "MainWindow.h"





int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPSTR args, int ncmdshow)
{
	font = CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH, "Arial Black");
	hInst = hInstance;
	auto MainClass = NewWndClass(hInst, (HBRUSH)COLOR_WINDOW, LoadCursor(NULL, IDC_ARROW), LoadIcon(NULL, IDI_APPLICATION), "ClientWindow", SoftwareMainProcedure);

	if (!RegisterClassA(&MainClass))
	{
		return -1;
	}
	MSG MainMessage = { 0 };


	toSend = new char[256];

	MainWnd = CreateWindowA("ClientWindow", "ClientWindow", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 700, 700, NULL, NULL, hInst, NULL);
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



LRESULT CALLBACK SoftwareMainProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{


	
	switch (msg)
	{
	case WM_CREATE:
		client = new ClientClass(hWnd);
		MainWidget(hWnd);
		
		break;
	case WM_COMMAND:
		switch (wp)
		{
		case OnConnectButtonClicked:
			client->connectServer();
			InvalidateRect(hWnd, NULL, TRUE);
			break;

		case OnSendButtonClicked:
			ZeroMemory(toSend, 256);
			GetWindowTextA(EditHWND, toSend, 256);
			client->sendMessage(std::string(toSend));
			break;
		default:
			break;
		}
		break;

	case WM_PAINT:
		BeginPaint(hWnd, &ps);
		if (client->checkConnection())
			SetTextColor(ps.hdc, RGB(0, 255, 0));
		else
			SetTextColor(ps.hdc, RGB(255, 0, 0));
		EndPaint(hWnd, &ps);
		break;
	case WM_DRAWITEM:
		Item = (LPDRAWITEMSTRUCT)lp;
		SetBkMode(Item->hDC, TRANSPARENT);
		if (client->checkConnection())
			SetTextColor(Item->hDC, RGB(0, 255, 0));
		else
			SetTextColor(Item->hDC, RGB(255, 0, 0));
		SelectObject(Item->hDC, font);
		if (Item->itemState & ODS_SELECTED)
			FillRect(Item->hDC, &Item->rcItem, (HBRUSH)GetStockObject(WHITE_BRUSH));
		else
			FillRect(Item->hDC, &Item->rcItem, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
		len = GetWindowTextLength(Item->hwndItem);
		buf = new char[len + 1];
		GetWindowTextA(Item->hwndItem, buf, len + 1);
		DrawTextA(Item->hDC, buf, len, &Item->rcItem, DT_CENTER |DT_SINGLELINE| DT_VCENTER);
		delete buf;
		return true;

		break;
	case WM_DESTROY:
		delete client;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, msg, wp, lp);
		break;
	}
}


void MainWidget(HWND hwnd)
{

	ConnectButtonHWND = CreateWindowA("button", "connect", WS_VISIBLE | WS_CHILD | ES_CENTER | BS_PUSHBUTTON | BS_OWNERDRAW, 250, 50, 150, 120, hwnd, (HMENU)OnConnectButtonClicked, NULL, NULL);
	EditHWND = CreateWindowA("edit", "write you message", WS_VISIBLE | WS_CHILD, 225, 200, 200, 20, hwnd, NULL, NULL, NULL);
	SendButtonHWND = CreateWindowA("button", "send", WS_VISIBLE | WS_CHILD | ES_CENTER | BS_PUSHBUTTON, 250, 240, 150, 120, hwnd, (HMENU)OnSendButtonClicked, NULL, NULL);

}
