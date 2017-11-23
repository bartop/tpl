
#pragma once

#include <type_traits>
#include <iterator>

namespace tpl {

template<class ...> struct type_sink { typedef void type; };

template<class T, class = void>
struct can_begin : std::false_type {
};

template<class T>
struct can_begin<
	T,
	typename type_sink<decltype(std::begin(std::declval<T &>()))>::type
> : std::true_type {};

template<class T, class = void>
struct can_end : std::false_type { };

template<class T>
struct can_end<
	T,
    typename type_sink<decltype(std::end(std::declval<T &>()))>::type
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
			decltype(std::begin(std::declval<T &>())),
			decltype(std::end(std::declval<T &>()))
		>::value
	>::type
> : std::true_type { };

}
