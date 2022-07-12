# memory-manip-lib
small memory manipulation library
# Primary features (10/07/2022)
```cpp
    template<std::size_t sz>
    manipulated_code mem_tramp_hook(const std::uint32_t func_addr) const;

    template<std::size_t sz>
    manipulated_code mem_set_byte(std::uint8_t byte) const;

    template<std::size_t sz>
    manipulated_code mem_set_bytes(std::span<std::uint8_t> bytes) const;

    template<typename stl_container_t>
    stl_container_t mem_read_bytes() const;

    template<typename value_t>
    manipulated_data<value_t> mem_set_data(value_t val) const;

    template<typename ret_t>
    ret_t mem_read_data() const;

    template<typename data_t, std::size_t sz>
    std::array<data_t, sizeof(data_t) * sz> mem_read_dyn() const;

    template<typename data_t>
    std::vector<data_t> mem_read_dyn(const std::size_t sz) const;

    template<typename value_t, std::size_t sz>
    manipulated_array_data<value_t, sz> mem_set_dyn(value_t val) const;

    template<std::size_t sz>
    manipulated_code mem_set_nop() const;

    std::uint32_t get_va() const;
    std::uint32_t get_rva() const;

    std::string mem_read_string(const std::size_t string_sz) const;
    std::uint8_t mem_read_byte(bool signednesss) const;

    std::vector<std::uint8_t> mem_read_func_bytes() const;

    void reloc_rva(const std::uint32_t mem_address);
    void set_va(const std::uint32_t mem_address);

    void set_page_flags(const std::size_t sz, DWORD new_page_flags);
    void restore_page_flags(const std::size_t sz);
    void unload_dll() const;

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
## manipulated_data
``` cpp
	void restore_data();
	void set_data(data_t new_data);

	data_t read_data() const;
	data_t* ret_data_address() const;
	
```
## manipulated_array_data
```cpp

	void set_overwritten_array(std::span<array_data_t> array);
	void set_new_array(std::span<array_data_t> array);
	void set_array_index(const std::uint32_t index, array_data_t value) const;

	void overwrite_array() const;
	void restore_array() const;

	array_data_t read_array_index(const std::uint32_t index) const;
	std::vector<array_data_t> read_array() const;

	array_data_t* ret_array_address() const;
```

## exception_handler
```
	static void change_hooked_address(void* new_hooked_address_);
	static void change_hook_to_address(void* new_hook_to_address_);

	void start_handler(void* hooked_address_, void* hook_to_address_, bool breakpoint);
	void remove_handler() const;
```

# update (12/07/2022)
- added exception handler hooking.
- minor changes.
