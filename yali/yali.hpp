#ifndef YALI_HPP
#define YALI_HPP

/*
	The YaLi programming language, based on a Lisp-like dialect.
*/

#include <string_view>

namespace yali
{
bool run(std::string_view source);
}

#endif // YALI_HPP
