#include "process.h"


process::process() : mHandle(INVALID_HANDLE_VALUE), mPid(-1), mpMemoryManager(nullptr)
{}


process::~process()
{
	if (mpMemoryManager)
		delete mpMemoryManager;
	close();
}

bool process::open(DWORD pid)
{
	mPid = pid;
	mHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	return (mHandle != INVALID_HANDLE_VALUE) && (mHandle != nullptr);
}

bool process::open(const char* pName)
{
	PROCESSENTRY32 procEntry;
	if (find(pName, &procEntry)) {
		return open(procEntry.th32ProcessID);
	}
	return false;
}


void process::close()
{
	CloseHandle(mHandle);
	mHandle = INVALID_HANDLE_VALUE;
}

void process::terminate(unsigned exitCode)
{
	TerminateProcess(mHandle, exitCode);
	close();
}

void process::enumerate_modules(std::function<bool(const MODULEENTRY32&)> functor) const
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, mPid);
	MODULEENTRY32 mEntry;
	mEntry.dwSize = sizeof(mEntry);
	Module32Next(hSnap, &mEntry);
	do {
		if (false == functor(mEntry)) {
			CloseHandle(hSnap);
			return;
		}
	} while (Module32Next(hSnap, &mEntry));
	CloseHandle(hSnap);
}

memory_manager& process::get_memory_manager()
{
	return (mpMemoryManager == nullptr) ? *(mpMemoryManager = new memory_manager(mHandle)) : *mpMemoryManager;
}

bool process::find(const char* pName, PROCESSENTRY32* pProcEntry)
{
	HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	pProcEntry->dwSize = sizeof(PROCESSENTRY32);
	do
		if (0 == _stricmp(pProcEntry->szExeFile, pName)) {
			CloseHandle(hSnap);
			return true;
		} while (Process32Next(hSnap, pProcEntry));
	CloseHandle(hSnap);
	return false;
}

