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

	yali::vm machine;
	machine.run({
		mk(local_push, {35, 0}),
		mk(invoke_user, {0, 1}),
		mk(invoke_system, {0, 1}),

		// fib
		mk(local_push, {2, 0}),
		mk(local_clone, {0}),
		mk(invoke_system, {0x20, 2}),

		// if block 1
		mk(jump_cond, {19}), //TODO

		// if block 2
		mk(local_push, {1, 0}),
		mk(local_clone, {0}),
		mk(invoke_system, {0x11, 2}),
		mk(invoke_user, {0, 1}),

		mk(local_push, {2, 0}),
		mk(local_clone, {0}),
		mk(invoke_system, {0x11, 2}),
		mk(invoke_user, {0, 1}),

		mk(invoke_system, {0x10, 2}),

		mk(local_swap),
		mk(local_pop),

		mk(func_return, {}),

		mk(func_return, {})
	});
}
