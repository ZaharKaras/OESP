#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

#include "libraries.h"
#include "Battery.h"

void MainWndAddWidgets(HWND hWnd);
void MainWndAddButton(HWND hWnd);
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

DWORD WINAPI Power(LPVOID lpParameter);
volatile bool isThreading = true;

HANDLE readThread;
HWND staticWindow;

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

	MSG msg{}; //структура которая содержит информацию о сообщении
	HWND hwnd{}; //дескриптор окна, хранит информацию об окне
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) }; //отвечает за характеристики окна

	wc.cbClsExtra = 0; //доп выделение памяти в классе окна
	wc.cbWndExtra = 0; //доп выделение памяти в классе окна
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //дескриптор кисти
	wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
	wc.lpfnWndProc = WndProc;
	wc.lpszClassName = L"BatteryApplication";
	wc.lpszMenuName = nullptr;
	wc.style = CS_VREDRAW | CS_HREDRAW;

	if (!RegisterClassExW(&wc))
		return EXIT_FAILURE;

	hwnd = CreateWindow(wc.lpszClassName, L"Battery", WS_OVERLAPPEDWINDOW, 0, 0, 600, 600, nullptr, nullptr, wc.hInstance, nullptr);

	if (hwnd == INVALID_HANDLE_VALUE)
		return EXIT_FAILURE;

	ShowWindow(hwnd, nCmdShow); //показать окно
	UpdateWindow(hwnd); //перерисовка окна

	while (GetMessage(&msg, nullptr, 0, 0)) //Цикл обработки сообщений
	{
		TranslateMessage(&msg); //расшифровывает сообщение
		DispatchMessage(&msg); //передает сообщение в оконную процедуру
	}

	TerminateThread(readThread, 0);

	return static_cast<int>(msg.wParam);
}


LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		MainWndAddButton(hWnd);
		MainWndAddWidgets(hWnd);

		readThread = CreateThread(nullptr, 0, Power, nullptr, 0, nullptr);
	}
	return 0;

	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 1337:
		{
			SetSuspendState(TRUE, FALSE, FALSE);
		}
		break;
		}
	}
	return 0;

	case WM_DESTROY:
	{
		isThreading = false;
		CloseHandle(readThread);
		PostQuitMessage(EXIT_SUCCESS);
	}
	return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}


void MainWndAddWidgets(HWND hWnd)
{
	staticWindow = CreateWindow(L"STATIC", L"Hello window", WS_VISIBLE | WS_CHILD | BS_MULTILINE, 5, 5, 480, 160, hWnd, nullptr, nullptr, nullptr);
}

void MainWndAddButton(HWND hWnd)
{
	CreateWindow(L"BUTTON", L"Sleep", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 200, 300, 30, hWnd, HMENU(1337), nullptr, nullptr);
}

DWORD WINAPI Power(LPVOID lpParameter)
{
	while (isThreading) {
		Battery battery;
		SetWindowTextA(staticWindow, BatteryStatus(battery).c_str());
		Sleep(1000);
	}

	return 0;
}
