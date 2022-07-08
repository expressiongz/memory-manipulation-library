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
    void restore_page_flags(const std::size_t sz);

    void mem_set_nop(const std::size_t sz);

    bool mem_set_bytes(const std::size_t szbyte, std::span<std::uint8_t> byte_arr);
    bool mem_set_byte(const std::size_t szbyte, std::uint8_t byte);

    void unload_dll();

 ```

# Code restoration and overwriting features.
```cpp
	void replace_new_code(bool overwrite, std::span<std::uint8_t> _new_code);
	void replace_overwritten_code(std::span<std::uint8_t> _overwritten_code);
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


# update (07/07/2022)
- updated stuff in general too lazy to write patch notes
- fixed bugs and discrepancies left over from 3am programming...
