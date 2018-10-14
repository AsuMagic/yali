#ifndef COMPILER_HPP
#define COMPILER_HPP

#include <optional>
#include <string_view>
#include "parser.hpp"
#include "bytecode.hpp"

namespace yali
{
namespace x3 = boost::spirit::x3;

std::optional<ast::sexpr> parse_ast(std::string_view source);
std::vector<bc::opcode> compile_program(const ast::sexpr& sexpr);
}

#endif // COMPILER_HPP
