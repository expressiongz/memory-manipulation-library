#include "memlib.h"
#include "includes.h"
#include <thread>

void main_thread( HMODULE mod_handle , const std::string_view dll_name ) {
	mem_manip_lib memlib( mod_handle , dll_name );
	memlib.set_va(0xE02128);
	std::cout << memlib.mem_read_string(5);
	memlib.unload();
}

bool __stdcall DllMain( HMODULE mod_handle , std::uint32_t reason , void* )
{
	if ( reason == DLL_PROCESS_ATTACH ) {
		std::thread( main_thread , mod_handle , "xprssn" ).detach();
	}
	return true;
}
