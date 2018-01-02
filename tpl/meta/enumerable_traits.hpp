
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
	using value_type = typename std::decay<T>::type::value_type;
	using iterator = typename std::decay<T>::type::iterator;
	using const_iterator = typename std::decay<T>::type::const_iterator;
};

template<class T>
using array_reference_value_type = typename std::remove_reference<decltype(*std::begin(std::declval<T>()))>::type;

template<class T>
struct enumerable_traits<
	T,
   	typename std::enable_if<
		is_enumerable<T>::value &&
	   	std::is_array<
			typename std::remove_reference<T>::type
		>::value
	>::type
> : private enforce_enumerable<T> {
	using value_type = array_reference_value_type<T>;
	using iterator = value_type *;
	using const_iterator = const value_type *;
};

}
}
