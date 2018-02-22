
#pragma once

#include "helpers.hpp"

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

template<class T>
struct associative_element_traits<
	T,
   	typename std::enable_if<
		std::is_same<
			typename std::decay<T>::type,
			std::pair<typename T::first_type, typename T::second_type>
   		>::value
	>::type
> {
	using key_type = typename T::first_type;
	using mapped_type = typename T::second_type;

	static const key_type &key_value(const T &value){
		return value.first;
	}

	static const mapped_type &mapped_value(const T &value){
		return value.second;
	}
};


template<class T, class = void>
struct is_associative : std::false_type {};

template<class T>
struct is_associative<
	T,
   	typename type_sink<
		typename associative_element_traits<typename std::decay<T>::type::value_type>::key_type,
		typename associative_element_traits<typename std::decay<T>::type::value_type>::mapped_type
	>::type
> : std::true_type{ };


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
