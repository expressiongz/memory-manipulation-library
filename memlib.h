#pragma once
#include "includes.h"

class mem_manip_lib {
private:

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

    std::vector< std::uint8_t > mem_ret_func_bytes() const;

    bool alloc_console() ;
    bool free_console();

    void reloc_rva( const std::uint32_t mem_address );
    void set_rwx( const std::size_t sz );

    void unset_rwx( const std::size_t sz );
    void mem_set_nop( const std::size_t sz );


    bool mem_set_bytes( const std::size_t szbyte, std::span< const std::uint8_t > byte_arr ) ;
    bool mem_tramp_hook( const std::size_t sz , const std::uint32_t func_addr, std::uint32_t* jmp_back_address );

    explicit mem_manip_lib( HMODULE hmod , const std::string_view dll_name , bool alloc_console_f = false) : dllname(dll_name), mod_handle(hmod) {
        if (alloc_console_f) {
            alloc_console();
            dbg_log( "initialized ", this->dllname );
        }
        else if (GetConsoleWindow()) {
            dbg_log("initialized ", this->dllname);
        }
    }
};
    
template< typename stl_container_t >
stl_container_t mem_manip_lib::mem_read_bytes() const {
    auto bytes_read = stl_container_t();
    dbg_log( "read bytes function called, reading %u bytes at virtual address 0x%p." , bytes_read.size(), this->mem_address);

    for ( auto k = 0u; k < bytes_read.size(); ++k ) {
        bytes_read[k] = ( * ( reinterpret_cast< std::uint8_t* >( reinterpret_cast< std::uint32_t >( this->mem_address ) + k) ) );
    }

    dbg_log( "finished reading %u bytes at virtual address 0x%p" , bytes_read.size(), this->mem_address);
    return bytes_read;
}

template< typename... var_arg >
void mem_manip_lib::dbg_log( std::string const& dbg_message , var_arg... fmt_args ) const {
    std::string dbg_err_prefix = "[+] ";
    std::printf( ( dbg_err_prefix + dbg_message ).c_str() , fmt_args... );
    std::cout << "\n";
}


template< typename... var_arg >
void mem_manip_lib::dbg_err( std::string const& dbg_message , var_arg... fmt_args ) const {
    std::string dbg_err_prefix = "[!] ";
    std::printf( ( dbg_err_prefix + dbg_message ).c_str() , fmt_args... );
    std::cout << "\n";
}