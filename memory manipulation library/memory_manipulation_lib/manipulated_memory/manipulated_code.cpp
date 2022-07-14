#include "manipulated_code.hpp"

void manipulated_code::restore_code() 
{
	std::memset(this->code_address, 0x90, this->new_code.size());
	std::memcpy(this->code_address, overwritten_code.data(), this->overwritten_code.size());

	this->overwritten_code = this->new_code;
	this->new_code = this->overwritten_code;
}

void manipulated_code::overwrite_code() {

	std::memset(this->code_address, 0x90, this->new_code.size());
	std::memcpy(this->code_address, this->new_code.data(), this->new_code.size());
}

void manipulated_code::replace_new_code(bool overwrite, std::span<std::uint8_t> _new_code) 
{

	this->overwritten_code.reserve(this->new_code.size());
	
	std::memcpy(this->overwritten_code.data(), this->new_code.data(), this->new_code.size());

	this->new_code.reserve(_new_code.size());

	if (overwrite) std::memcpy(this->new_code.data(), _new_code.data(), _new_code.size());
}

void manipulated_code::replace_overwritten_code(std::span<std::uint8_t> _overwritten_code) 
{

	this->overwritten_code.reserve(_overwritten_code.size());
	std::memcpy(this->overwritten_code.data(), _overwritten_code.data(), _overwritten_code.size());
}

void* manipulated_code::ret_address() 
{
	return this->code_address;
}