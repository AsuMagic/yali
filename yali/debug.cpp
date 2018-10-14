#include "debug.hpp"

#include <fmt/core.h>

namespace yali::debug
{
void sexpr_printer::tab(unsigned extra_indent) const
{
	for (unsigned i = 0; i < indent + extra_indent; ++i)
	{
		fmt::print("    ");
	}
}

void sexpr_printer::operator()(const ast::sexpr& ast) const
{
	tab();
	fmt::print("( <params={}>", ast.atoms.size());

	for (auto& atom : ast.atoms)
	{
		fmt::print("\n");
		boost::apply_visitor(sexpr_printer{indent + 1}, atom);
	}

	fmt::print("\n");
	tab();
	fmt::print(")");
}

void sexpr_printer::operator()(const std::string& str) const
{
	tab();
	fmt::print("<identifier> {}", str);
}

void sexpr_printer::operator()(int val) const
{
	tab();
	fmt::print("<integer> {}", val);
}

void sexpr_printer::operator()(double val) const
{
	tab();
	fmt::print("<real> {}", val);
}
}
