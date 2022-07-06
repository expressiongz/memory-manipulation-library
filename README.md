# memory-manip-lib
small memory manipulation library
# Primary features (6/07/2022)
```cpp
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

    void mem_set_nop(const std::size_t sz);

    bool mem_set_bytes(const std::size_t szbyte, std::span<std::uint8_t> byte_arr);
    bool mem_set_bytes(const std::size_t szbyte, std::uint8_t byte);

    void unload_dll();
 ```

# Code restoration and overwriting features.
```cpp
	void set_new_code(bool overwrite, std::span<std::uint8_t> _new_code);
	void set_code_restore_buf(std::span<std::uint8_t> _code_restore_buf);
	void restore_code();

	void overwrite_code();
	void* ret_address();


```

# Data restoration and overwriting features

``` cpp
	void restore_data();
	void set_data(data_t new_data);

	data_t read_data() const;
	data_t* ret_data_address() const;
```


# update (6/07/2022)
- overloaded mem_read_dyn with runtime capabilities.
- added set_new_code for the manipulated_code class
- added ret_code_address for the manipulated_code class
- added ret_data_address for the manipulated_data class
- made the set_data function in the manipulated_data class set the old data to the previous "new_data"
- replaced set_rwx and unset_rwx with set_page_flags.
- library no longer sets page flags in its own functions, that is now up to the user. user can still use set_page_flags before calling functions such as mem_set_bytes
- mem_tramp_hook now returns a manipulated_code class.
- removed console functions.
- improved manipulated_code class and manipulated_data class.
