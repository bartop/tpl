
#pragma once

#include <iterator>

namespace tpl{

template<class Enumerable, class OutputIterator>
void
copy_to_function(Enumerable &&enumerable, OutputIterator &&outputIterator){
	std::copy(std::begin(enumerable), std::end(enumerable), outputIterator);
}

template<class OutputIterator>
class copy_to_factory {
public:
	explicit copy_to_factory(OutputIterator &&outputIterator) :
		m_outputIterator(std::forward<OutputIterator>(outputIterator)){}

	template<class Enumerable>
	void
	create(Enumerable &&enumerable) const & {
		copy_to_function(
			std::forward<Enumerable>(enumerable),
			m_outputIterator
		);
	}

	template<class Enumerable>
	void
	create(Enumerable &&enumerable) && {
		copy_to_function(
			std::forward<Enumerable>(enumerable),
			std::forward<OutputIterator>(m_outputIterator)
		);
	}
private:
	OutputIterator m_outputIterator;
};

template<class OutputIterator>
copy_to_factory<OutputIterator>
copy_to(OutputIterator &&outputIterator){
	return copy_to_factory<OutputIterator>(
		std::forward<OutputIterator>(outputIterator)
	);
}

template<class Enumerable, class OutputIterator>
void
operator|(
	Enumerable &&enumerable,
   	copy_to_factory<OutputIterator> &&factory
){
	std::forward<copy_to_factory<OutputIterator>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

}
