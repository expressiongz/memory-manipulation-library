#include "memlib.h"
#include "includes.h"
#include <thread>
void main_thread(HMODULE mod_handle, const std::string& dll_name) {
	auto addr = 0xC73EF;
	std::array<std::uint8_t, 2> bytes = {0xFF, 0x00};
	memorypatch_lib memlib(mod_handle, dll_name);
	memlib.deflat(addr);
	memlib.mem_set_bytes(2, bytes);
	memlib.unload();
}

bool __stdcall DllMain(HMODULE mod_handle, DWORD reason, void*)
{
	if (reason == DLL_PROCESS_ATTACH) {
		std::thread(main_thread, mod_handle, "xenon").detach();
	}
	return true;
}
