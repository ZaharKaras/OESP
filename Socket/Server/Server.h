#pragma once

#include "Librarus.h"



class Server {
private:
	WSADATA wsaData;
	SOCKET serverSocket;
	sockaddr_in serverAddr;
	SOCKET clientSocket;
	sockaddr_in clientAddr;

	int clientAddrLen = sizeof(clientAddr);

public:
	~Server();

	int openServer();
	int connectServer();
	int sendMessage(string message);
	int getMessage(int *length, char *message);
	int getPhoto();
};

