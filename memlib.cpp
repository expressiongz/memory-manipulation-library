#include "memlib.h"
#include <Windows.h>
#include <string>
void memorypatch_lib::create_console() {
	std::FILE* fp;
	AllocConsole();
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitleA("wooware");
}

void memorypatch_lib::deflat(const std::uintptr_t& address) {
	static auto base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr));
	this->addr = reinterpret_cast<std::uintptr_t*>(base+address);
}

void memorypatch_lib::set_nop(const std::size_t num_bytes) {
	DWORD old_protect;
	VirtualProtect(this->addr, num_bytes, PAGE_EXECUTE_READWRITE, &old_protect);
	std::memset(this->addr, 0x90, num_bytes);
	VirtualProtect(this->addr, num_bytes, old_protect, nullptr);
	std::cout << "Set " << num_bytes << " bytes to 0x90 (nop) " << "at address: 0x" << reinterpret_cast<std::uintptr_t>(this->addr) << "\n";
}

void memorypatch_lib::set_bytes(const std::size_t num_bytes, std::vector<unsigned char>& byte_arr) {
	DWORD old_protect;
	memorypatch_lib::set_nop(num_bytes);
	VirtualProtect(this->addr, num_bytes, PAGE_EXECUTE_READWRITE, &old_protect);
	std::memcpy(this->addr, byte_arr.data(), num_bytes);
	VirtualProtect(this->addr, num_bytes, old_protect, nullptr);
	std::cout << "Set " << num_bytes << " user-provided bytes at address: 0x" << reinterpret_cast<std::uintptr_t>(this->addr) << "\n";
}