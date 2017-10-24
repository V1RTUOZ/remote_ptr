#include "memory_manager.h"

bool memory_manager::read(uintptr_t addr, size_t size, void* data)
{
	SIZE_T nReaded;
	BOOL status = ReadProcessMemory(mhProcess, reinterpret_cast<LPVOID>(addr), data, size, &nReaded);
	return status && nReaded == size;
}

bool memory_manager::write(uintptr_t addr, size_t size, void* data)
{
	SIZE_T nWritten;
	BOOL status = WriteProcessMemory(mhProcess, reinterpret_cast<LPVOID>(addr), data, size, &nWritten);
	return status && nWritten == size;
}

memory_manager::memory_manager(HANDLE hProcess) : mhProcess(hProcess)
{}
