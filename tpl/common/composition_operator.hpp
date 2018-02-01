
#pragma once

#include "../meta/is_enumerable.hpp"

#include <type_traits>

namespace tpl{

template<
	class Factory,
	class Enumerable,
   	class = typename std::enable_if<meta::is_enumerable<Enumerable>::value>::type
>
auto
operator|(Enumerable &&enumerable, Factory &&factory) {
	return std::forward<Factory>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

}
