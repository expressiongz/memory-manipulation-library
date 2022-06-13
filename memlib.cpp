#include "memlib.h"


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
    static auto base = reinterpret_cast< std::uint32_t >( GetModuleHandleA(nullptr) );
    this->mem_address = reinterpret_cast< void* >(base + address);
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

bool mem_manip_lib::mem_set_bytes( const std::size_t instr_sz , std::span< const std::uint8_t > byte_arr )
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

std::uint32_t mem_manip_lib::mem_tramp_hook(const std::uint32_t new_func, const std::uint32_t instr_size)
{
    constexpr auto jmp_instr_size = 5;

    if (instr_size < 5) {
        return 0;
    }

    DWORD vp_old_prot;
    VirtualProtect(this->mem_address, instr_size, PAGE_EXECUTE_READWRITE, &vp_old_prot);
    std::memset(this->mem_address, 0x90, instr_size);

    const auto rel_addr = (new_func - reinterpret_cast<std::uint32_t>(this->mem_address)) - jmp_instr_size;

    *(static_cast<std::uint8_t*>(this->mem_address)) = 0xE9;
    *(reinterpret_cast<std::uint32_t*>(reinterpret_cast<std::uint32_t>(this->mem_address) + 1)) = rel_addr;

    VirtualProtect(this->mem_address, instr_size, vp_old_prot, nullptr);
    return reinterpret_cast<std::uint32_t>(this->mem_address) + instr_size;
}

std::vector< std::uint8_t > mem_manip_lib::mem_ret_func_bytes(  ) const {
    auto bytes_read = std::vector< std::uint8_t >();
    auto const* curr_byte = reinterpret_cast< std::uint8_t const* >( this->mem_address );
    do {
        bytes_read.push_back( *curr_byte );
        curr_byte += 1;

    } while (*( curr_byte + 1 ) != 0xCC || *( curr_byte + 2 ) != 0xCC || *( curr_byte + 3 ) != 0xCC);
    return bytes_read;
}