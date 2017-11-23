
#include "type_sink.hpp"

#include <type_traits>

namespace tpl{

namespace meta{

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
struct associative_base {
	using mapped_type = typename T::mapped_type;
	using key_type = typename T::key_type;
};

struct empty_base { };

template<class T>
using choose_associative_base = typename std::conditional<is_associative<T>::value, associative_base<T>, empty_base>::type;

template<class T>
struct enforce_associative {
	static_assert(is_associative<T>::value, "Type given to enforce_associative must have defined mapped_type and key_type");
	using type = T;
};

}

}
