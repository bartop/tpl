
#pragma once

#include "helpers.hpp"

#include <vector>

namespace tpl {
namespace meta {

template<class T, class = void>
struct is_factory : std::false_type { };

template<class T>
struct is_factory<
	T,
	typename type_sink<
		decltype(std::declval<T>().create(std::declval<std::vector<int>>()))
	>::type
> : std::true_type { };

}
}
