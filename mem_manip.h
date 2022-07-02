#pragma once
#include "includes.h"

class mem_manip_lib 
{
private:
    std::uint32_t base = reinterpret_cast<std::uint32_t>(GetModuleHandleA(nullptr));
    const std::string_view dllname = nullptr;
    void* mem_address = nullptr;

    HMODULE mod_handle = nullptr;
    DWORD old_prot = 0;

    bool console_attached = false;
public:
    void unload();

    template< typename...var_arg >
    void dbg_log( std::string const& dbg_message , var_arg...fmt_args ) const;

    template< typename... var_arg >
    void dbg_err( std::string const& dbg_error , var_arg...fmt_args ) const;

    template< typename stl_container_t >
    stl_container_t mem_read_bytes() const;

    template<typename value_t>
    void mem_set_val(value_t val) const;

    template<typename ret_t>
    ret_t mem_read_val() const;

    std::uint32_t get_va() const;
    std::uint32_t get_rva() const;

    std::uint8_t mem_read_byte(bool signednesss) const;
    std::string mem_read_string(const std::uint32_t string_sz) const;

    std::vector< std::uint8_t > mem_read_func_bytes() const;
    std::uint32_t mem_tramp_hook(const std::uint32_t func_addr, const std::uint32_t instr_sz);

    bool alloc_console();
    bool free_console();

    void reloc_rva( const std::uint32_t mem_address );
    void set_va(const std::uint32_t mem_address);


    void set_rwx( const std::size_t sz );
    void unset_rwx( const std::size_t sz );

    void mem_set_nop( const std::size_t sz );

    bool mem_set_bytes( const std::size_t szbyte, std::span< std::uint8_t > byte_arr );
    bool mem_set_bytes(const std::size_t szbyte, std::uint8_t byte);

    explicit mem_manip_lib( HMODULE hmod , const std::string_view dll_name , bool alloc_console_f = false) 
        : dllname(dll_name), mod_handle(hmod) 
    {
        if ( alloc_console_f ) 
        {   
            alloc_console();
            dbg_log( "\ninitialized %s\n", this->dllname );
        }
        else if (GetConsoleWindow()) 
        {
            dbg_log("\ninitialized %s\n", this->dllname);
        }
    }
};
    
template< typename stl_container_t >
stl_container_t mem_manip_lib::mem_read_bytes() const 
{
    auto bytes_read = stl_container_t();

    for ( auto k = 0u; k < bytes_read.size(); ++k )
    {
        bytes_read[k] = ( * ( reinterpret_cast< std::uint8_t* >( reinterpret_cast< std::uint32_t >( this->mem_address ) + k) ) );
    }

    return bytes_read;
}

template< typename... var_arg >
void mem_manip_lib::dbg_log( std::string const& dbg_message , var_arg... fmt_args ) const 
{
    std::string dbg_err_prefix = "[+] ";
    std::printf( ( dbg_err_prefix + dbg_message ).c_str() , fmt_args... );
}


template< typename... var_arg >
void mem_manip_lib::dbg_err( std::string const& dbg_message , var_arg... fmt_args ) const 
{
    std::string dbg_err_prefix = "[!] ";
    std::printf( ( dbg_err_prefix + dbg_message ).c_str() , fmt_args... );
}


template<typename value_t>
void mem_manip_lib::mem_set_val(value_t val) const
{
    *static_cast<value_t*>(this->mem_address) = val;
}

template<typename ret_t>
ret_t mem_manip_lib::mem_read_val() const {
    return *reinterpret_cast<ret_t*>(this->mem_address);
}