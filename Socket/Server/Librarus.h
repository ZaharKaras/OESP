#pragma once

#define WIN_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING	
#define _WIN32_WINNT 0x0A00

#define PHOTO_FOLDER_NAME "./Photo/"  // папка для фото
#define PHOTO_FORMAT ".jpg"           // расширение фото
#define PHOTO_DELAY 5000              // задержка между фото

#include <iostream>
#include <iomanip>
#include <string>
#include <locale.h>
#include <fstream>
#include <experimental/filesystem>
#include <set>
#include <thread>
#include <atomic>
#include <vector>
#include <cmath>
#include <winsock2.h>

#include <opencv2\opencv.hpp>
#include <windows.h>
#include <vfw.h>
#include <setupapi.h>
#include <devguid.h>

#pragma comment(lib,"Ws2_32.lib")
#pragma comment(lib,"Wsock32.lib")
#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "vfw32.lib")

using namespace std;
using namespace cv;
