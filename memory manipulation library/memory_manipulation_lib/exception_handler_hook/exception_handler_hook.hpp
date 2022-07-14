#pragma once

class exception_handler
{
private:
	void* exception_handler_handle_{ nullptr };
public:
	static void change_hooked_address(void* new_hooked_address_);
	static void change_hook_to_address(void* new_hook_to_address_);

	void start_handler(void* hooked_address_, void* hook_to_address_, bool breakpoint);
	void remove_handler() const;
};

