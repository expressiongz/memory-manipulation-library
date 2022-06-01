#include "memlib.h"
#include "includes.h"
void mem_manip_lib::unload() {
	mem_manip_lib::dbg_log("unloading ", this->dllname, " dll");
	mem_manip_lib::free_console();
	FreeLibrary(this->mod_handle);
}




bool mem_manip_lib::create_console() {
	if (this->console_attached == true) {
		mem_manip_lib::dbg_err("console already allocated");
		return false;
	}
	this->console_attached = true;
	std::FILE* fp;
	AllocConsole();
	freopen_s(&fp, "CONOUT$", "w", stdout);
	SetConsoleTitleA(this->dllname.data());
	mem_manip_lib::dbg_log("allocated console.");
	return true;
}

constexpr bool mem_manip_lib::free_console() {
	if (this->console_attached) {
		mem_manip_lib::dbg_log("free'd the console from the process");
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
	mem_manip_lib::dbg_log("set rwx for %u bytes at virtual address 0x%p", sz, this->mem_address);
}
void mem_manip_lib::unset_rwx(const std::size_t sz) {
	VirtualProtect(this->mem_address, sz, this->old_prot, nullptr);
	mem_manip_lib::dbg_log("unset rwx for %u bytes at virtual address 0x%p", sz, this->mem_address);
	this->old_prot = 0;
}

void mem_manip_lib::mem_set_nop(const std::size_t sz) {
	mem_manip_lib::dbg_log("nop byte patch function called, attempting to patch bytes to nop.");
	mem_manip_lib::set_rwx(sz);
	std::memset(this->mem_address, 0x90, sz);
	mem_manip_lib::dbg_log("set %u bytes to 0x90 at virtual address 0x%p", sz, this->mem_address);
}
bool mem_manip_lib::mem_tramp_hook(const std::size_t sz, const std::uint32_t func_address, const std::uint32_t hook_address, int& jmp_back_address) {
	mem_manip_lib::dbg_log("trampoline hook function called, attempting a trampoline hook.");
	if (sz < 5) {
		mem_manip_lib::dbg_err("number of bytes too small for trampoline hook. number of bytes given: %u", sz);
		return false;
	}
	jmp_back_address = hook_address + sz;
	this->mem_address = reinterpret_cast<void*>(hook_address);
	auto rel_address = (func_address - hook_address) - 5;
	std::array<std::uint8_t, 5> tramp_hook_bytes = { 0xE9, 0x00, 0x00, 0x00, 0x00 };
	*(reinterpret_cast<std::uint32_t*>(&tramp_hook_bytes[1])) = rel_address;
	mem_manip_lib::mem_set_bytes(sz, tramp_hook_bytes);
	mem_manip_lib::dbg_log("trampoline hook created at virtual address 0x%p", this->mem_address);
	return true;
}
