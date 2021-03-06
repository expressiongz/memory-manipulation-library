#pragma once
#define MEM_FORCE_INLINE __forceinline
#include "includes.hpp"
#include "../manipulated_memory/manipulated_code.hpp"
#include "../manipulated_memory/manipulated_values.hpp"


class mem_manip_lib
{
private:
    std::uint32_t base{ reinterpret_cast<std::uint32_t>(GetModuleHandleA(nullptr)) };

    void* memory_address{ nullptr };
    bool console_attached{ false };

    HMODULE lib_module_handle{ nullptr };
    DWORD vp_old_protection{ 0 };

public:
    template<std::size_t sz>
    manipulated_code mem_tramp_hook(const std::uint32_t func_addr) const;

    template<std::size_t sz>
    manipulated_code mem_set_byte(std::uint8_t byte) const;

    template<std::size_t sz>
    manipulated_code mem_set_bytes(std::span<std::uint8_t> bytes) const;

    template<typename stl_container_t>
    stl_container_t mem_read_bytes() const;

    template<typename value_t>
    manipulated_data<value_t> mem_set_data(value_t val) const;

    template<typename ret_t>
    ret_t mem_read_data() const;

    template<typename data_t, std::size_t sz>
    std::array<data_t, sizeof(data_t) * sz> mem_read_dyn() const;

    template<typename data_t>
    std::vector<data_t> mem_read_dyn(const std::size_t sz) const;

    template<typename value_t, std::size_t sz>
    manipulated_array_data<value_t, sz> mem_set_dyn(value_t val) const;

    template<std::size_t sz>
    manipulated_code mem_set_nop() const;

    std::uint32_t get_va() const;
    std::uint32_t get_rva() const;

    std::string mem_read_string(const std::size_t string_sz) const;
    std::uint8_t mem_read_byte(bool signednesss) const;

    std::vector<std::uint8_t> mem_read_func_bytes() const;

    void reloc_rva(const std::uint32_t mem_address);
    void set_va(const std::uint32_t mem_address);

    void set_page_flags(const std::size_t sz, DWORD new_page_flags);
    void restore_page_flags(const std::size_t sz);
    void unload_dll() const;

    explicit mem_manip_lib(HMODULE hmod)
        :lib_module_handle(hmod)
    {
    }
};


MEM_FORCE_INLINE void mem_manip_lib::unload_dll() const
{
    FreeLibrary(this->lib_module_handle);
}

MEM_FORCE_INLINE void mem_manip_lib::reloc_rva(const std::uint32_t address)
{
    this->memory_address = reinterpret_cast<void*>(this->base + address);
}

MEM_FORCE_INLINE void mem_manip_lib::set_va(const std::uint32_t address)
{
    this->memory_address = reinterpret_cast<void*>(address);
}

MEM_FORCE_INLINE std::uint32_t mem_manip_lib::get_va() const
{
    return reinterpret_cast<std::uint32_t>(this->memory_address);
}

MEM_FORCE_INLINE std::uint32_t mem_manip_lib::get_rva() const
{
    return reinterpret_cast<std::uint32_t>(this->memory_address) - this->base;
}

MEM_FORCE_INLINE void mem_manip_lib::set_page_flags(const std::size_t sz, DWORD new_page_flags)
{
    VirtualProtect(this->memory_address, sz, new_page_flags, &vp_old_protection);
}

MEM_FORCE_INLINE void mem_manip_lib::restore_page_flags(const std::size_t sz)
{
    VirtualProtect(this->memory_address, sz, vp_old_protection, &vp_old_protection);
}


MEM_FORCE_INLINE std::uint8_t mem_manip_lib::mem_read_byte(bool signedness) const
{
    if (signedness)
    {
        return *static_cast<std::int8_t*>(this->memory_address);
    }
    return *static_cast<std::uint8_t*>(this->memory_address);
}


template<typename ret_t>
MEM_FORCE_INLINE ret_t mem_manip_lib::mem_read_data() const
{
    return *static_cast<ret_t*>(this->memory_address);
}

template<typename value_t>
manipulated_data<value_t> mem_manip_lib::mem_set_data(value_t val) const
{
    const auto read_data = mem_read_data<value_t>();

    manipulated_data data(read_data, val, this->memory_address);
    *static_cast<value_t*>(this->memory_address) = val;
    return data;

}

template<typename data_t, std::size_t sz>
std::array<data_t, sizeof(data_t) * sz> mem_manip_lib::mem_read_dyn() const 
{

    std::array<data_t, sizeof(data_t) * sz> data{};

    for (auto idx = 0u; idx < sizeof(data_t) * sz; idx++) {

        data[idx] = static_cast<data_t*>(this->memory_address)[idx];
    }

    return data;
}

template<typename data_t>
std::vector<data_t> mem_manip_lib::mem_read_dyn(const std::size_t sz) const 
{
    std::vector<data_t> data{};

    for (auto idx = 0u; idx < sz; idx++) {
        data.push_back(static_cast<data_t*>(this->memory_address)[idx]);
    }

    return data;
}

template<typename value_t, std::size_t sz>
manipulated_array_data<value_t, sz> mem_manip_lib::mem_set_dyn(value_t val) const 
{
    const auto read_array = mem_read_dyn<value_t, sz>();

    for (auto idx = 0u; idx < sz; idx++) {
        static_cast<value_t*>(this->memory_address)[idx] = val;
    }
    
    const auto new_array = mem_read_dyn<value_t, sz>(); 

    return manipulated_array_data<value_t, sz>(read_array, new_array, this->memory_address);
}

template<typename stl_container_t>
stl_container_t mem_manip_lib::mem_read_bytes() const
{
    stl_container_t bytes_read{};

    for (auto idx = 0u; idx < bytes_read.size(); idx++)
    {
        bytes_read[idx] = static_cast<std::uint8_t*>(this->memory_address)[idx];
    }
    return bytes_read;
}

template<std::size_t sz>
manipulated_code mem_manip_lib::mem_set_nop() const
{
    return mem_set_byte<sz>(0x90);
}

template<std::size_t sz>
manipulated_code mem_manip_lib::mem_set_bytes(std::span<std::uint8_t> bytes) const
{
    if (bytes.size() > sz)
    {
        return manipulated_code();
    }

    auto read_bytes = mem_read_bytes<std::array<std::uint8_t, sz>>();
    manipulated_code code(read_bytes, bytes, this->memory_address);

    mem_set_nop<sz>();
    std::memcpy(this->memory_address, bytes.data(), bytes.size());

    return code;
}

template<std::size_t sz>
manipulated_code mem_manip_lib::mem_set_byte(std::uint8_t byte) const
{
    auto read_bytes = mem_read_bytes<std::array<std::uint8_t, sz>>();
    std::array<std::uint8_t, sz> new_bytes{};

    new_bytes.fill(byte);
    manipulated_code code(read_bytes, new_bytes, this->memory_address);

    std::memset(this->memory_address, byte, sz);
    return code;
}

template<std::size_t sz>
manipulated_code mem_manip_lib::mem_tramp_hook(const std::uint32_t new_func) const
{
    constexpr auto jmp_instr_size = 5;

    if (sz < jmp_instr_size)
    {
        return manipulated_code();
    }

    const auto rel_addr = (new_func - reinterpret_cast<std::uint32_t>(this->memory_address)) - jmp_instr_size;

    auto overwritten_bytes = mem_read_bytes<std::array<std::uint8_t, sz>>();
    auto new_bytes = std::to_array<std::uint8_t>({ 0xE9, 0x00, 0x00, 0x00, 0x00 });

    *reinterpret_cast<std::uint32_t*>(&new_bytes[1]) = rel_addr;
    
    mem_set_nop<sz>();
    mem_set_bytes<sz>(new_bytes);

    manipulated_code code_class(overwritten_bytes, new_bytes, this->memory_address);
    return code_class;
}

