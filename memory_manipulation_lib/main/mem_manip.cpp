#include "../main/mem_manip.hpp"

std::string mem_manip_lib::mem_read_string(const std::size_t string_sz) const 
{
    std::string read_string{};
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

    } while (*curr_byte != 0xC3);
    return bytes_read;
}
