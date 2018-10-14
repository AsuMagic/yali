#ifndef DEBUG_HPP
#define DEBUG_HPP

#include "ast.hpp"

namespace yali::debug
{
constexpr bool debug_mode = false;

struct sexpr_printer
{
	using result_type = void;

	unsigned indent = 0;
	void tab(unsigned extra_indent = 0) const;

	void operator()(const ast::sexpr& ast) const;
	void operator()(const std::string& str) const;
	void operator()(int val) const;
	void operator()(double val) const;
};
}

#endif // DEBUG_HPP
