#pragma once
#include "includes.h"
class mem_manip_lib{
private:
	const std::string_view dllname = nullptr;
	DWORD old_prot = 0;
	HMODULE mod_handle = nullptr;
	bool console_attached = false;
	void* mem_address = nullptr;
public:
	void unload();
	bool create_console();
	constexpr bool free_console();
	void reloc_rva(const std::uint32_t mem_address);
	void set_rwx(const std::size_t num_bytes);
	void unset_rwx(const std::size_t num_bytes);
	void mem_set_nop(const std::size_t num_bytes);
	bool mem_tramp_hook(const std::size_t num_bytes, const std::uint32_t func_addr, const std::uint32_t hook_address, int& jmp_back_address);
	template<std::size_t sz>
	bool mem_set_bytes(const std::size_t num_bytes, const std::array<std::uint8_t, sz>& byte_arr);
	explicit mem_manip_lib(HMODULE hmod, const std::string_view dll_name) : dllname(dll_name), mod_handle(hmod) {
		create_console();
		std::cout << "expgz memory manipulation library.\n";
		std::cout << "initialized " << this->dllname << "\n";
	}
};
template<std::size_t sz>
bool mem_manip_lib::mem_set_bytes(const std::size_t instr_sz, const std::array<std::uint8_t, sz>& byte_arr) {
	std::cout << "byte patch function called, attempting to patch bytes.\n";
	if (byte_arr.size() > instr_sz) {
		std::cout << "amount of bytes provided by user exceeds amount of bytes to be patched.\n";
		return false;
	}
	mem_manip_lib::mem_set_nop(instr_sz);
	std::memcpy(this->mem_address, byte_arr.data(), byte_arr.size());
	std::printf("set %u user-provided bytes to address 0x%p\nuser-provided bytes: %X\n", byte_arr.size(), this->mem_address, byte_arr);
	mem_manip_lib::unset_rwx(instr_sz);
	return true;
}