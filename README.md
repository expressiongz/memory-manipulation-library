# memory-manip-lib
very basic and minimal memory manipulation/editing library
# Current features (28/05/2022)
- rebase_address
- set_rwx (set page_execute_readwrite permissions)
- unset_rwx
- mem_set_nop (set the bytes of a memory address to 0x90 / nop)
- mem_set_bytes (set the bytes of a memory address to the bytes of a user-provided std::array (uint8_t)
## misc: 
- creates dll console on class construction
- free_console
- unload (unloads the dll, useful for debugging mid-development)
# purpose / objective
the purpose of this library is to make it easier for developers to partake in their developing of dlls which manipulate or edit memory without doing the grunt work.
if you have any suggestions contact alephnull#6435 (developer of this library)
