#include "Server.h"

Server::~Server() {
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
}

int Server::openServer() {
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failde!" << std::endl;
        return 1;
    }

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Invalid." << std::endl;
        WSACleanup();
        return 2;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(12345);

    if (::bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Invalid." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 4;
    }

    return 0;
}

int Server::connectServer() {
    if (listen(serverSocket, 1) == SOCKET_ERROR) {
        std::cerr << "Invalid." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Invalid." << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 2;
    }
    return 0;
}

int Server::sendMessage(string message) {
    int bytesReceived;
    bytesReceived = send(clientSocket, message.c_str(), message.size() + 1, 0);
    if (bytesReceived <= 0) {
        std::cout << "Invalid." << std::endl;
    }

    return bytesReceived;
}

int Server::getMessage(int *length, char *message) {
    int bytesReceived;

    sendMessage("send");
    bytesReceived = recv(clientSocket, message, 1024, 0);
    if (bytesReceived <= 0) {
        std::cout << "Error when receiving data." << std::endl;
    }

    *length = bytesReceived;

    return 0;
}

int Server::getPhoto() {
    int bytesReceived;
    char photo[1024 * 80];
    ofstream outputFile("./webcum/output.jpg", ios::out | ios::binary | ios::trunc);

    sendMessage("send");
    bytesReceived = recv(clientSocket, photo, 1024 * 80, 0);
    if (bytesReceived <= 0) {
        std::cout << "Error when receiving data." << std::endl;
    }

    for (int i = 0; i < bytesReceived; i++) {
        outputFile << photo[i];
    }

    outputFile.close();

    return 0;
}