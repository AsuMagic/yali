#include "vm.hpp"
#include <fmt/core.h>
#include <fmt/color.h>

#define INSTR(label) \
	label: \
	auto op = program[ip]; \
	(void)op; /*\
	fmt::print("{:5}: op {:15}, depth {:3}, locals: {:4}/{:4}    {}\n", \
		ip, \
		bc::def::infos[op.instruction()].name, \
		frames.size(), \
		locals.size() - current_callframe().local_frame_offset, \
		locals.size(), \
		fmt::join(locals, " ")); \
	(void)op; // inhibit warning*/

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
	return frames.back();
}

uint32_t vm::local_pop()
{
	auto ret = locals.back();
	locals.pop_back();
	return ret;
}

vm::vm()
{
	frames.reserve(256);
	locals.reserve(1024);
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
		locals.push_back(op.read<uint32_t>(8));
		NEXT_INSTR_AUTOPC()
	}

	{
		INSTR(llocal_clone)
		locals.push_back(locals[current_callframe().local_frame_offset + op.read<uint32_t>(8)]);
		NEXT_INSTR_AUTOPC()
	}

	{
		INSTR(llocal_pop)
		locals.pop_back();
		NEXT_INSTR_AUTOPC()
	}

	{
		INSTR(llocal_pop_but_top)
		auto count = op.read<uint32_t>(8);
		locals[locals.size() - count] = locals.back();
		locals.resize(locals.size() - count + 1);
		NEXT_INSTR_AUTOPC()
	}

	{
		INSTR(linvoke_user)
		auto userfuncip = op.read<uint32_t>(8);
		auto passthrough = op.read<uint16_t>(40);
		//fmt::print("invoke_user {}\n", userfuncid);
		frames.emplace_back(locals.size() - passthrough, ip + 1);
		ip = userfuncip;
		NEXT_INSTR()
	}

	{
		INSTR(linvoke_system)
		auto sysfuncid = program[ip].read<uint32_t>(8);
		auto passthrough = program[ip].read<uint16_t>(40);
		frames.emplace_back(locals.size() - passthrough, ip + 1);
		//fmt::print("invoke_system {}\n", sysfuncid);
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
			locals.push_back(a + b);
		} break;

		case bc::system_function::arith_sub: {
			auto a = local_pop();
			auto b = local_pop();
			locals.push_back(a - b);
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

		frames.pop_back();
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
		ip = current_callframe().return_address;
		frames.pop_back();
		NEXT_INSTR()
	}
}
}
