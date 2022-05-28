#pragma once
#include "includes.h"
class mem_manip_lib{
private:
	const std::string& dllname = 0;
	DWORD old_prot = 0;
	HMODULE mod_handle = 0;
	bool console_attached = false;
	void* mem_address = nullptr;
public:
	void unload();
	bool create_console();
	bool free_console();
	void deflat(const void*& mem_address);
	void set_rwx(const std::size_t& num_bytes);
	void unset_rwx(const std::size_t& num_bytes);
	void mem_set_nop(const std::size_t& num_bytes);
	bool mem_tramp_hook(const std::size_t& num_bytes, const void*& func_addr, const void*& hook_address);
	template<std::size_t sz>
	bool mem_set_bytes(const std::size_t& num_bytes, const std::array<std::uint8_t, sz>& byte_arr);
	explicit mem_manip_lib(HMODULE hmod, const std::string& dll_name) : dllname(dll_name), mod_handle(hmod) {
		create_console();
		std::cout << "alephnull memory manipulation library.\n";
		std::cout << "initialized " << this->dllname << "\n";
	}
};
template<std::size_t sz>
bool mem_manip_lib::mem_set_bytes(const std::size_t& instr_size, const std::array<std::uint8_t, sz>& byte_arr) {
	if (byte_arr.size() > instr_size) {
		std::cout << "amount of bytes provided by user exceeds amount of bytes to be replaced.\n";
		return false;
	}
	mem_manip_lib::mem_set_nop(instr_size);
	std::memcpy(this->mem_address, byte_arr.data(), byte_arr.size());
	std::printf("set %u user-provided bytes to address 0x%p\nuser-provided bytes: ", byte_arr.size(), this->mem_address);
	for (auto& i : byte_arr) {
		std::printf("%X", i);
	}
	std::cout << "\n";
	mem_manip_lib::unset_rwx(instr_size);
	return true;
}