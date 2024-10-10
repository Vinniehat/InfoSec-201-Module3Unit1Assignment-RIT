#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>


char CHILD_PROCESS_PATH[] = "H:\\Work\\Development\\School\\Rochester Institute of Technology\\InfoSec 201\\Module3Unit1Assignment\\x64\\Debug\\Module3Unit1AssignmentChild.exe";

DWORD GetParentProcessID(DWORD processID)
{
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapshot == INVALID_HANDLE_VALUE)
		return 0;

	PROCESSENTRY32 pe32;
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hSnapshot, &pe32))
	{
		do
		{
			if (pe32.th32ProcessID == processID)
			{
				CloseHandle(hSnapshot);
				return pe32.th32ParentProcessID;
			}
		} while (Process32Next(hSnapshot, &pe32));
	}

	CloseHandle(hSnapshot);
	return 0;
}

int main(void)
{
	// Get the current process ID
	DWORD processID = GetCurrentProcessId();

	// Get parent process ID
	DWORD parentProcessID = GetParentProcessID(processID);

	// Print the process ID and parent process ID
	printf("[PARENT]: Process ID: %lu\n", processID);
	printf("[PARENT]: Parent Process ID: %lu\n", parentProcessID);



	// Run the child process
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	if (!CreateProcessA(NULL, CHILD_PROCESS_PATH, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		printf("[PARENT]: CreateProcess failed (%lu)\n", GetLastError());
		return 1;
	} else
	{
		printf("[PARENT]: Child process is running...\n");

		WaitForSingleObject(pi.hProcess, INFINITE);

		// Wait for the child process to finish
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);

	}
	return 0;
}
