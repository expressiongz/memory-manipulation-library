# memory-manip-lib
small memory manipulation library
# current memory features (09/05/2022)
- reloc_rva (relocates relative virtual address according to the image base address)
- set_rwx (set page_execute_readwrite permissions)
- unset_rwx (self explanatory)
- mem_set_nop (sets the x amount bytes starting at a user-provided base address to 0x90)
- mem_set_bytes (replaces the bytes of a user-provided base address to the bytes of a user-provided stl container ( container should store byte-size data types i.e std::uint8_t)
- mem_tramp_hook (creates a trampoline hook, the hook address should be a virtual address, and so should be your hook function address.)
- mem_read_bytes (reads x amount of bytes at a base memory address provided by the user.)
# misc features (09/06/2022)
- unload (unloads the dll)
- create_console (self explanatory)
- free_console (self explanatory)
- dbg_log (custom debug print function)
- dbg_err (custom debug print function, used for logging errors)

# fixes (09/05/2022)


# purpose / objective
the purpose of this library is to make it easier for developers to partake in their developing of dlls which manipulate or edit memory without doing the grunt work.
if you have any suggestions contact expb#6435 (developer of this library).
this library is also a part of my cpp learning journey, this is my main project for learning and experimenting, i update this library as i learn and improve my coding practices in modern cpp
