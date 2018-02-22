
#pragma once

#include "helpers.hpp"

#include <type_traits>
#include <iterator>

namespace tpl {
namespace meta {

template<class T, class = void>
struct can_begin : std::false_type {
};

template<class T>
struct can_begin<
	T,
	typename type_sink<typename std::iterator_traits<decltype(std::begin(std::declval<T>()))>::value_type>::type
> : std::true_type {};

template<class T, class = void>
struct can_end : std::false_type { };

template<class T>
struct can_end<
	T,
    typename type_sink<typename std::iterator_traits<decltype(std::end(std::declval<T>()))>::value_type>::type
> : std::true_type { };

template<class T, class U, class = void>
struct are_equality_comparable : std::false_type { };

template<class T, class U>
struct are_equality_comparable<
	T,
	U,
	typename type_sink<decltype(std::declval<T>() == std::declval<U>())>::type
> : std::true_type { };

template<class T, class = void>
struct is_enumerable : std::false_type { };

template<class T>
struct is_enumerable<
	T,
	typename std::enable_if<
		can_begin<T>::value &&
	   	can_end<T>::value &&
	   	are_equality_comparable<
			typename std::iterator_traits<decltype(std::begin(std::declval<T>()))>::value_type,
			typename std::iterator_traits<decltype(std::end(std::declval<T>()))>::value_type
		>::value
	>::type
> : std::true_type { };

template<class T>
struct enforce_enumerable {
	static_assert(is_enumerable<T>::value, "Enumerable type must have comparable begin and end iterators.");
	using type = T;
};

}
}
