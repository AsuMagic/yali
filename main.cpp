#include <fmt/core.h>
#include <fmt/color.h>
#include <fstream>
#include <string>
#include "yali/yali.hpp"
#include "yali/vm.hpp"

int main(/*int argc, char* argv[]*/)
{
	/*if (argc != 2)
	{
		fmt::print(fmt::color::red, "Syntax: yali <source.lisp>\n");
		return 1;
	}

	std::fstream file{argv[1]};
	std::string source{std::istreambuf_iterator{file}, {}};

	if (!yali::run(source))
	{
		fmt::print(fmt::color::orange_red, "error: program execution has failed\n");
	}*/

	using namespace yali::bc;
	using namespace yali::bc::def;

	using in = instruction;

	yali::vm machine;
	fmt::print("Program exited with code {}\n", machine.run({
		mk(in::local_push, {35, 0}),
		mk(in::invoke_user, {3, 1}),
		mk(in::invoke_system, {0, 1}),

		// fib
		mk(in::local_push, {2, 0}),
		mk(in::local_clone, {0}),
		mk(in::invoke_system, {0x20, 2}),

		// if block 1
		mk(in::jump_cond, {18}), //TODO

		// if block 2
		mk(in::local_push, {1, 0}),
		mk(in::local_clone, {0}),
		mk(in::invoke_system, {0x11, 2}),
		mk(in::invoke_user, {3, 1}),

		mk(in::local_push, {2, 0}),
		mk(in::local_clone, {0}),
		mk(in::invoke_system, {0x11, 2}),
		mk(in::invoke_user, {3, 1}),

		mk(in::invoke_system, {0x10, 2}),

		mk(in::local_pop_but_top, {1}),
		mk(in::func_return, {}),

		mk(in::func_return, {})
	}));
}
