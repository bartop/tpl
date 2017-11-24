
#pragma once

#include "is_enumerable.hpp"

#include <type_traits>

namespace tpl{

namespace meta{

template<class T, class = void>
struct enumerable_traits {};

template<class T>
struct enumerable_traits<
	T,
	typename std::enable_if<
		is_enumerable<T>::value &&
	   	!std::is_array<
			typename std::remove_reference<T>::type
		>::value
	>::type
> : private enforce_enumerable<T> {
	using value_type = typename T::value_type;
};

template<class T, size_t N>
struct enumerable_traits<T(&)[N]> : private enforce_enumerable<T(&)[N]> {
	using value_type = T;
};

}

}
