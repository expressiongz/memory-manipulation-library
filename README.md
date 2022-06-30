# memory-manip-lib
small memory manipulation library
# current features (30/06/2022)
```cpp
    void unload();

    template< typename...var_arg >
    void dbg_log( std::string const& dbg_message , var_arg...fmt_args ) const;

    template< typename... var_arg >
    void dbg_err( std::string const& dbg_error , var_arg...fmt_args ) const;

    template< typename stl_container_t >
    stl_container_t mem_read_bytes() const;

    template<typename int_t>
    int_t mem_read_int(bool signedness) const;

    std::uint8_t mem_read_byte(bool signednesss) const;

    std::string mem_read_string(const std::uint32_t string_sz) const;

    std::vector< std::uint8_t > mem_read_func_bytes() const;

    bool alloc_console();
    bool free_console();

    void reloc_rva( const std::uint32_t mem_address );
    void set_va(const std::uint32_t mem_address);

    void set_rwx( const std::size_t sz );
    void unset_rwx( const std::size_t sz );

    void mem_set_nop( const std::size_t sz );


    bool mem_set_bytes( const std::size_t szbyte, std::span< std::uint8_t > byte_arr );
    bool mem_set_bytes( const std::size_t szbyte, std::uint8_t byte );
    
    std::uint32_t mem_tramp_hook( const std::uint32_t func_addr, const std::uint32_t instr_sz );
```

# fixes (11/05/2022)
- major improvements
- fixed some debug logging bugs.

# purpose / objective
this purpose is a part of my journey towards mastering C++ and also towards reverse engineering. It's also a contribution towards anybody who needs a small and simple memory editing library with no hassles.
