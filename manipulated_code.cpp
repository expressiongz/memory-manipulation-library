#include "manipulated_code.hpp"

void manipulated_code::restore_code() {
	std::memset(this->code_address, 0x90, this->new_code.size());
	std::memcpy(this->code_address, old_code.data(), this->old_code.size());

}

void manipulated_code::overwrite_code() {

	std::memset(this->code_address, 0x90, this->new_code.size());
	std::memcpy(this->code_address, this->new_code.data(), this->new_code.size());
}

void manipulated_code::set_new_code(std::span<std::uint8_t> _new_code) {

	this->old_code.reserve(this->new_code.size());
	
	std::memcpy(this->old_code.data(), this->new_code.data(), this->new_code.size());

	this->new_code.reserve(_new_code.size());

	std::memcpy(this->new_code.data(), _new_code.data(), _new_code.size());
}

void* manipulated_code::ret_address() {
	return this->code_address;
}