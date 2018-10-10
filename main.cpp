#include <fmt/core.h>
#include <fmt/color.h>
#include <fstream>
#include <string>
#include "yali/yali.hpp"

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fmt::print(fmt::color::red, "Syntax: yali <source.lisp>\n");
	}

	std::fstream file{argv[1]};
	std::string source{std::istreambuf_iterator{file}, {}};

	if (!yali::run(source))
	{
		fmt::print(fmt::color::orange_red, "error: program execution has failed\n");
	}
}
