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

struct sexpr_value :
	x3::variant<std::string, x3::forward_ast<sexpr>>,
	x3::position_tagged
{
	using base_type::base_type;
	using base_type::operator=;
};

struct sexpr :
	x3::position_tagged
{
	std::vector<sexpr_value> atoms;
};
}

BOOST_FUSION_ADAPT_STRUCT(yali::ast::sexpr, atoms)

#endif // AST_HPP
