#ifndef PARSER_HPP
#define PARSER_HPP

#include "ast.hpp"
#include "error.hpp"

namespace yali::parser
{
namespace x3 = boost::spirit::x3;

struct program_class : error_handler {};

static x3::rule<struct identifier_class, std::string> identifier = "identifier";
static x3::rule<struct sexpr_value_class, ast::sexpr_value> sexpr_value = "atom";
static x3::rule<struct sexpr_class, ast::sexpr> sexpr = "s-expression";

const auto pad = x3::omit[*x3::space];

const auto identifier_def = +(~x3::space - x3::char_("()"));
const auto sexpr_value_def =
	x3::real_parser<double, x3::strict_real_policies<double>>{} |
	x3::int_ |
	identifier |
	sexpr;

const auto sexpr_def = '(' > pad > -(sexpr_value % +x3::space) > pad > ')';
const auto program_parser = sexpr % *x3::space;

BOOST_SPIRIT_DEFINE(sexpr_value, sexpr, identifier);
}

#endif // PARSER_HPP
