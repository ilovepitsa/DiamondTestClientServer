#include "ClientClass.h"
#include <fstream>
#include <mutex>



ClientClass::ClientClass(HWND hWnd)
{
	this->hWnd = hWnd;
	WSAData ws;
	ADDRINFOA hints = { 0 };
	if (WSAStartup(MAKEWORD(2, 2), &ws))
	{
		log.write("error startup");
	}
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	loadConfig();
	if (getaddrinfo(ip.c_str(), port.c_str(), &hints, &addrRes))
	{
		log.write("error getaddrinfo");
		freeaddrinfo(addrRes);
		WSACleanup();
	}
	if ((ConnectionSocket = socket(addrRes->ai_family, addrRes->ai_socktype, addrRes->ai_protocol)) == SOCKET_ERROR)
	{
		log.write("error socket");
		freeaddrinfo(addrRes);
		WSACleanup();
	}
}

ClientClass::~ClientClass()
{

	closesocket(ConnectionSocket);
	freeaddrinfo(addrRes);
	WSACleanup();
}
std::mutex mtx;
bool ClientClass::sendMessage(std::string msg)
{
	std::lock_guard<std::mutex> lckgrd(mtx);
	if(send(ConnectionSocket,msg.c_str(),(int)msg.size(),NULL)==SOCKET_ERROR)
		return false;
	return true;
}

void ClientClass::connectServer()
{
	if (bIsConnected)
		return;
	if (connect(ConnectionSocket, addrRes->ai_addr, (int)addrRes->ai_addrlen) == SOCKET_ERROR)
	{
		bIsConnected = false;
		return;
	}
	bIsConnected = true;
	th = thread(&ClientClass::checkDisconect, this);
	th.detach();
}

bool ClientClass::checkConnection()
{
	return bIsConnected;
}

bool ClientClass::checkFailed()
{
	return bIsFailed;
}

void ClientClass::checkDisconect()
{
	char* buffer = new char[1];
	int res;
	do
	{
		res = recv(ConnectionSocket, buffer, 1, NULL);
		if (res == -1)
		{
			bIsConnected = false;
			InvalidateRect(hWnd, NULL, TRUE);
		}
	} while (res > 0);
}

void ClientClass::loadConfig()
{
	char* buff = new char[256];
	ZeroMemory(buff, 256);
	ifstream configFile("config.txt", std::ios::in);
	while (configFile.getline(buff, (size_t)256, '='))
	{
		if (strcmp(buff, "port") == 0)
		{
			configFile.getline(buff, (size_t)256);
			port = buff;
		}
		else if (strcmp(buff, "ip")==0)
		{
			configFile.getline(buff, (size_t)256);
			ip = buff;
		}
		ZeroMemory(buff, 256);
	}
}
