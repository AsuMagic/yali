#ifndef AST_HPP
#define AST_HPP

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/ast/position_tagged.hpp>
#include <boost/spirit/home/x3/support/ast/variant.hpp>
#include <boost/fusion/include/adapt_struct.hpp>
#include <vector>

namespace yali::ast
{
namespace x3 = boost::spirit::x3;

struct sexpr;

using sexpr_value = x3::variant<
	x3::forward_ast<sexpr>,
	std::string,
	int,
	double
>;

struct sexpr
{
	std::vector<sexpr_value> atoms;
};
}

BOOST_FUSION_ADAPT_STRUCT(yali::ast::sexpr, atoms)

#endif // AST_HPP
