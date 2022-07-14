#include "main\mem_manip.hpp"
#include "exception_handler_hook/exception_handler_hook.hpp"
#include <Windows.h>
#include <thread>


void main_thread(const HMODULE mod_handle) 
{
	mem_manip_lib memory_lib{ mod_handle };
	memory_lib.unload_dll();
}


bool __stdcall DllMain(const HMODULE mod_handle, const std::uint32_t reason, void const*)
{
	if ( reason == DLL_PROCESS_ATTACH ) {
		std::thread(main_thread, mod_handle).detach();
	}
	return true;
}
