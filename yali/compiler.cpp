#include "compiler.hpp"
#include "debug.hpp"
#include <fmt/core.h>
#include <iostream>

namespace yali
{
std::optional<ast::sexpr> compile(std::string_view source)
{
	ast::sexpr ast;

	x3::error_handler handler{source.begin(), source.end(), std::cerr};
	const auto parser = x3::with<x3::error_handler_tag>(std::ref(handler))[parser::program];

	if (!x3::parse(source.begin(), source.end(), parser, ast))
	{
		return {};
	}

	if (debug::debug_mode)
	{
		debug::sexpr_printer{}(ast);
	}

	return {ast};
}

}
