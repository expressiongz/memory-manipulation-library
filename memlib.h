#pragma once
#include <iostream>
#include <vector>
class memorypatch_lib {
private: 
	std::uintptr_t* addr;
public:
	static void create_console();
	void deflat(const std::uintptr_t& address);
	void set_nop(const std::size_t num_bytes);
	void set_bytes(const std::size_t num_bytes, std::vector<unsigned char>& byte_arr);
};