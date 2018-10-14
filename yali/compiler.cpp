#include "compiler.hpp"
#include "debug.hpp"
#include <fmt/core.h>
#include <iostream>

namespace yali
{
std::optional<ast::sexpr> parse_ast(std::string_view source)
{
	ast::sexpr ast;

	x3::error_handler handler{source.begin(), source.end(), std::cerr};
	const auto parser = x3::with<x3::error_handler_tag>(handler)[parser::program_parser];

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

std::vector<bc::opcode> compile_program(const ast::sexpr& sexpr)
{
	return {};
}
}
