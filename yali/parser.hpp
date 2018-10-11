#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include "error.hpp"

namespace yali::parser
{
namespace x3 = boost::spirit::x3;

struct program_class : error_handler {};

static x3::rule<struct sexpr_value_class, ast::sexpr_value> sexpr_value = "atom";
static x3::rule<struct sexpr_class, ast::sexpr> sexpr = "s-expression";
static x3::rule<program_class, ast::sexpr> program = "s-expression";

const auto identifier = +(~x3::space - x3::char_("()"));
const auto pad = x3::omit[*x3::space];

const auto sexpr_value_def = identifier | sexpr;
const auto sexpr_def = '(' > pad > (sexpr_value % +x3::space) > pad > ')';
const auto program_def = pad > sexpr > pad > !x3::char_;

BOOST_SPIRIT_DEFINE(sexpr_value, sexpr, program);
}

#endif // PARSER_HPP
