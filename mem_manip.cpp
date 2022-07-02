#include "mem_manip.h"


void mem_manip_lib::unload()
{
    free_console();
    FreeLibrary( this->mod_handle );
}

bool mem_manip_lib::alloc_console()
{
    if ( this->console_attached == true )
    {
        return false;
    }

    this->console_attached = true;
    std::FILE* fp;

    AllocConsole();
    freopen_s( &fp , "CONOUT$" , "w" , stdout );
    SetConsoleTitleA( this->dllname.data() );

    return true;
}


bool mem_manip_lib::free_console()
{
    if ( this->console_attached )
    {
        FreeConsole();
        this->console_attached = false;
        return true;
    }

    return false;
}


void mem_manip_lib::reloc_rva( const std::uint32_t address )
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

void mem_manip_lib::set_rwx(const std::size_t sz)
{
    VirtualProtect( this->mem_address , sz, PAGE_EXECUTE_READWRITE , &( this->old_prot ) );
}


void mem_manip_lib::unset_rwx(const std::size_t sz)
{
    VirtualProtect( this->mem_address , sz , this->old_prot , nullptr );

    this->old_prot = 0;
}

void mem_manip_lib::mem_set_nop( const std::size_t sz )
{
    set_rwx( sz );

    std::memset( this->mem_address , 0x90 , sz );


}

bool mem_manip_lib::mem_set_bytes( const std::size_t instr_sz , std::span< std::uint8_t > byte_arr ) 
{

    if ( byte_arr.size() > instr_sz )
    {
        return false;
    }

    mem_set_nop( instr_sz );
    std::memcpy( this->mem_address , byte_arr.data() , byte_arr.size() );

    unset_rwx( instr_sz );
    return true;
}


bool mem_manip_lib::mem_set_bytes(const std::size_t instr_sz, std::uint8_t byte)
{

    mem_set_nop(instr_sz);
    std::memset(this->mem_address, byte, instr_sz);
    unset_rwx(instr_sz);
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


std::vector< std::uint8_t > mem_manip_lib::mem_read_func_bytes() const 
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

std::uint32_t mem_manip_lib::mem_tramp_hook(const std::uint32_t new_func, const std::uint32_t instr_sz)
{
    constexpr auto jmp_instr_size = 5;
    if (instr_sz < 5) 
    {
        return 0;
    }
    set_rwx(instr_sz),
    std::memset(this->mem_address, 0x90, instr_sz);

    const auto rel_addr = (new_func - reinterpret_cast<std::uint32_t>(this->mem_address)) - jmp_instr_size;

    *(static_cast<std::uint8_t*>(this->mem_address)) = 0xE9;
    *(reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint32_t>(this->mem_address) + 1)) = rel_addr;
    unset_rwx(instr_sz);
    return reinterpret_cast<std::uint32_t>(this->mem_address) + instr_sz;
}


