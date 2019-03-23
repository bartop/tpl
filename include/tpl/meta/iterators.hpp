
#pragma once

#include <iterator>
#include <type_traits>

namespace tpl{
namespace meta{

template<class T>
using is_random_access_iterator = std::is_same<
	typename T::iterator_category,
	std::random_access_iterator_tag
>;

template<class T>
using is_bidirectional_iterator = std::is_same<
	typename T::iterator_category,
	std::bidirectional_iterator_tag
>;

template<class T>
using is_forward_iterator = std::is_same<
	typename T::iterator_category,
	std::forward_iterator_tag
>;

template<class T>
using is_input_iterator = std::is_same<
	typename T::iterator_category,
	std::input_iterator_tag
>;

template<class T>
using is_output_iterator = std::is_same<
	typename T::iterator_category,
	std::output_iterator_tag
>;

template<class T>
using demote_to_bidirectional_tag = std::conditional<
	is_random_access_iterator<T>::value,
	std::bidirectional_iterator_tag,
	typename T::iterator_category
>;

template<class T>
struct type_wrapper{
	using type = T;	
};
	
template<class>
using demote_to_input_tag = type_wrapper<std::input_iterator_tag>;

template<class Iterator>
std::reverse_iterator<Iterator> make_reverse_iterator(Iterator iter){
	return std::reverse_iterator<Iterator>(std::move(iter));
}

}
}
