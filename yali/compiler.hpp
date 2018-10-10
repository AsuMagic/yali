#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <string_view>
#include "parser.hpp"

namespace yali
{
namespace x3 = boost::spirit::x3;

class compiler
{
	std::string_view _source;
	bool _ok = false;

public:
	compiler(std::string_view p_source);

	operator bool() const
	{
		return _ok;
	}
};
}

#endif // COMPILER_HPP
