#include "manipulated_code.hpp"

void manipulated_code::restore_code() {
	std::memset(this->code_address, 0x90, this->new_code.size());
	std::memcpy(this->code_address, code_restore_buf.data(), this->code_restore_buf.size());

	this->code_restore_buf = this->new_code;
	this->new_code = this->code_restore_buf;
}

void manipulated_code::overwrite_code() {

	std::memset(this->code_address, 0x90, this->new_code.size());
	std::memcpy(this->code_address, this->new_code.data(), this->new_code.size());
}

void manipulated_code::set_new_code(bool overwrite, std::span<std::uint8_t> _new_code) {

	this->code_restore_buf.reserve(this->new_code.size());
	
	std::memcpy(this->code_restore_buf.data(), this->new_code.data(), this->new_code.size());

	this->new_code.reserve(_new_code.size());

	if (overwrite) std::memcpy(this->new_code.data(), _new_code.data(), _new_code.size());
}

void manipulated_code::set_code_restore_buf(std::span<std::uint8_t> _code_restore_buf) {

	this->code_restore_buf.reserve(_code_restore_buf.size());
	std::memcpy(this->code_restore_buf.data(), _code_restore_buf.data(), _code_restore_buf.size());
}

void* manipulated_code::ret_address() {
	return this->code_address;
}