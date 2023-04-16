#include <Windows.h>

// Функция экспорта
extern "C" __declspec(dllexport) void __stdcall InjectionComplete(HANDLE hProcess)
{
	MessageBoxA(NULL, "Injection complete!", "DLL Injection", MB_OK);
	DWORD offset = 0x0021FC64;
	int write = 8888;
	WriteProcessMemory(hProcess, (PBYTE*)offset, &write, sizeof(int), 0);
}
