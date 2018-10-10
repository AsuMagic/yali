#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "ast.hpp"

namespace yali::debug
{
constexpr bool debug_mode = true;

struct sexpr_printer
{
	using result_type = void;

	unsigned indent = 0;
	void tab(unsigned extra_indent = 0) const;

	void operator()(const ast::sexpr& ast) const;
	void operator()(const std::string& str) const;
};
}

#endif // DEBUG_HPP
