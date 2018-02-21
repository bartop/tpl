
#pragma once

#include <iterator>

#include "../common/composite_factory.hpp"

namespace tpl{

template<class Enumerable>
class number_of_copied {
public:
	number_of_copied(Enumerable &&enumerable) :
		m_enumerable(std::forward<Enumerable>(enumerable)) {}

	unsigned
	result() const {
		return std::distance(std::begin(m_enumerable), std::end(m_enumerable));
	}

private:
	Enumerable m_enumerable;
};

template<class Enumerable, class OutputIterator>
void
copy_to_function(const Enumerable &enumerable, OutputIterator &&outputIterator){
	std::copy(std::begin(enumerable), std::end(enumerable), outputIterator);
}

template<class OutputIterator>
class copy_to_factory {
public:
	explicit copy_to_factory(OutputIterator &&outputIterator) :
		m_outputIterator(std::forward<OutputIterator>(outputIterator)){}

	template<class Enumerable>
	number_of_copied<Enumerable>
	create(Enumerable &&enumerable) const & {
		copy_to_function(
			enumerable,
			m_outputIterator
		);
		return number_of_copied<Enumerable>(std::forward<Enumerable>(enumerable));
	}

	template<class Enumerable>
	number_of_copied<Enumerable>
	create(Enumerable &&enumerable) && {
		copy_to_function(
			enumerable,
			std::forward<OutputIterator>(m_outputIterator)
		);
		return number_of_copied<Enumerable>(std::forward<Enumerable>(enumerable));
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

template<
	class Enumerable,
	class OutputIterator,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
void
operator|(Enumerable &&enumerable, copy_to_factory<OutputIterator> &&factory){
	std::forward<copy_to_factory<OutputIterator>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
	class OutputIterator,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
void
operator|(Enumerable &&enumerable, const copy_to_factory<OutputIterator> &factory){
	factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

}
