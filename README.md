# memory-manip-lib
A very basic and minimal memory manipulation/editing library, with example use for Assault Cube
# Future updates
I may update this repository in the case of: adding functions, cleaning code, optimizing code
# Latest update (21/05/2022)
- Added user provided names for your project (will appear in console)
- Added set rwx (set page_execute_readwrite) and unset_rwx to prevent setting and unsetting page permissions constantly.
- Added DLL unloading. Allows developers to inject and "uninject" their dlls when developing.
- Added set value. Allows you to set values of memory addresses.
- Added free console and integrated create console into memorypatch_lib (soon to be mem_manip_lib) constructor.
- Changed std::vector use in mem_set_bytes to std::array (thank you 0x90 and iivillian)
