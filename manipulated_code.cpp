#include "manipulated_code.hpp"

void manipulated_code::restore_code() {

	VirtualProtect(this->code_address, this->new_code.size(), PAGE_EXECUTE_READWRITE, &(this->vp_pf));

	std::memset(this->code_address, 0x90, this->new_code.size());

	std::memcpy(this->code_address, old_code.data(), this->old_code.size());

	VirtualProtect(this->code_address, this->new_code.size(), this->vp_pf, &(this->vp_pf));
}

void manipulated_code::overwrite_code() {

	VirtualProtect(this->code_address, this->new_code.size(), PAGE_EXECUTE_READWRITE, &(this->vp_pf));

	std::memset(this->code_address, 0x90, this->new_code.size());

	std::memcpy(this->code_address, this->new_code.data(), this->new_code.size());

	VirtualProtect(this->code_address, this->new_code.size(), this->vp_pf, &(this->vp_pf));
}