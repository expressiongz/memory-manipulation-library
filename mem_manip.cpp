#include "mem_manip.hpp"
#include "manipulated_code.hpp"
#include "manipulated_values.hpp"

void mem_manip_lib::unload_dll() const
{
    FreeLibrary(this->lib_module_handle);
}

void mem_manip_lib::reloc_rva(const std::uint32_t address)
{
    this->memory_address = reinterpret_cast< void* >(this->base + address);
}

void mem_manip_lib::set_va(const std::uint32_t address) 
{
    this->memory_address = reinterpret_cast<void*>(address);
}

std::uint32_t mem_manip_lib::get_va() const 
{
    return reinterpret_cast<std::uint32_t>(this->memory_address);
}

std::uint32_t mem_manip_lib::get_rva() const 
{
    return reinterpret_cast<std::uint32_t>(this->memory_address) - this->base;
}

void mem_manip_lib::set_page_flags(const std::size_t sz, DWORD new_page_flags)
{
    VirtualProtect(this->memory_address, sz, new_page_flags, &vp_old_protection);
}

void mem_manip_lib::restore_page_flags(const std::size_t sz)
{
    VirtualProtect(this->memory_address, sz, vp_old_protection, &vp_old_protection);
}


std::uint8_t mem_manip_lib::mem_read_byte(bool signedness) const 
{
    if (signedness) 
    { 
        return *static_cast<std::int8_t*>(this->memory_address);
    }
    return *static_cast<std::uint8_t*>(this->memory_address);
}

std::string mem_manip_lib::mem_read_string(const std::size_t string_sz) const 
{
    auto read_string = std::string();
    for (auto idx = 0u; idx < string_sz; idx++) 
    {
        read_string.push_back(static_cast<std::uint8_t*>(this->memory_address)[idx]);
    }
    return read_string;
}


std::vector<std::uint8_t> mem_manip_lib::mem_read_func_bytes() const 
{
    auto bytes_read = std::vector< std::uint8_t >();
    auto const* curr_byte = static_cast<std::uint8_t*>(this->memory_address);
    do 
    {
        bytes_read.push_back(*curr_byte);
        curr_byte += 1;

    } while (*(curr_byte + 1) != 0xCC || *(curr_byte + 2) != 0xCC || *(curr_byte + 3) != 0xCC);
    return bytes_read;
}
