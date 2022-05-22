#include "memlib.h"
#include "includes.h"
#include <thread>
void main_thread(HMODULE mod_handle, const std::string& dll_name) {
	memorypatch_lib memlib(mod_handle, dll_name);
	memlib.unload();
}

bool __stdcall DllMain(HMODULE mod_handle, DWORD reason, void*)
{
	if (reason == DLL_PROCESS_ATTACH) {
		std::thread(main_thread, mod_handle, "xenon").detach();
	}
	return true;
}
