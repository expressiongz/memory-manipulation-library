#include "memlib.h"
#include "includes.h"


void mem_manip_lib::unload()
{
    dbg_log( "unloading " , this->dllname , " dll");
    free_console();
    FreeLibrary( this->mod_handle );
}


bool mem_manip_lib::alloc_console()
{
    if ( this->console_attached == true )
    {
       dbg_err( "console already allocated" );
        return false;
    }

    this->console_attached = true;
    std::FILE* fp;

    AllocConsole();
    freopen_s( &fp , "CONOUT$" , "w" , stdout );
    SetConsoleTitleA( this->dllname.data() );

    dbg_log( "allocated console." );
    return true;
}


bool mem_manip_lib::free_console()
{
    if (!this->console_attached) return false;
      
    dbg_log( "free'd the console from the process" );
    FreeConsole();
    this->console_attached = false;  

    return true;
}


void mem_manip_lib::reloc_rva( const std::uint32_t address )
{
    static auto base = reinterpret_cast < std::uint32_t > ( GetModuleHandleA(nullptr) );
    this->mem_address = reinterpret_cast < void* > (base + address);
}


void mem_manip_lib::set_rwx(const std::size_t sz)
{
    VirtualProtect( this->mem_address , sz, PAGE_EXECUTE_READWRITE , &( this->old_prot ) );
    dbg_log("set rwx for %u bytes at virtual address 0x%p", sz, this->mem_address);
}


void mem_manip_lib::unset_rwx(const std::size_t sz)
{
    VirtualProtect( this->mem_address , sz , this->old_prot , nullptr );
    dbg_log( "unset rwx for %u bytes at virtual address 0x%p" , sz , this->mem_address );

    this->old_prot = 0;
}

void mem_manip_lib::mem_set_nop( const std::size_t sz )
{
    dbg_log( "nop byte patch function called, attempting to patch bytes to nop." );
    set_rwx( sz );

    std::memset( this->mem_address , 0x90 , sz );

    dbg_log( "set %u bytes to 0x90 at virtual address 0x%p" , sz , this->mem_address );

}

bool mem_manip_lib::mem_set_bytes( const std::size_t instr_sz , std::span < const std::uint8_t > byte_arr )
{
    dbg_log( "byte patch function called, attempting to patch bytes." );

    if ( byte_arr.size() > instr_sz )
    {
        dbg_err( "amount of bytes provided by user exceeds amount of bytes to be patched." );
        return false;
    }

    mem_set_nop( instr_sz );
    std::memcpy( this->mem_address , byte_arr.data() , byte_arr.size() );

    dbg_log( "set %u user-provided bytes at virtual address 0x%p" , byte_arr.size() , this->mem_address );
    dbg_log( "user provided bytes %X" , byte_arr );

    unset_rwx( instr_sz );
    return true;
}

bool mem_manip_lib::mem_tramp_hook( const std::size_t sz , const std::uint32_t func_address , const std::uint32_t hook_address , std::uint32_t* p_jmp_back )
{
    dbg_log( "trampoline hook function called, attempting a trampoline hook." );

    if ( sz < 5 )
    {
        dbg_err( "number of bytes too small for trampoline hook. number of bytes given: %u" , sz );
        return false;
    }

    *p_jmp_back = hook_address + sz;
    this->mem_address = reinterpret_cast < void* > ( hook_address );

    auto rel_address = ( func_address - hook_address ) - 5;
    auto tramp_hook_bytes = std::to_array<std::uint8_t>(
        { 0xE9, 0x00, 0x00, 0x00, 0x00 }
    );

    *( reinterpret_cast < std::uint32_t* > ( &tramp_hook_bytes[1] ) ) = rel_address;

    if ( mem_set_bytes( sz, tramp_hook_bytes ) )
    {
        dbg_log( "trampoline hook created at virtual address 0x%p" , this->mem_address );
        return true;
    }

    dbg_err( "failed to create trampoline hook. amount of bytes is not enough." );
    return false;
}
