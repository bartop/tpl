
#pragma once

#include "type_sink.hpp"

#include <type_traits>
#include <utility>

namespace tpl{

namespace meta{

template<class T, class = void>
struct is_associative_element : std::false_type {};

template<class T, class U>
struct is_associative_element<std::pair<T, U>> : std::true_type {};

template<class T, class = void>
struct associative_element_traits {};

template<class T, class U>
struct associative_element_traits<std::pair<T, U>> {
	using key_type = T;
	using mapped_type = U;
};

template<class T, class = void>
struct is_associative : std::false_type {};

template<class T>
struct is_associative<
	T,
   	typename type_sink<
		typename T::mapped_type,
		typename T::key_type
	>::type
> : std::true_type{};


template<class T>
struct enforce_associative {
	static_assert(is_associative<T>::value, "Type given to enforce_associative must have defined mapped_type and key_type");
	using type = T;
};

template<class T>
struct associative_base : enforce_associative<T> {
	using mapped_type = typename T::mapped_type;
	using key_type = typename T::key_type;
};

struct empty_base { };

template<class T>
using associative_traits = typename std::conditional<is_associative<T>::value, associative_base<T>, empty_base>::type;

}

}
