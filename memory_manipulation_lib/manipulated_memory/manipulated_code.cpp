#include "manipulated_code.hpp"


void manipulated_code::replace_new_code(bool overwrite, std::span<std::uint8_t> _new_code)
{

	this->overwritten_code.reserve(this->new_code.size());

	std::memcpy(this->overwritten_code.data(), this->new_code.data(), this->new_code.size());

	this->new_code.reserve(_new_code.size());

	if (overwrite) std::memcpy(this->new_code.data(), _new_code.data(), _new_code.size());
}