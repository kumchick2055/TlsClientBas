#pragma once

typedef char* (*DoRequest)(char* data);

extern "C" {
	typedef char* (*ResizeFunction)(int, void*);

	__declspec(dllexport) void* StartDll();
	__declspec(dllexport) void EndDll(void* DllData);
	__declspec(dllexport) void* StartThread();
	__declspec(dllexport) void EndThread(void* DllData);

	__declspec(dllexport) void OpenSniffer(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
	__declspec(dllexport) void Request(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
	__declspec(dllexport) void GetResponse(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
	__declspec(dllexport) void SetFingerprint(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
	__declspec(dllexport) void GetHeaders(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
	__declspec(dllexport) void SetTimeout(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
	__declspec(dllexport) void Reset(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
	__declspec(dllexport) void SetHeader(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
	__declspec(dllexport) void ClearHeaders(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
	__declspec(dllexport) void SetProxy(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
	__declspec(dllexport) void SaveCookies(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
	__declspec(dllexport) void GetStatus(char* InputJson, ResizeFunction AllocateSpace, void* AllocateData, void* DllData, void* ThreadData, unsigned int ThreadId, bool* NeedToStop, bool* WasError);
}
