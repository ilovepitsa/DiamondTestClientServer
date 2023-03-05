#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include "Logger.h"
using namespace std;
class ClientClass
{
public:
	ClientClass(HWND hWnd);
	~ClientClass();

	bool sendMessage(std::string msg);
	void connectServer();
	bool checkConnection();
	bool checkFailed();
private:
	HWND hWnd;
	void checkDisconect();
	Logger log;
	void loadConfig();
	std::string port;
	std::string ip;
	ADDRINFO* addrRes = NULL;
	SOCKET ConnectionSocket = INVALID_SOCKET;
	bool bIsConnected = false;
	bool bIsFailed = false;
	thread th;
};

