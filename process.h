#pragma once
#include <windows.h>
#include <TlHelp32.h>
#include <functional>
#include "memory_manager.h"

class process {
private:
	HANDLE mHandle;
	DWORD mPid;
	memory_manager* mpMemoryManager;
	static bool find(const char* pName, PROCESSENTRY32* pProcEntry);
public:
	process();
	~process();
	bool open(DWORD pid);
	bool open(const char* pName);
	void close();
	void terminate(unsigned exitCode = 0);
	void enumerate_modules(std::function<bool(const MODULEENTRY32&)> functor) const;

	memory_manager& get_memory_manager();

	DWORD pid() const
	{
		return mPid;
	}
};

