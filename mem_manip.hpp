#pragma once

#include "includes.hpp"
#include "manipulated_code.hpp"
#include "manipulated_values.hpp"


class mem_manip_lib
{
private:
        
    std::uint32_t base{ reinterpret_cast<std::uint32_t>(GetModuleHandleA(nullptr)) };
    const std::string_view dllname{ nullptr };

    void* mem_address{ nullptr };
    bool console_attached{ false };

    HMODULE mod_handle{ nullptr };
    DWORD old_prot{ 0 };

public:

    template<typename stl_container_t>
    stl_container_t mem_read_bytes() const;

    template<typename value_t>
    void mem_set_data(value_t val) const;

    template<typename ret_t>
    ret_t mem_read_data() const;

    template<std::uint32_t sz>
    manipulated_code* mem_tramp_hook(const std::uint32_t func_addr);

    template<typename data_t, std::size_t sz>
    std::array<data_t, sizeof(data_t) * sz> mem_read_dyn() const;

    template<typename data_t>
    std::vector<data_t> mem_read_dyn(const std::uint32_t sz) const;

    std::uint32_t get_va() const;
    std::uint32_t get_rva() const;

    std::uint8_t mem_read_byte(bool signednesss) const;
    std::string mem_read_string(const std::uint32_t string_sz) const;

    std::vector< std::uint8_t > mem_read_func_bytes() const;

    void reloc_rva(const std::uint32_t mem_address);
    void set_va(const std::uint32_t mem_address);

    void set_page_flags(const std::size_t sz, DWORD new_page_flags);
    void restore_page_flags(const std::size_t sz);

    void mem_set_nop(const std::size_t sz);

    bool mem_set_bytes(const std::size_t szbyte, std::span<std::uint8_t> byte_arr);
    bool mem_set_bytes(const std::size_t szbyte, std::uint8_t byte);

    void unload_dll();

    explicit mem_manip_lib(HMODULE hmod, const std::string_view dll_name)
        : dllname(dll_name), mod_handle(hmod)
    {

    }
 
};


template<typename value_t>
void mem_manip_lib::mem_set_data(value_t val) const
{
    *static_cast<value_t*>(this->mem_address) = val;
}

template<typename ret_t>
ret_t mem_manip_lib::mem_read_data() const 
{

    return *reinterpret_cast<ret_t*>(this->mem_address);

}

template<typename data_t, std::size_t sz>
std::array<data_t, sizeof(data_t) * sz> mem_manip_lib::mem_read_dyn() const 
{

    std::array<data_t, sizeof(data_t) * sz> data;

    for (auto idx = 0u; idx < sizeof(data_t) * sz; idx += sizeof(data_t)) {

        data[(idx / sizeof(data_t))] = *reinterpret_cast<data_t*>(reinterpret_cast<std::uint32_t>(this->mem_address) + idx);

    }

    return data;
}

template<typename data_t>
std::vector<data_t> mem_manip_lib::mem_read_dyn(const std::uint32_t sz) const 
{
    std::vector<data_t> data;

    for (auto idx = 0u; idx < sz; idx += sizeof(data_t)) {
        data.push_back(*reinterpret_cast<data_t*>(reinterpret_cast<std::uint32_t>(this->mem_address) + idx));
    }

    return data;

}

template< typename stl_container_t >
stl_container_t mem_manip_lib::mem_read_bytes() const
{
    auto bytes_read = stl_container_t();

    for (auto k = 0u; k < bytes_read.size(); ++k)
    {
        bytes_read[k] = (*(reinterpret_cast<std::uint8_t*>(reinterpret_cast<std::uint32_t>(this->mem_address) + k)));
    }

    return bytes_read;
}

template<std::uint32_t sz>
manipulated_code* mem_manip_lib::mem_tramp_hook(const std::uint32_t new_func)
{
    constexpr auto jmp_instr_size = 5;

    if (sz < jmp_instr_size)
    {
        return nullptr;
    }

    const auto overwritten_bytes = mem_read_bytes<std::array<std::uint8_t, sz>>();
    const auto new_bytes = std::to_array<std::uint8_t>({0xE9, 0x00, 0x00, 0x00, 0x00});

    const auto rel_addr = (new_func - reinterpret_cast<std::uint32_t>(this->mem_address)) - jmp_instr_size;

    *reinterpret_cast<std::uint32_t*>(new_bytes[1]) = rel_addr;

    std::memset(this->mem_address, 0x90, sz);
    std::memcpy(this->mem_address, new_bytes.data(), new_bytes.size());

    manipulated_code code_class(overwritten_bytes, new_bytes, this->mem_address);
    return &code_class;
}