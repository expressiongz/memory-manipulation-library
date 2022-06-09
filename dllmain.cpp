#include "memlib.h"
#include "includes.h"
#include <thread>


void main_thread( HMODULE mod_handle , const std::string_view dll_name ) {
	mem_manip_lib memlib( mod_handle , dll_name, false );
	memlib.reloc_rva(0x1000);

	auto const bytes = memlib.mem_read_bytes < std::array< std::uint8_t,  6 > >();
	memlib.dbg_log("bytes read at relative virtual address 0x1000: %x", bytes);

	memlib.unload();
}

bool __stdcall DllMain( HMODULE mod_handle , std::uint32_t reason , void* )
{
	if ( reason == DLL_PROCESS_ATTACH ) {
		std::thread( main_thread , mod_handle , "south" ).detach();
	}
	return true;
}
