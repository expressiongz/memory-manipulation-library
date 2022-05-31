#include "memlib.h"
#include "includes.h"
void mem_manip_lib::unload() {
	std::cout << "unloading " << this->dllname << " dll\n";
	mem_manip_lib::free_console();
	FreeLibrary(this->mod_handle);
}

bool mem_manip_lib::create_console() {
	if (this->console_attached == true) {
		std::cout << "console already attached.\n";
		return false;
	}
	this->console_attached = true;
	std::FILE* fp;
	AllocConsole();
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitleA(this->dllname.data());
	std::cout << this->dllname << " console allocated.\n";
	return true;
}

constexpr bool mem_manip_lib::free_console() {
	if (this->console_attached) {
		std::cout << "free'd the console from the process. feel free to close it.\n";
		FreeConsole();
		this->console_attached = false;
		return true;
	}
	return false;
}

void mem_manip_lib::reloc_rva(const std::uint32_t address) {
	static auto base = reinterpret_cast<std::uint32_t>(GetModuleHandleA(nullptr));
	this->mem_address = reinterpret_cast<void*>(base+address);
}

void mem_manip_lib::set_rwx(const std::size_t sz) {
	VirtualProtect(this->mem_address, sz, PAGE_EXECUTE_READWRITE, &(this->old_prot));
	std::printf("set rwx for %u bytes at virtual address: 0x%p\n", sz, this->mem_address);
}
void mem_manip_lib::unset_rwx(const std::size_t sz) {
	VirtualProtect(this->mem_address, sz, this->old_prot, nullptr);
	std::printf("unset rwx for %u bytes at virtual address: 0x%p\n", sz, this->mem_address);
	this->old_prot = 0;
}

void mem_manip_lib::mem_set_nop(const std::size_t sz) {
	std::cout << "nop byte patch function called, attempting to patch bytes to nop.\n";
	mem_manip_lib::set_rwx(sz);
	std::memset(this->mem_address, 0x90, sz);
	std::printf("set %u bytes to 0x90 at address: 0x%p\n", sz, this->mem_address);
}
bool mem_manip_lib::mem_tramp_hook(const std::size_t sz, const std::uint32_t func_address, const std::uint32_t hook_address, int& jmp_back_address) {
	std::cout << "trampoline hook function called, attempting a trampoline hook.\n";
	if (sz < 5) {
		std::printf("number of bytes too small for trampoline hook. number of bytes given: %u\n", sz);
		return false;
	}
	jmp_back_address = hook_address + sz;
	this->mem_address = reinterpret_cast<void*>(hook_address);
	auto rel_address = (func_address - hook_address) - 5;
	std::array<std::uint8_t, 5> tramp_hook_bytes = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
	*(reinterpret_cast<std::uint32_t*>(&tramp_hook_bytes[1])) = rel_address;
	mem_manip_lib::mem_set_bytes(sz, tramp_hook_bytes);
	std::cout << "trampoline hook created.\n";
	return true;
}
