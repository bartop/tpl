
#pragma once

#include <iterator>

namespace tpl{
namespace sink{

template<class Enumerable, class OutputIterator>
void
copy_to_function(Enumerable &&enumerable, OutputIterator &&outputIterator){
	std::copy(std::begin(enumerable), std::end(enumerable), outputIterator);
}

template<class OutputIterator>
struct copy_to_iterator_holder {
	explicit copy_to_iterator_holder(OutputIterator &&outputIterator) :
		m_outputIterator(std::forward<OutputIterator>(outputIterator)){}

	OutputIterator m_outputIterator;
};

template<class OutputIterator>
copy_to_iterator_holder<OutputIterator>
copy_to(OutputIterator &&outputIterator){
	return copy_to_iterator_holder<OutputIterator>(
		std::forward<OutputIterator>(outputIterator)
	);
}

template<class Enumerable, class OutputIterator>
void
operator|(
	Enumerable &&enumerable,
   	copy_to_iterator_holder<OutputIterator> &&holder
){
	copy_to_function(
		std::forward<Enumerable>(enumerable),
	   	std::forward<copy_to_iterator_holder<OutputIterator>>(holder).m_outputIterator
	);
}

}
}
