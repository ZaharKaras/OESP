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
HWND childWindow;
HWND logsWindow;
int startPercent = 0;

using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

void* MapFileToMemory(const char* fileName, size_t& fileSize)
{
	// Открываем файл для чтения
	HANDLE fileHandle = CreateFileA(fileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		// Обработка ошибки открытия файла
		return nullptr;
	}

	// Получаем размер файла
	fileSize = GetFileSize(fileHandle, NULL);

	// Создаем отображение файла в память
	HANDLE fileMappingHandle = CreateFileMapping(fileHandle, NULL, PAGE_READONLY, 0, 0, NULL);
	if (fileMappingHandle == NULL)
	{
		// Обработка ошибки создания отображения файла
		CloseHandle(fileHandle);
		return nullptr;
	}

	// Отображаем файл в память
	void* mappedFile = MapViewOfFile(fileMappingHandle, FILE_MAP_READ, 0, 0, 0);
	if (mappedFile == NULL)
	{
		// Обработка ошибки отображения файла в память
		CloseHandle(fileMappingHandle);
		CloseHandle(fileHandle);
		return nullptr;
	}

	// Закрываем дескрипторы файла и отображения
	CloseHandle(fileMappingHandle);
	CloseHandle(fileHandle);

	return mappedFile;
}

void SaveBatteryStateToFile(int percent)
{
	// Отображаем файл в память
	size_t fileSize = 0;
	void* mappedFile = MapFileToMemory("D:\1\test.txt", fileSize);
	if (mappedFile == nullptr)
	{
		// Обработка ошибки отображения файла в память
		return;
	}

	// Открываем файл для записи
	FILE* file = nullptr;
	if (fopen_s(&file, "D:\1\test.txt", "a") != 0)
	{
		// Обработка ошибки открытия файла для записи
		UnmapViewOfFile(mappedFile);
		return;
	}

	// Записываем данные в файл
	// Получаем текущее время
	std::time_t currentTime = std::time(nullptr);
	std::tm localTime;
	localtime_s(&localTime, &currentTime);

	// Записываем время и проценты батареи в файл
	fprintf(file, "Time: %02d:%02d:%02d | Battery Percent: %d\n",
		localTime.tm_hour, localTime.tm_min, localTime.tm_sec, percent);
	// Закрываем файл и освобождаем отображение файла
	fclose(file);
	UnmapViewOfFile(mappedFile);
}

void OnPaint(HDC hdc)
{
	Graphics graphics(hdc);
	Pen pen1(Color(255, 0, 0, 0), 5);
	Pen pen2(Color(0, 255, 0, 0));
	SolidBrush brush(Color(0, 255, 0)); // Зеленая кисть

	graphics.DrawRectangle(&pen1, 10, 10, 400, 100);
	graphics.DrawRectangle(&pen2, 15, 15, 390, 90);

	int greenRectWidth = static_cast<int>(390 * startPercent / 100);

	graphics.FillRectangle(&brush, 15, 15, greenRectWidth, 90);

}

int CALLBACK wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{

	MSG msg{};
	HWND hwnd{}; 
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) }; 

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;

	// Initialize GDI+.
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

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

	ShowWindow(hwnd, nCmdShow); 
	UpdateWindow(hwnd); 

	while (GetMessage(&msg, nullptr, 0, 0)) 
	{
		TranslateMessage(&msg); 
		DispatchMessage(&msg); //передает сообщение в оконную процедуру
	}

	TerminateThread(readThread, 0);

	GdiplusShutdown(gdiplusToken);

	return static_cast<int>(msg.wParam);
}

void CreateLogsWindow()
{
	SetWindowTextA(logsWindow, " ");

	std::ifstream file("D:\1\test.txt");
	if (file.is_open())
	{
		std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

		SetWindowTextA(logsWindow, content.c_str());

		file.close();
	}
}

LRESULT CALLBACK LogsWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
	{
		logsWindow = CreateWindowEx(WS_EX_CLIENTEDGE, L"EDIT", nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL, 0, 0, 290, 400, hWnd, nullptr, nullptr, nullptr);
		CreateLogsWindow();
		
		break;
	}
	case WM_DESTROY:
	{
		
		break;
	}
	// Обработка других сообщений
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void CreateLogsWindow(HWND hWndParent)
{
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpfnWndProc = LogsWndProc;
	wc.lpszClassName = L"Logs";
	wc.hbrBackground = CreateSolidBrush(RGB(240, 0, 240));
	RegisterClassEx(&wc);

	HWND childwnd = CreateWindow(wc.lpszClassName, L"Logs Window", WS_OVERLAPPED | WS_CLIPCHILDREN  | ES_MULTILINE, 500, 500, 320, 420, hWndParent, nullptr, nullptr, nullptr);
	
	if (childwnd == nullptr)
	{
		MessageBox(hWndParent, L"Failed to create logss window", L"Error", MB_OK | MB_ICONERROR);
		return;
	}

	ShowWindow(childwnd, SW_SHOW);
	UpdateWindow(childwnd);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_CREATE:
	{
		MainWndAddButton(hWnd);
		MainWndAddWidgets(hWnd);

		readThread = CreateThread(nullptr, 0, Power, nullptr, 0, nullptr);
	}
	return 0;
	/*case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
	}
	return 0;*/
	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case 1337:
		{
			SetSuspendState(TRUE, FALSE, FALSE);
		}
		case 1338:
		{
			//MessageBox(hWnd, L"Hello", L"Message", MB_OK);
			CreateLogsWindow(hWnd);
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

LRESULT CALLBACK ChildWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (uMsg)
	{
	case WM_PAINT:
	{
		hdc = BeginPaint(hWnd, &ps);
		OnPaint(hdc);
		EndPaint(hWnd, &ps);
	}
	return 0;
	case WM_DESTROY:
	{
		PostQuitMessage(EXIT_SUCCESS);
	}
	return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void MainWndAddWidgets(HWND hWnd)
{
	staticWindow = CreateWindow(L"STATIC", L"Hello window", WS_VISIBLE | WS_CHILD | BS_MULTILINE, 5, 5, 480, 160, hWnd, nullptr, nullptr, nullptr);

	// Create child window
	WNDCLASSEX wc{ sizeof(WNDCLASSEX) };
	wc.hInstance = GetModuleHandle(nullptr);
	wc.lpfnWndProc = ChildWndProc;
	wc.lpszClassName = L"ChildWindowClass";
	wc.hbrBackground = CreateSolidBrush(RGB(240, 240, 240));
	RegisterClassEx(&wc);

	childWindow = CreateWindow(wc.lpszClassName, L"Child Window", WS_VISIBLE | WS_CHILD, 5, 300, 420, 120, hWnd, nullptr, wc.hInstance, nullptr);
}

void MainWndAddButton(HWND hWnd)
{
	CreateWindow(L"BUTTON", L"Sleep", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 200, 300, 30, hWnd, HMENU(1337), nullptr, nullptr);
	CreateWindow(L"BUTTON", L"Logs", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 10, 240, 300, 30, hWnd, HMENU(1338), nullptr, nullptr);
}

DWORD WINAPI Power(LPVOID lpParameter)
{
	while (isThreading) {
		Battery battery;

		if (battery.getPercent() != startPercent)
		{
			SaveBatteryStateToFile(battery.getPercent());
			
			CreateLogsWindow();
			
			InvalidateRect(childWindow, nullptr, TRUE);
		}
		startPercent = battery.getPercent();
		SetWindowTextA(staticWindow, BatteryStatus(battery).c_str());
		Sleep(1000);
	}

	return 0;
}
