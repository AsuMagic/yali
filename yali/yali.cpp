#include "yali.hpp"
#include "compiler.hpp"

namespace yali
{
bool run(std::string_view source)
{
	auto program = compile(source);

	if (!program)
	{
		return false;
	}

	return true;
}
}
