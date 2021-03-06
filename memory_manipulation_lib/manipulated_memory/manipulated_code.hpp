#pragma once
#define MANIPULATED_CODE_FORCE_INLINE __forceinline
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


	explicit manipulated_code(std::span<std::uint8_t> _overwritten_code, std::span<std::uint8_t> _new_code, void* _code_address) : code_address(_code_address)
	{
		this->overwritten_code.reserve(_overwritten_code.size());

		std::memcpy(this->overwritten_code.data(), _overwritten_code.data(), _overwritten_code.size());

		this->new_code.reserve(_new_code.size());

		std::memcpy(this->new_code.data(), _new_code.data(), _new_code.size());

	}

	explicit manipulated_code() : failed(true) {

	}

};


MANIPULATED_CODE_FORCE_INLINE void manipulated_code::replace_overwritten_code(std::span<std::uint8_t> _overwritten_code)
{

	this->overwritten_code.reserve(_overwritten_code.size());
	std::memcpy(this->overwritten_code.data(), _overwritten_code.data(), _overwritten_code.size());
}

MANIPULATED_CODE_FORCE_INLINE void* manipulated_code::ret_address()
{
	return this->code_address;
}

MANIPULATED_CODE_FORCE_INLINE void manipulated_code::restore_code()
{
	std::memset(this->code_address, 0x90, this->new_code.size());
	std::memcpy(this->code_address, overwritten_code.data(), this->overwritten_code.size());

	this->overwritten_code = this->new_code;
	this->new_code = this->overwritten_code;
}

MANIPULATED_CODE_FORCE_INLINE void manipulated_code::overwrite_code() {

	std::memset(this->code_address, 0x90, this->new_code.size());
	std::memcpy(this->code_address, this->new_code.data(), this->new_code.size());	
}