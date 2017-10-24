#pragma once
#include "remote_ptr/iiomanager.h"
#include <windows.h>

class memory_manager : public IIOManager {

	HANDLE mhProcess;
public:
	memory_manager(HANDLE hProcess);
	virtual ~memory_manager() = default;
	virtual bool read(uintptr_t addr, size_t size, void* data) override;
	virtual bool write(uintptr_t addr, size_t size, void* data) override;
};
