#include "yali.hpp"
#include "compiler.hpp"

namespace yali
{
bool run(std::string_view source)
{
	auto program = parse_ast(source);

	if (!program)
	{
		return false;
	}



	return true;
}
}
