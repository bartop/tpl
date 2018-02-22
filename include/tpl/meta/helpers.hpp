
#pragma once

namespace tpl {
namespace meta {

template<class T, class R = void>
struct enable_if_type { typedef R type; };

template<class T, class Enable = void>
struct is_defined : std::false_type {};

template<class T>
struct is_defined<T, typename enable_if_type<T>::type> : std::true_type {};

template<class ...> struct type_sink { using type = void; };

}
}
