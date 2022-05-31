#include "memlib.h"
#include "includes.h"
#include <thread>
auto jmp_back_address = 0;
void _declspec(naked) hook_func() {
	_asm {
		add [eax], 100
		lea eax, [esp + 0x1C]
		jmp [jmp_back_address]
	}
}


void main_thread(HMODULE mod_handle, const std::string_view dll_name) {
	mem_manip_lib memlib(mod_handle, dll_name);
	auto address = 0x4C73EF;
	memlib.mem_tramp_hook(6, reinterpret_cast<std::uint32_t>(&hook_func), 0x4C73EF, jmp_back_address);
}

bool __stdcall DllMain(HMODULE mod_handle, std::uint32_t reason, void*)
{
	if (reason == DLL_PROCESS_ATTACH) {
		std::thread(main_thread, mod_handle, "wooware").detach();
	}
	return true;
}
