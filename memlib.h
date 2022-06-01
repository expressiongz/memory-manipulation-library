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
	template<typename... var_arg>
	void dbg_log(std::string dbg_message, var_arg... fmt_args) const;
	template<typename... var_arg>
	void dbg_err(std::string dbg_error, var_arg... fmt_args) const;
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
		mem_manip_lib::dbg_log("initialized " , this->dllname);
	}
};
template<std::size_t sz>
bool mem_manip_lib::mem_set_bytes(const std::size_t instr_sz, const std::array<std::uint8_t, sz>& byte_arr) {
	mem_manip_lib::dbg_log("byte patch function called, attempting to patch bytes.");
	if (byte_arr.size() > instr_sz) {
		mem_manip_lib::dbg_err("amount of bytes provided by user exceeds amount of bytes to be patched.");
		return false;
	}
	mem_manip_lib::mem_set_nop(instr_sz);
	std::memcpy(this->mem_address, byte_arr.data(), byte_arr.size());
	mem_manip_lib::dbg_log("set %u user-provided bytes at virtual address 0x%p", byte_arr.size(), this->mem_address);
	mem_manip_lib::dbg_log("user provided bytes %X", byte_arr);
	mem_manip_lib::unset_rwx(instr_sz);
	return true;
}
template<typename ...var_arg>
void mem_manip_lib::dbg_log(std::string dbg_message, var_arg... fmt_args) const {
	std::string dbg_err_prefix = "[+] ";
	std::printf((dbg_err_prefix + dbg_message).c_str(), fmt_args...);
	std::cout << "\n";
}

template<typename ...var_arg>
void mem_manip_lib::dbg_err(std::string dbg_message, var_arg... fmt_args) const {
	std::string dbg_err_prefix = "[!] ";
	std::printf((dbg_err_prefix + dbg_message).c_str(), fmt_args...);
	std::cout << "\n";
}