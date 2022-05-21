#include "memlib.h"
#include "includes.h"
void memorypatch_lib::unload() {
	std::cout << "unloading " << this->dllname << " dll\n";
	memorypatch_lib::free_console();
	FreeLibrary(this->mod_handle);
}

int memorypatch_lib::create_console() {
	if (this->console_attached == true) {
		std::cout << "console already attached.\n";
		return 0;
	}
	this->console_attached = true;
	std::FILE* fp;
	AllocConsole();
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitleA(this->dllname.c_str());
	std::cout << this->dllname << " console allocated.\n";
	return 1;
}

int memorypatch_lib::free_console() {
	if (this->console_attached) {
		std::cout << "free'd the console from the process. feel free to close it.\n";
		FreeConsole();
		this->console_attached = false;
		return 1;
	}
	return 0;
}

void memorypatch_lib::deflat(const std::uintptr_t& address) {
	static auto base = reinterpret_cast<std::uintptr_t>(GetModuleHandleA(nullptr));
	this->mem_address = reinterpret_cast<std::uintptr_t*>(base+address);
}

int memorypatch_lib::set_rwx(const std::size_t num_bytes) {
	VirtualProtect(this->mem_address, num_bytes, PAGE_EXECUTE_READWRITE, &(this->old_prot));
	std::cout << "set page_execute_readwrite for " << num_bytes << " at address: 0x" << reinterpret_cast<std::uintptr_t>(this->mem_address) << "\n";
	return 1;
}
int memorypatch_lib::unset_rwx(const std::size_t num_bytes) {
	VirtualProtect(this->mem_address, num_bytes, this->old_prot, nullptr);
	std::cout << "unset page_execute_readwrite for " << num_bytes << " at address: 0x" << reinterpret_cast<std::uintptr_t>(this->mem_address) << "\n";
	return 1;
}

void memorypatch_lib::mem_set_nop(const std::size_t num_bytes) {
	memorypatch_lib::set_rwx(num_bytes);
	std::memset(this->mem_address, 0x90, num_bytes);
	std::cout << "set " << num_bytes << " bytes to 0x90 (nop) " << "at address: 0x" << reinterpret_cast<std::uintptr_t>(this->mem_address) << "\n";
}

int memorypatch_lib::mem_set_val(const int& val) const {
	*(this->mem_address) = val;
	return 1;
}