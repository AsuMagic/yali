#include "compiler.hpp"
#include "debug.hpp"
#include <fmt/core.h>
#include <iostream>

namespace yali
{
compiler::compiler(std::string_view p_source) :
	_source{p_source}
{
	ast::sexpr ast;

	x3::error_handler handler{_source.begin(), _source.end(), std::cerr};
	const auto parser = x3::with<x3::error_handler_tag>(std::ref(handler))[parser::sexpr];
	if (!x3::parse(_source.begin(), _source.end(), parser, ast))
	{
		return;
	}

	if (debug::debug_mode)
	{
		debug::sexpr_printer{}(ast);
	}

	_ok = true;
}
}
