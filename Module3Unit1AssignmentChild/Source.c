#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

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
	printf("[CHILD]: Process ID: %lu\n", processID);
	printf("[CHILD]: Parent Process ID: %lu\n", parentProcessID);

	printf("[CHILD]: Child process is running...\n");

	Sleep(120000); // Sleep for 120 seconds (2 minutes)
	printf("[CHILD]: Child process is done\n");

	return 0;
}
