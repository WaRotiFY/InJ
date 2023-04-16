#include <windows.h>
#include <iostream>
#include<TlHelp32.h>
using namespace std;

typedef void(__stdcall* InjectionComplete_t)(HANDLE proc);
const char* dllPath = "C:\\myDllLibrary.dll";
const char* processName = "example.exe";

int main(int argc, char* argv[])
{
	// �������� ������������� ��������
	DWORD processId = 0;
	HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processEntry;
	ZeroMemory(&processEntry, sizeof(processEntry));
	processEntry.dwSize = sizeof(processEntry);
	if (Process32First(snapshot, &processEntry))
	{
		do {
			if (strcmp(processEntry.szExeFile, processName) == 0)
			{
				processId = processEntry.th32ProcessID;
				break;
			}
		} while (Process32Next(snapshot, &processEntry));
	}
	CloseHandle(snapshot);
	if (processId == 0)
	{
		cout << "Process not found." << endl;
		return 1;
	}

	// ��������� ���������� DLL
	HMODULE dllModule = LoadLibrary(dllPath);
	if (dllModule == NULL)
	{
		cout << "Failed to load DLL." << endl;
		return 1;
	}

	// �������� ����� ������� ���������� �� �������� ��������������
	InjectionComplete_t injectionComplete = (InjectionComplete_t)GetProcAddress(dllModule, "InjectionComplete");
	if (injectionComplete == NULL)
	{
		cout << "Failed to get function address." << endl;
		return 1;
	}

	// ��������� �������
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (hProcess == NULL)
	{
		cout << "Failed to open process." << endl;
		return 1;
	}

	// �������� ������ ��� ���� � DLL-�����
	LPVOID dllPathAddress = VirtualAllocEx(hProcess, NULL, strlen(dllPath) + 1, MEM_COMMIT, PAGE_READWRITE);
	if (dllPathAddress == NULL)
	{
		cout << "Failed to allocate memory." << endl;
		CloseHandle(hProcess);
		return 1;
	}

	// ����� ���� � DLL-����� � ������ ��������
	if (!WriteProcessMemory(hProcess, dllPathAddress, dllPath, strlen(dllPath) + 1, NULL))
	{
		cout << "Failed to write to process memory." << endl;
		CloseHandle(hProcess);
		VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
		return 1;
	}

	// �������� ����� ������� LoadLibrary
	HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
	LPVOID loadLibraryAddress = GetProcAddress(kernel32, "LoadLibraryA");

	// ������� ��������� ����� � �������� � �������� ������� LoadLibrary
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)loadLibraryAddress, dllPathAddress, 0, NULL);
	if (hThread == NULL)
	{
		cout << "Failed to create remote thread." << endl;
		CloseHandle(hProcess);
		VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
		return 1;
	}
	injectionComplete(hProcess);
	// ������� ���������� ������
	WaitForSingleObject(hThread, INFINITE);

	// ����������� �������
	CloseHandle(hThread);
	VirtualFreeEx(hProcess, dllPathAddress, 0, MEM_RELEASE);
	CloseHandle(hProcess);

	// ��������� �� �������� ��������


	return 0;
}