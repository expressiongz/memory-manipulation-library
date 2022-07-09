#pragma once

#include <span>
#include <Windows.h>
#include <vector>


class manipulated_code 
{
private:
	DWORD vp_pf{0};
	void* code_address{nullptr};
	bool failed{ false };
	std::vector<std::uint8_t> overwritten_code{};
	std::vector<std::uint8_t> new_code{};
public:

	void replace_new_code(bool overwrite, std::span<std::uint8_t> _new_code);
	void replace_overwritten_code(std::span<std::uint8_t> _overwritten_code);
	void restore_code();

	void overwrite_code();
	void* ret_address();


	explicit manipulated_code(std::span<std::uint8_t> _overwritten_code, std::span<std::uint8_t> _new_code, void* _code_address)
	{
		this->overwritten_code.reserve(_overwritten_code.size());

		std::memcpy(this->overwritten_code.data(), _overwritten_code.data(), _overwritten_code.size());

		this->new_code.reserve(_new_code.size());

		std::memcpy(this->new_code.data(), _new_code.data(), _new_code.size());

	}

	explicit manipulated_code() {
		this->failed = true;
	}

};