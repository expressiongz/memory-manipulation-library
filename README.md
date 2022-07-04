# memory-manip-lib
small memory manipulation library
# current features (2/06/2022)
```cpp
    void unload();

    template<typename...var_arg>
    void dbg_log(std::string const& dbg_message, var_arg...fmt_args) const;

    template<typename... var_arg>
    void dbg_err(std::string const& dbg_error, var_arg...fmt_args) const;

    template<typename stl_container_t>
    stl_container_t mem_read_bytes() const;

    template<typename value_t>
    void mem_set_val(value_t val) const;

    template<typename ret_t>
    ret_t mem_read_val() const;

    template<std::uint32_t sz>
    manipulated_code mem_tramp_hook(const std::uint32_t func_addr);

    template<typename data_t, std::size_t sz>
    std::array<data_t, sizeof(data_t) * sz> mem_read_dyn();
    
    std::uint32_t get_va() const;
    std::uint32_t get_rva() const;

    std::uint8_t mem_read_byte(bool signednesss) const;
    std::string mem_read_string(const std::uint32_t string_sz) const;

    std::vector< std::uint8_t > mem_read_func_bytes() const;

    bool alloc_console();
    bool free_console();

    void reloc_rva(const std::uint32_t mem_address);
    void set_va(const std::uint32_t mem_address);


    void set_rwx(const std::size_t sz);
    void unset_rwx(const std::size_t sz);

    void mem_set_nop(const std::size_t sz);

    bool mem_set_bytes(const std::size_t szbyte, std::span<std::uint8_t> byte_arr);
    bool mem_set_bytes(const std::size_t szbyte, std::uint8_t byte);

```

# update (4/05/2022)
- added mem_read_dyn
allows reading multiple values of data_t consecutively. in the future will overload to allow runtime-calculated sizes.
- fixed formatting
