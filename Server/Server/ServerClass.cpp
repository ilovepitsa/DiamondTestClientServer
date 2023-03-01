#include "ServerClass.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
using namespace std;
int wait()
{
	WSAData ws;
	ADDRINFOA hints;
	ADDRINFO* addrRes = NULL;

	int result = WSAStartup(MAKEWORD(2, 2), &ws);
	if (result != 0)
	{
		std::cout << "error startup";
		return -4;
	}
	cout << "end startup" << endl;
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if (getaddrinfo(NULL, "777", &hints, &addrRes))
	{
		cout << "error getaddrinfo";
		freeaddrinfo(addrRes);
		WSACleanup();
		return -3;
	}
	cout << "end getaddrinfo" << endl;
	SOCKET ClientSocket = INVALID_SOCKET, ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(addrRes->ai_family, addrRes->ai_socktype, addrRes->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		cout << "error socket";
		WSACleanup();
		freeaddrinfo(addrRes);
		return -2;
	}
	cout << "end socket" << endl;
	if (bind(ListenSocket, addrRes->ai_addr, (int)addrRes->ai_addrlen) == SOCKET_ERROR)
	{
		cout << "error bind";
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
		freeaddrinfo(addrRes);
		WSACleanup();
		return -1;
	}
	cout << "end bind" << endl;

	if (listen(ListenSocket, 1) == SOCKET_ERROR)
	{
		cout << "Listen failed";
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
		freeaddrinfo(addrRes);
		WSACleanup();
		return -1;
	}
	cout << "end listen" << endl;
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET)
	{
		cout << "accept failed";
		closesocket(ListenSocket);
		ListenSocket = INVALID_SOCKET;
		freeaddrinfo(addrRes);
		WSACleanup();
		return -1;
	}
	cout << "end accept" << endl;
	closesocket(ListenSocket);

	string sendbuffer = "aboba server\0";
	char* recvBuffer = new char[512];
	ZeroMemory(recvBuffer, 512);
	int res;
	do
	{
		res = recv(ClientSocket, recvBuffer, 512, NULL);
		if (res > 0)
		{
			cout << endl << "Recieve  " << recvBuffer << endl << "Size " << res << endl;
			ZeroMemory(recvBuffer, 512);
			if (send(ClientSocket, sendbuffer.c_str(), (int)sendbuffer.size(), NULL))
			{
				closesocket(ClientSocket);
				ListenSocket = INVALID_SOCKET;
				freeaddrinfo(addrRes);
				WSACleanup();
				return -1;
			}
		}
		else if (res == 0)
		{
			cout << "connect closing";
		}
		else
		{
			cout << "recv failde";
			closesocket(ClientSocket);
			ClientSocket = INVALID_SOCKET;
			freeaddrinfo(addrRes);
			WSACleanup();
			return 1;
		}
	} while (res > 0);
	if (shutdown(ClientSocket, SD_SEND) == SOCKET_ERROR)
	{
		cout << "error shutdown";
		closesocket(ClientSocket);
		ClientSocket = INVALID_SOCKET;
		freeaddrinfo(addrRes);
		WSACleanup();
		return -2;
	}
	closesocket(ClientSocket);
	ClientSocket = INVALID_SOCKET;
	freeaddrinfo(addrRes);
	WSACleanup();

	delete[] recvBuffer;
	return 0;
}