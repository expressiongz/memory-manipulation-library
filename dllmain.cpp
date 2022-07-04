#include "mem_manip.hpp"
#include "includes.hpp"
#include <thread>

void main_thread(HMODULE mod_handle, const std::string_view dll_name) 
{
	mem_manip_lib memory_lib(mod_handle, dll_name);
	memory_lib.unload();
}

bool __stdcall DllMain(HMODULE mod_handle, std::uint32_t reason , void*)
{
	if ( reason == DLL_PROCESS_ATTACH ) {
		std::thread(main_thread , mod_handle , "xprssn").detach();
	}
	return true;
}
