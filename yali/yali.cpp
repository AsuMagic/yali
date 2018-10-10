#include "yali.hpp"
#include "compiler.hpp"

namespace yali
{
bool run(std::string_view source)
{
	compiler comp{source};

	if (!comp)
	{
		return false;
	}

	return true;
}
}
