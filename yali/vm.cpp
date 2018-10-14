#include "vm.hpp"
#include <fmt/core.h>
#include <fmt/color.h>

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

	for (;;)
	{
		auto op = program[ip];

		size_t next_ip = ip + 1;

		/*fmt::print("{:5}: op {:15}, depth {:3}, locals: {:4}/{:4}    ",
			ip,
			bc::def::infos[op.instruction()].name,
			frames.size(),
			locals.size() - current_callframe().local_frame_offset,
			locals.size());

		fmt::print("local stack: [");
		for (size_t i = 0; i < locals.size(); ++i)
		{
			if (i >= current_callframe().local_frame_offset)
			{
				fmt::print(fmt::color::orange, "{} ", locals[i]);
			}
			else
			{
				fmt::print(fmt::color::gray, "{} ", locals[i]);
			}
		}
		fmt::print("]\n");*/

		switch (static_cast<bc::instruction>(op.instruction()))
		{
		case bc::instruction::local_push:
			locals.push_back(op.read<uint32_t>(8));
			break;

		case bc::instruction::local_clone:
			locals.push_back(locals[current_callframe().local_frame_offset + op.read<uint32_t>(8)]);
			break;

		case bc::instruction::local_pop:
			locals.pop_back();
			break;

		case bc::instruction::local_swap:
			std::swap(locals[locals.size() - 2], locals[locals.size() - 1]);
			break;

		case bc::instruction::invoke_user: {
			auto userfuncip = op.read<uint32_t>(8);
			auto passthrough = op.read<uint16_t>(40);
			//fmt::print("invoke_user {}\n", userfuncid);
			frames.emplace_back(locals.size() - passthrough, next_ip);
			next_ip = userfuncip; // HACK
		} break;

		case bc::instruction::invoke_system: {
			auto sysfuncid = program[ip].read<uint32_t>(8);
			auto passthrough = program[ip].read<uint16_t>(40);
			frames.emplace_back(locals.size() - passthrough, next_ip);
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

			next_ip = current_callframe().return_address;
			frames.pop_back();
		} break;

		case bc::instruction::jump_cond:
			if (jump_flag)
			{
				next_ip = op.read<uint32_t>(8);
			}
			break;

		case bc::instruction::func_return:
			next_ip = current_callframe().return_address;
			frames.pop_back();
			break;

		default:
			fmt::print("Unhandled instruction! ID {}\n", program[ip].instruction());
			return;
		}

		ip = next_ip;
	}
}
}
