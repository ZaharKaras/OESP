#pragma once
#include "Librarus.h"
#include "Camera.h"
#include "Console.h"
#include "Server.h"

class Manager {
private:
	const string photoName = "photo";     
	set<string> commands = {              
		"photo",
		"info",
		"help",
		"server",
		"exit"
	};
	VideoCapture camera; // объект камеры
	Console console; // объект консоли

public:
	bool isActive = false; // флаг активности приложения

	Manager();                             // конструктор по умолчанию
	~Manager();                            // деструктор
	void help();                           // вывод информации о доступных командах
	bool doCommand(string command);        // выполнение команды
	void makePhoto();                      // сделать фотографию
	void aboutCamera();                    // вывод информации о камере
	void doServer();                       // запустить сервер
	Console& getConsole();                 // получить объект консоли
	Server server;

};

