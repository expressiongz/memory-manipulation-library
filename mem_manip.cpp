#include "mem_manip.hpp"
#include "manipulated_code.hpp"
#include "manipulated_values.hpp"

void mem_manip_lib::unload_dll()
{
    FreeLibrary(this->mod_handle);
}


void mem_manip_lib::reloc_rva(const std::uint32_t address)
{
    this->mem_address = reinterpret_cast< void* >(this->base + address);
}

void mem_manip_lib::set_va(const std::uint32_t address) {
    this->mem_address = reinterpret_cast<void*>(address);
}


std::uint32_t mem_manip_lib::get_va() const {
    return reinterpret_cast<std::uint32_t>(this->mem_address);
}

std::uint32_t mem_manip_lib::get_rva() const {
    return reinterpret_cast<std::uint32_t>(this->mem_address) - this->base;
}

void mem_manip_lib::set_page_flags(const std::size_t sz, DWORD new_page_flags)
{
    VirtualProtect(this->mem_address, sz, new_page_flags, &(this->old_prot));
}

void mem_manip_lib::mem_set_nop(const std::size_t sz)
{
    std::memset(this->mem_address, 0x90 , sz);
}

bool mem_manip_lib::mem_set_bytes(const std::size_t instr_sz, std::span<std::uint8_t> byte_arr) 
{

    if (byte_arr.size() > instr_sz)
    {
        return false;
    }

    mem_set_nop(instr_sz);
    std::memcpy(this->mem_address, byte_arr.data(), byte_arr.size());

    return true;
}


bool mem_manip_lib::mem_set_bytes(const std::size_t instr_sz, std::uint8_t byte)
{
    mem_set_nop(instr_sz);
    std::memset(this->mem_address, byte, instr_sz);
    return true;
}


std::uint8_t mem_manip_lib::mem_read_byte(bool signedness) const 
{
    if (signedness) 
    { 
        return *static_cast<std::int8_t*>(this->mem_address);
    }
    return *static_cast<std::uint8_t*>(this->mem_address);
}

std::string mem_manip_lib::mem_read_string(const std::uint32_t string_sz) const 
{
    auto read_string = std::string();
    for (auto idx = 0u; idx < string_sz; idx++) 
    {
        read_string.push_back(*reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uint32_t>(this->mem_address) + idx));
    }
    return read_string;
}


std::vector<std::uint8_t> mem_manip_lib::mem_read_func_bytes() const 
{
    auto bytes_read = std::vector< std::uint8_t >();
    auto const* curr_byte = reinterpret_cast<std::uint8_t const*>(this->mem_address);
    do 
    {
        bytes_read.push_back(*curr_byte);
        curr_byte += 1;

    } while (*(curr_byte + 1) != 0xCC || *(curr_byte + 2) != 0xCC || *(curr_byte + 3) != 0xCC);
    return bytes_read;
}
