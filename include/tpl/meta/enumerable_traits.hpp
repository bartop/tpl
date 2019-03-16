
#pragma once

#include "is_enumerable.hpp"

#include <type_traits>
#include <iterator>

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
	using decayed = typename std::decay<T>::type;
	using value_type = typename decayed::value_type;
	using iterator = typename decayed::iterator;
	using const_iterator = typename decayed::const_iterator;

	static iterator begin(decayed &enumerable){
		return std::begin(enumerable);
	}

	static iterator end(decayed &enumerable){
		return std::end(enumerable);
	}

	static const_iterator begin(const decayed &enumerable){
		return std::begin(enumerable);
	}

	static const_iterator end(const decayed &enumerable){
		return std::end(enumerable);
	}

	static const_iterator cbegin(decayed &enumerable){
		return std::begin(const_cast<const decayed &>(enumerable));
	}

	static const_iterator cend(decayed &enumerable){
		return std::end(const_cast<const decayed &>(enumerable));
	}
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

	static iterator begin(T enumerable){
		return std::begin(enumerable);
	}

	static iterator end(T enumerable){
		return std::end(enumerable);
	}

	static const_iterator cbegin(T enumerable){
		return std::begin(const_cast<const T>(enumerable));
	}

	static const_iterator cend(T enumerable){
		return std::end(const_cast<const T>(enumerable));
	}
};

}
}
