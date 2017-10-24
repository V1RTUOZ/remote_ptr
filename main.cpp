#include "remote_ptr/remote_ptr.h"
#include "process.h"
#include <thread>
#include <iostream>
#include <iomanip>

namespace offsets {
	constexpr ptrdiff_t dwEntityList = 0x4A8D1AC;
	constexpr ptrdiff_t dwClientState = 0x5A5344;
	constexpr ptrdiff_t dwClientState_State = 0x108;

	constexpr ptrdiff_t m_bDormant = 0xE9;
	constexpr ptrdiff_t m_bSpotted = 0x939;
}

void print_pointer(const char *szName, uintptr_t nValue)
{
	std::cout << szName << " - 0x" << std::hex << std::setfill('0') << std::setw(8) << nValue << std::endl;
}

int main()
{
	process csgo;
	uintptr_t engine = 0, client = 0;
	// open csgo process
	if (!csgo.open("csgo.exe")) {
		std::cerr << "csgo.exe not found" << std::endl;
		return EXIT_FAILURE;
	}

	// set iomanager pointer (important)
	remote_ptr_base::set_io_manager(&csgo.get_memory_manager());

	// wait for modules
	while (!client || !engine) {
		csgo.enumerate_modules([&engine, &client](const MODULEENTRY32& module) -> bool {
			if (0 == _stricmp(module.szModule, "engine.dll"))
				engine = reinterpret_cast<uintptr_t>(module.modBaseAddr);
			if (0 == _stricmp(module.szModule, "client.dll"))
				client = reinterpret_cast<uintptr_t>(module.modBaseAddr);

			return true;
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	print_pointer("client.dll", client);
	print_pointer("engine.dll", engine);

	const size_t entitySize = 0x10;
	const size_t MaxClients = 64;
	const uint32_t SIGNONSTATE_FULL = 6;

	try {
		// initializing remote pointers to ClientState and ClientState_State
		remote_ptr<uintptr_t> rpClientState(engine + offsets::dwClientState);
		remote_ptr<uint32_t> rpClientState_State(*rpClientState + offsets::dwClientState_State);
		print_pointer("ClientState", rpClientState.addr());
		print_pointer("ClientState_State", rpClientState_State.addr());
		for (;;) {
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			// check for player signed in
			if (*rpClientState_State != SIGNONSTATE_FULL) continue;

			for (size_t i = 1; i < MaxClients; ++i) {
				// initializing entoty pointer
				remote_ptr<uintptr_t> rpEntity(client + offsets::dwEntityList + (i * entitySize));
				// checking if entity have valid pointer and is not dormant
				if (!(*rpEntity)) continue;
				if (*remote_ptr<bool>(*rpEntity + offsets::m_bDormant)) continue;

				// writing bSpotted (showiing player on radar)
				*(remote_ptr<bool>(*rpEntity + offsets::m_bSpotted)) = true;
			}
		}
	} catch (access_error &e) {
		std::cerr << "Exception: " << e.what() << std::endl;
	}
	return EXIT_SUCCESS;
}
