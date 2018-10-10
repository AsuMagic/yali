#ifndef ERROR_HPP
#define ERROR_HPP

#include <boost/spirit/home/x3.hpp>
#include <boost/spirit/home/x3/support/utility/annotate_on_success.hpp>
#include <boost/spirit/home/x3/support/utility/error_reporting.hpp>
#include <fmt/format.h>

namespace yali::parser
{
namespace x3 = boost::spirit::x3;

struct error_handler
{
	template<class It, class Exception, class Context>
	auto on_error(
		const It&, const It&,
		const Exception& except,
		const Context& ctx
	)
	{
		auto& handler = x3::get<x3::error_handler_tag>(ctx).get();
		std::string message = fmt::format("error: expected {}:", except.which());
		handler(except.where(), message);

		return x3::error_handler_result::fail;
	}
};
}

#endif // ERROR_HPP
