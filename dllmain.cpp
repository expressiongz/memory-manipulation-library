#include "memlib.h"
#include <Windows.h>
#include <iostream>
#include <thread>
void main_thread() {
	auto addr = static_cast<std::uintptr_t>(0xC73EF);
	std::vector<unsigned char> bytes = { 0xFF, 0x00 };
	memorypatch_lib memlib;
	memorypatch_lib::create_console();
	memlib.deflat(addr);
	memlib.set_bytes(2, bytes);
}

bool __stdcall DllMain(HMODULE, DWORD reason, void*)
{
	if (reason == DLL_PROCESS_ATTACH) {
		std::thread(main_thread).detach();
	}
	return 1;
}
