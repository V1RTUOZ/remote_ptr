#pragma once
#include <cstdint>

class IIOManager {
public:
	virtual ~IIOManager() = default;
	virtual bool read(uintptr_t addr, size_t size, void* data) = 0;
	virtual bool write(uintptr_t addr, size_t size, void* data) = 0;
};
