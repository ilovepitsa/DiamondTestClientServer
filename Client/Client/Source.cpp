//#define WIN32_LEAN_AND_MEAN
#include <WinSock2.h>
#include <WS2tcpip.h>
#include<iostream>
//#include <Windows.h>
#pragma comment(lib, "Ws2_32.lib")
using namespace std;


int main()
{
	WSAData ws;
	ADDRINFOA hints;
	ADDRINFO* addrRes = NULL;

	int result = WSAStartup(MAKEWORD(2, 2), &ws);
	if (result != 0)
	{
		cout << "error startup";
		return -4;
	}
	memset(&hints, 0, sizeof(hints));

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo("localhost", "777", &hints, &addrRes))
	{
		cout << "error getaddrinfo";
		freeaddrinfo(addrRes);
		WSACleanup();
		return -3;
	}

	SOCKET ConnectSocket = INVALID_SOCKET;
	ConnectSocket = socket(addrRes->ai_family, addrRes->ai_socktype, addrRes->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		cout << "error socket";
		WSACleanup();
		freeaddrinfo(addrRes);
		return -2;
	}

	if (connect(ConnectSocket, addrRes->ai_addr, (int)addrRes->ai_addrlen) == SOCKET_ERROR)
	{
		cout << "error connect";
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;

		freeaddrinfo(addrRes);
		WSACleanup();
		return -1;
	}
	string sendbuffer = "aboba\0";
	if (send(ConnectSocket, sendbuffer.c_str(), (int)sendbuffer.size(), NULL) == SOCKET_ERROR)
	{
		cout << "error send";
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		freeaddrinfo(addrRes);
		WSACleanup();
		return -2;
	}
	if (shutdown(ConnectSocket, SD_SEND) == SOCKET_ERROR)
	{
		cout << "error shutdown";
		closesocket(ConnectSocket);
		ConnectSocket = INVALID_SOCKET;
		freeaddrinfo(addrRes);
		WSACleanup();
		return -2;
	}
	char* recvBuffer = new char[512];
	ZeroMemory(recvBuffer, 512);
	int res;
	do
	{
		res = recv(ConnectSocket, recvBuffer, 512, NULL);
		if (res > 0)
		{
			cout << endl << "Recieve  " << recvBuffer << endl << "Size " << res << endl;
			ZeroMemory(recvBuffer, 512);
		}
		else if (res == 0)
		{
			cout << "connect close";
		}
		else
		{
			cout << "recv failde";
		}
	} while (res > 0);

	closesocket(ConnectSocket);
	ConnectSocket = INVALID_SOCKET;
	freeaddrinfo(addrRes);
	WSACleanup();

	delete[] recvBuffer;
}