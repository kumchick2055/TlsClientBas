#pragma once

#include <mutex>
#include <Windows.h>

// Функции для работы с GUI
bool startGui();
bool stopGui();
bool addHttpData(char* requestData, char* responseData);

// Объявления функций из вашего cpp файла
bool init(HMODULE hModule);
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Декларации переменных из вашего cpp файла
extern std::atomic_bool isRunning;
extern std::atomic_bool isDone;
extern std::mutex mux;
extern std::string currentRequestData;
