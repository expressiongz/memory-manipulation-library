# memory-manip-lib
small memory manipulation library
# current memory features (12/05/2022)
- reloc_rva:
relocates relative virtual address according to the image base address
- set_rwx: 
set page_execute_readwrite permissions
- unset_rwx:
self explanatory
- mem_set_nop:
sets the x amount bytes starting at a user-provided base address to nop (0x90)
- mem_set_bytes: \n 
sets x amount of bytes from user-provided stl container to address given by the user and set by reloc_rva ( container should store byte-size data types i.e std::uint8_t)
- mem_tramp_hook: 
creates a trampoline hook, the hook address should be a virtual address, and so should be your hook function address.
- mem_read_bytes: 
reads x amount of bytes at a base memory address provided by the user.
-  mem_ret_func_bytes:
returns a std::vector of a functions bytes, function end is determined by checking for alignment bytes
# misc features (09/06/2022)
- unload:
unloads the dll
- create_console:
self explanatory
- free_console: 
self explanatory
- dbg_log: 
custom debug print function
- dbg_err: 
custom debug print function, used for logging errors:

# fixes (11/05/2022)
- improved the code in general.
- optimized some stuff

# purpose / objective
this purpose is a part of my journey towards mastering C++ and also towards reverse engineering. It's also a contribution towards anybody who needs a small and simple memory editing library with no hassles.
