# memory-manip-lib
small memory manipulation library
# Current memory features (31/05/2022)
- rebase_address
- set_rwx (set page_execute_readwrite permissions)
- unset_rwx
- mem_set_nop (set the bytes of a memory address to 0x90 / nop)
- mem_set_bytes (set the bytes of a memory address to the bytes of a user-provided std::array (uint8_t)
- mem_tramp_hook (creates a trampoline hook, the hook address should be a virtual address, and so should be your hook function address.)

# purpose / objective
the purpose of this library is to make it easier for developers to partake in their developing of dlls which manipulate or edit memory without doing the grunt work.
if you have any suggestions contact expfloxks#6435 (developer of this library)
