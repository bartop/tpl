
#pragma once

#include "../meta/helpers.hpp"
#include "../meta/is_enumerable.hpp"
#include "../meta/is_factory.hpp"

#include <vector>
#include <type_traits>

namespace tpl{

template<
	class Enumerable,
	class Factory,
	class = typename std::enable_if<
		meta::is_enumerable<Enumerable>::value && meta::is_factory<Factory>::value
	>::type
>
decltype(std::declval<Factory>().create(std::declval<Enumerable>()))
operator|(Enumerable &&enumerable, Factory &&factory){
	return std::forward<Factory>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

}
