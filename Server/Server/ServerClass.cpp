#include "ServerClass.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <fstream>
#include <string.h>
#include <strsafe.h>


using namespace std;

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}
ServerClass::ServerClass(HWND hWnd)
{
	this->hWnd = hWnd;
	loadConfig();
	WSAData ws;
	if (WSAStartup(MAKEWORD(2, 2), &ws))
	{
		log.write("Startup error");
	}
	ADDRINFOA hints = { 0 };

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, port.c_str(), &hints, &addrRes))
	{
		ErrorExit(const_cast<LPTSTR>(TEXT("error")));
		log.write("getaddrinfo error");
		freeaddrinfo(addrRes);
		WSACleanup();
		bIsFailed = true;
	}
	if ((ListenSocket = socket(addrRes->ai_family, addrRes->ai_socktype, addrRes->ai_protocol)) == INVALID_SOCKET)
	{
		log.write("socket error");
		freeaddrinfo(addrRes);
		WSACleanup();
		bIsFailed = true;
	}
	if (bind(ListenSocket, addrRes->ai_addr, (int)addrRes->ai_addrlen) == SOCKET_ERROR )
	{

		log.write("bind error");
		freeaddrinfo(addrRes);
		WSACleanup();
		bIsFailed = true;
	}
}

ServerClass::~ServerClass()
{
	closesocket(ListenSocket);
	closesocket(ConnectionSocket);
	ListenSocket = INVALID_SOCKET;
	ConnectionSocket = INVALID_SOCKET;
	freeaddrinfo(addrRes);
	WSACleanup();
	delete[] recvBuffer;
}

void ServerClass::Startlisten()
{
	if (listen(ListenSocket, 1) == SOCKET_ERROR)
	{
		ErrorExit(const_cast<LPTSTR>(TEXT("error listen")));
		closesocket(ListenSocket);
		closesocket(ConnectionSocket);
		ListenSocket = INVALID_SOCKET;
		ConnectionSocket = INVALID_SOCKET;
		freeaddrinfo(addrRes);
		WSACleanup();
		return;
	}

	int res;
	ZeroMemory(recvBuffer, 256);
	while ((ConnectionSocket = accept(ListenSocket, NULL, NULL)) != INVALID_SOCKET)
	{
		bIsConnected = true;
		InvalidateRect(hWnd, NULL, TRUE);
		do {
			{
				std::unique_lock<std::mutex> unqlck(mtx);
				ZeroMemory(recvBuffer, 256);
				res = recv(ConnectionSocket, recvBuffer, 256, NULL);

			}
			if (res > 0)
			{
				SendMessage(hWnd, WM_COMMAND, (WPARAM)1, NULL);
			}
			else
			{
				bIsConnected = false;
				InvalidateRect(hWnd, NULL, TRUE);
			}
		} while (res > 0);
	}
}

bool ServerClass::getFailed()
{
	return bIsFailed;
}

bool ServerClass::checkConnection()
{
	return bIsConnected;
}

void ServerClass::loadConfig()
{
	char* buff = new char[256];
	ZeroMemory(buff, 256);
	ifstream configFile("config.txt", std::ios::in);
	while (configFile.getline(buff, (size_t)256, '='))
	{
		if (strcmp(buff, "port")==0)
		{
			configFile.getline(buff, (size_t)256);
			port=buff;
		}
		ZeroMemory(buff, 256);
	}
}


char* ServerClass::getText()
{
	return recvBuffer;
}

