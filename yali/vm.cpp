#include "debug.hpp"
#include "vm.hpp"
#include <fmt/core.h>
#include <fmt/color.h>

#define INSTR(label) \
	label: \
	const auto& op = program[ip]; \
	(void)op; /* inhibit warning */ \
	if constexpr (debug::debug_mode) \
	{ \
		fmt::print("{:5}: op {:15}, depth {:3}, [{}]\n", \
			ip, \
			bc::def::infos[op.instruction()].name, \
			_frames_top - _frames.data(), \
			fmt::join(std::vector<uint32_t>{_locals.data(), _locals_top + 1}, " ")); \
	} \

#define DISPATCH() \
	goto *jump_targets[program[ip].instruction()];

#define NEXT_INSTR() \
	DISPATCH()

#define NEXT_INSTR_AUTOPC() \
	++ip; \
	DISPATCH()

namespace yali
{
const callframe& vm::current_callframe() const
{
	return *_frames_top;
}

void vm::frame_push(callframe frame)
{
	new(++_frames_top) callframe{frame};
}

callframe::address_type vm::frame_pop()
{
	// This is OK because return a copy of it.
	return (_frames_top--)->return_address;
}

uint32_t vm::local_pop()
{
	return *(_locals_top--);
}

void vm::local_push(uint32_t value, uint8_t typeinfo)
{
	*(++_locals_top) = value;
}

uint32_t& vm::local_ref_stack(size_t depth)
{
	return *(_locals_top - depth);
}

uint32_t& vm::local_ref_id(size_t id)
{
	return *(current_callframe().local_frame + id);
}

uint32_t& vm::local_top()
{
	return *_locals_top;
}

vm::vm()
{
	_locals.resize(1024 * 1024 / 4); // 1MiB locals
	_locals_top = _locals.data() - 1;

	_frames.resize(256); // 256 nested calls
	_frames_top = _frames.data();
	new(_frames_top) callframe{_locals_top, 0};
}

void vm::run(const std::vector<bc::opcode>& program)
{
	fmt::print("VM initializing, program size {} ({} bytes).\n", program.size(), program.size() * 8);

	size_t ip = 0;

	std::array jump_targets {
		&&llocal_push,
		&&llocal_clone,
		&&llocal_pop,
		&&llocal_pop_but_top,
		&&linvoke_user,
		&&linvoke_system,
		&&lfunc_return,
		&&ljump_cond
	};

	DISPATCH()

	{
		INSTR(llocal_push)
		local_push(op.read<uint32_t>(8), op.read<uint8_t>(40));
		NEXT_INSTR_AUTOPC()
	}

	{
		INSTR(llocal_clone)
		local_push(local_ref_id(op.read<uint32_t>(8)));
		NEXT_INSTR_AUTOPC()
	}

	{
		INSTR(llocal_pop)
		local_pop();
		NEXT_INSTR_AUTOPC()
	}

	{
		INSTR(llocal_pop_but_top)
		auto count = op.read<uint32_t>(8);
		local_ref_stack(count) = local_top();
		_locals_top -= count;
		NEXT_INSTR_AUTOPC()
	}

	{
		INSTR(linvoke_user)
		auto userfuncip = op.read<uint32_t>(8);
		auto passthrough = op.read<uint16_t>(40);
		frame_push({_locals_top - passthrough + 1, ip + 1});
		ip = userfuncip;
		NEXT_INSTR()
	}

	{
		INSTR(linvoke_system)
		auto sysfuncid = program[ip].read<uint32_t>(8);
		auto passthrough = program[ip].read<uint16_t>(40);
		switch (static_cast<bc::system_function>(sysfuncid))
		{
		case bc::system_function::print_arg: {
			auto a = local_pop();
			fmt::print("{}\n", a);
			return;
		} break;

		case bc::system_function::arith_add: {
			auto a = local_pop();
			auto b = local_pop();
			local_push(a + b);
		} break;

		case bc::system_function::arith_sub: {
			auto a = local_pop();
			auto b = local_pop();
			local_push(a - b);
		} break;

		case bc::system_function::cmp_lt: {
			auto a = local_pop();
			auto b = local_pop();
			jump_flag = a < b;
		} break;

		default:
			fmt::print("Unhandled system function!\n");
			return;
		}

		NEXT_INSTR_AUTOPC()
	}

	{
		INSTR(ljump_cond)
		if (jump_flag)
		{
			ip = op.read<uint32_t>(8);
			NEXT_INSTR()
		}
		NEXT_INSTR_AUTOPC()
	}

	{
		INSTR(lfunc_return)
		ip = frame_pop();
		NEXT_INSTR()
	}
}
}
