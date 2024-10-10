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
	// Variables to store start and end times
	LARGE_INTEGER frequency, start, end;
	double elapsedTime;

	// Get the frequency of the high-resolution performance counter
	QueryPerformanceFrequency(&frequency);

	// Get the start time
	QueryPerformanceCounter(&start);

	// Get the current process ID
	DWORD processID = GetCurrentProcessId();

	// Get parent process ID
	DWORD parentProcessID = GetParentProcessID(processID);

	// Print the process ID and parent process ID
	printf("[CHILD]: Process ID: %lu\n", processID);
	printf("[CHILD]: Parent Process ID: %lu\n", parentProcessID);

	// printf("[CHILD]: Child process is running...\n");

	// Sleep(120000); // Sleep for 120 seconds (2 minutes)

	// Loop 6 times @ 20 seconds each
	for (int i = 0; i < 6; i++)
    {
        Sleep(20000); // Sleep for 20 seconds
        printf("[CHILD]: Child process has woken up...\n");
    }
	printf("[CHILD]: Child process is done.\n");

	// Get the end time
	QueryPerformanceCounter(&end);

	// Calculate the elapsed time in seconds with 6 decimal places of accuracy
	elapsedTime = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

	// Print the elapsed time
	printf("Elapsed time: %.6f seconds\n", elapsedTime);

	return 0;
}
