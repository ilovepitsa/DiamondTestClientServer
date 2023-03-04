#pragma once
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <thread>
#include <condition_variable>
#include <mutex>
#include "Logger.h"


class ServerClass
{

public:
	ServerClass();
	~ServerClass();
	void Startlisten();
	bool getFailed();
	char* getText();
	bool checkConnection();
	std::mutex mtx;
	std::condition_variable cv;
	bool checkRecv();
private:
	void loadConfig();
	ADDRINFO* addrRes = NULL;
	SOCKET ListenSocket = INVALID_SOCKET;
	SOCKET ConnectionSocket = INVALID_SOCKET;
	Logger log;
	std::string port;
	char* recvBuffer = new char[256];
	bool bIsFailed = false;
	bool bIsConnected = false;
	std::mutex boolMtx;
	bool bRecv = false;
	friend void ErrorExit(LPTSTR lpszFunction);
};
