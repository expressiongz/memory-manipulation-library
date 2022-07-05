#pragma once

#include <span>
#include <Windows.h>
#include <vector>


class manipulated_code {
private:
	DWORD vp_pf{0};
	void* code_address{nullptr};
	std::span<std::uint8_t> old_code{};
	std::span<std::uint8_t> new_code{};
public:

	void restore_code();
	void overwrite_code();

	explicit manipulated_code(std::span<std::uint8_t> _old_code, std::span<std::uint8_t> _new_code, void* _code_address) 
		: old_code(_old_code), new_code(_new_code), code_address(_code_address) {
		
	}

};

