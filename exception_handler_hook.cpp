#include "includes.hpp"
#include "exception_handler_hook.hpp"

#ifdef _M_IX86
#define XIP Eip
#elif _M_X64
#define XIP Rip
#endif

bool breakpoint_exception{ false };

static std::uint32_t hooked_address_{ 0 };
static std::uint32_t hook_to_address_{ 0 };
static std::uint8_t o_byte_{ 0 };

long __stdcall handler(EXCEPTION_POINTERS* exception)
{
	if (exception->ContextRecord->XIP != hooked_address_) return EXCEPTION_CONTINUE_SEARCH;

	if (exception->ExceptionRecord->ExceptionCode == EXCEPTION_BREAKPOINT && breakpoint_exception)
	{
		exception->ContextRecord->XIP = hook_to_address_;
		return EXCEPTION_CONTINUE_EXECUTION;
	}

	if(exception->ExceptionRecord->ExceptionCode == STATUS_GUARD_PAGE_VIOLATION)
	{
		DWORD vp_old_protection{ 0 };

		VirtualProtect(reinterpret_cast<void*>(hooked_address_), 1, PAGE_EXECUTE_READ | PAGE_GUARD, &vp_old_protection);

		exception->ContextRecord->XIP = hook_to_address_;
		return EXCEPTION_CONTINUE_EXECUTION;
	}
	return EXCEPTION_CONTINUE_SEARCH;
}

void exception_handler::start_handler(void* hooked_address, void* hook_to_address, bool breakpoint)
{
	DWORD vp_old_protection{ 0 };
	
	hooked_address_ = reinterpret_cast<std::uint32_t>(hooked_address);
	hook_to_address_ = reinterpret_cast<std::uint32_t>(hook_to_address);
	breakpoint_exception = breakpoint;

	this->exception_handler_handle_ = AddVectoredExceptionHandler(true, &handler);

	if (breakpoint)
	{
		o_byte_ = *static_cast<std::uint8_t*>(hooked_address);

		VirtualProtect(hooked_address, 1, PAGE_EXECUTE_READWRITE, &vp_old_protection);

		*static_cast<std::uint8_t*>(hooked_address) = 0xCC;

		VirtualProtect(hooked_address, 1, vp_old_protection, &vp_old_protection);
		return;
	}

	VirtualProtect(hooked_address, 1, PAGE_EXECUTE_READ | PAGE_GUARD, &vp_old_protection);
}

void exception_handler::remove_handler() const
{
	if(breakpoint_exception)
	{
		DWORD vp_old_protection{ 0 };
		VirtualProtect(reinterpret_cast<void*>(hooked_address_), 1, PAGE_EXECUTE_READWRITE, &vp_old_protection);

		*reinterpret_cast<std::uint8_t*>(hooked_address_) = o_byte_;

		VirtualProtect(reinterpret_cast<void*>(hooked_address_), 1, vp_old_protection, &vp_old_protection);
	}

	RemoveVectoredExceptionHandler(this->exception_handler_handle_);
}



void exception_handler::change_hook_to_address(void* new_hook_to_address)
{
	hook_to_address_ = reinterpret_cast<std::uint32_t>(new_hook_to_address);
}


void exception_handler::change_hooked_address(void* new_hooked_address)
{
	hooked_address_ = reinterpret_cast<std::uint32_t>(new_hooked_address);
}
