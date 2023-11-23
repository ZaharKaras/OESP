#include "Console.h"

Console::Console() {
	hwnd = GetConsoleWindow();
}

void Console::hide() {
	ShowWindow(hwnd, SW_HIDE);
}

void Console::show() {
	ShowWindow(hwnd, SW_SHOW);
}