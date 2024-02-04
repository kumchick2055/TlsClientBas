#pragma once

#include <mutex>
#include <Windows.h>

// ������� ��� ������ � GUI
bool startGui();
bool stopGui();
bool addHttpData(char* requestData, char* responseData);

// ���������� ������� �� ������ cpp �����
bool init(HMODULE hModule);
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// ���������� ���������� �� ������ cpp �����
extern std::atomic_bool isRunning;
extern std::atomic_bool isDone;
extern std::mutex mux;
extern std::string currentRequestData;
