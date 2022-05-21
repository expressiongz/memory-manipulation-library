#pragma once
#include "includes.h"
class memorypatch_lib {
private:
	const std::string& dllname;
	DWORD old_prot = 0;
	HMODULE mod_handle;
	bool console_attached;
	std::uintptr_t* mem_address;
public:
	void unload();
	int create_console();
	int free_console();
	void deflat(const std::uintptr_t& mem_address);
	int set_rwx(const std::size_t num_bytes);
	int unset_rwx(const std::size_t num_bytes);
	void mem_set_nop(const std::size_t num_bytes);
	template<std::size_t sz>
	int mem_set_bytes(const std::size_t num_bytes, std::array<std::uint8_t, sz> byte_arr);
	int mem_set_val(const int& val) const;

	explicit memorypatch_lib(HMODULE hmod, const std::string& dll_name) : dllname(dll_name), mod_handle(hmod) {
		create_console();
		std::cout << "initialized " << this->dllname << "\n";
	}

};
template<std::size_t sz>
int memorypatch_lib::mem_set_bytes(const std::size_t instr_size, std::array<std::uint8_t, sz> byte_arr) {
	if (byte_arr.size() > instr_size) {
		std::cout << "amount of bytes provided by user exceeds amount of bytes to be replaced.\n";
		return 0;
	}
	memorypatch_lib::mem_set_nop(instr_size);
	std::memcpy(this->mem_address, byte_arr.data(), byte_arr.size());
	std::cout << "set " << byte_arr.size() << " user-provided bytes at address: 0x" << reinterpret_cast<std::uintptr_t>(this->mem_address) << "\n";
	memorypatch_lib::unset_rwx(instr_size);
	return 1;
}