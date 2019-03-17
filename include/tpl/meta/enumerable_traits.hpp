
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
	using enumerable_type = typename std::decay<T>::type;
	using value_type = typename enumerable_type::value_type;
	using iterator = typename enumerable_type::iterator;
	using const_iterator = typename enumerable_type::const_iterator;

	static iterator begin(enumerable_type &enumerable){
		return std::begin(enumerable);
	}

	static iterator end(enumerable_type &enumerable){
		return std::end(enumerable);
	}

	static const_iterator begin(const enumerable_type &enumerable){
		return std::begin(enumerable);
	}

	static const_iterator end(const enumerable_type &enumerable){
		return std::end(enumerable);
	}

	static const_iterator cbegin(enumerable_type &enumerable){
		return std::begin(const_cast<const enumerable_type &>(enumerable));
	}

	static const_iterator cend(enumerable_type &enumerable){
		return std::end(const_cast<const enumerable_type &>(enumerable));
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
	using enumerable_type = typename std::remove_reference<T>::type;
	using value_type = array_reference_value_type<T>;
	using iterator = value_type *;
	using const_iterator = const value_type *;

	static iterator begin(enumerable_type &enumerable){
		return std::begin(enumerable);
	}

	static iterator end(enumerable_type &enumerable){
		return std::end(enumerable);
	}

	static const_iterator cbegin(enumerable_type &enumerable){
		return std::begin(const_cast<const enumerable_type &>(enumerable));
	}

	static const_iterator cend(enumerable_type &enumerable){
		return std::end(const_cast<const enumerable_type &>(enumerable));
	}
};

}
}
