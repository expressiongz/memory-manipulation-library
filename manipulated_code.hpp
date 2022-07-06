#pragma once

#include <span>
#include <Windows.h>
#include <vector>


class manipulated_code {
private:
	DWORD vp_pf{0};
	void* code_address{nullptr};
	std::vector<std::uint8_t> old_code{};
	std::vector<std::uint8_t> new_code{};
public:

	void set_new_code(std::span<std::uint8_t> _new_code);
	void restore_code();

	void overwrite_code();
	void* ret_address();


	explicit manipulated_code(std::span<std::uint8_t> _old_code, std::span<std::uint8_t> _new_code, void* _code_address) 
	{
		this->old_code.reserve(_old_code.size());

		std::memcpy(this->old_code.data(), _old_code.data(), _old_code.size());

		this->new_code.reserve(_new_code.size());

		std::memcpy(this->new_code.data(), _new_code.data(), _new_code.size());

	}

};