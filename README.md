# memory-manip-lib
small memory manipulation library
# Primary features (6/07/2022)
```cpp
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

    bool alloc_console();
    bool free_console();

    void reloc_rva(const std::uint32_t mem_address);
    void set_va(const std::uint32_t mem_address);


    void set_page_flags(const std::size_t sz, DWORD new_page_flags);

    void mem_set_nop(const std::size_t sz);

    bool mem_set_bytes(const std::size_t szbyte, std::span<std::uint8_t> byte_arr);
    bool mem_set_bytes(const std::size_t szbyte, std::uint8_t byte);

    void unload();
```

# Code restoration and overwriting features.
```
	void set_new_code(std::span<std::uint8_t> _new_code);
	void restore_code();

	void overwrite_code();
	void* ret_address();

```

# Data restoration and overwriting features

```
	void restore_data();
	void set_data(data_t new_data);

	data_t read_data() const;
	data_t* ret_address() const;
```

# TL:DR

Currently as of 06/07/2022 at 2:57 AM the library has code overwriting (mem_set_nop, mem_set_bytes, mem_set_byte, mem_tramp_hook) with a VirtualProtect wrapper function for ease of use. The code overwriting also comes with a library for restoring and overwriting code without making multiple calls to set virtual and relative addresses in order to overwrite code (currently only implemented for mem_tramp_hook, will be implemented in all other code overwriting functions within 24h ). 

The library also has a data overwrite function mem_set_val, soon to be renamed to mem_set_data. This also comes with its own restoration class for overwriting and restoring data at the same memory location easily.

The library has multiple data reading functions: mem_read_dyn (reads data consecutively, i.e very useful for reading arrays), mem_read_val (reads data. e.g health or ammo ), mem_read_func_bytes (starts reading at a certain base address provided by the user until it encounters alignment bytes, extremely useful if you need to copy the bytecode of a function), mem_read_string (can be replaced with mem_read_dyn but specifically returns a std::string).

There are a few redundancies in the code that I am aware of, simply do not have the time to fix them as of right now. 

# update (6/07/2022)
- overloaded mem_read_dyn with runtime capabilities.
- added set_new_code for the manipulated_code class
- added ret_code_address for the manipulated_code class
- added ret_data_address for the manipulated_data class
- made the set_data function in the manipulated_data class set the old data to the previous "new_data"
- replaced set_rwx and unset_rwx with set_page_flags.
- library no longer sets page flags in its own functions, that is now up to the user. user can still use set_page_flags before calling functions such as mem_set_bytes
- mem_tramp_hook now returns a manipulated_code class.

