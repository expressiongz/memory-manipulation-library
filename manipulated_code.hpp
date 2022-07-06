#pragma once

#include <span>
#include <Windows.h>
#include <vector>


class manipulated_code {
private:
	DWORD vp_pf{0};
	void* code_address{nullptr};
	std::vector<std::uint8_t> code_restore_buf{};
	std::vector<std::uint8_t> new_code{};
public:

	void set_new_code(bool overwrite, std::span<std::uint8_t> _new_code);
	void set_code_restore_buf(std::span<std::uint8_t> _code_restore_buf);
	void restore_code();

	void overwrite_code();
	void* ret_address();


	explicit manipulated_code(std::span<std::uint8_t> _code_restore_buf, std::span<std::uint8_t> _new_code, void* _code_address) 
	{
		this->code_restore_buf.reserve(_code_restore_buf.size());

		std::memcpy(this->code_restore_buf.data(), _code_restore_buf.data(), _code_restore_buf.size());

		this->new_code.reserve(_new_code.size());

		std::memcpy(this->new_code.data(), _new_code.data(), _new_code.size());

	}

};