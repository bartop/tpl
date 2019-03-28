/**
 * \file
 * \brief File defining sink which copies all elements to given output iterator.
 */

#pragma once

#include "../common/sink.hpp"
#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include "../meta/enumerable_traits.hpp"

#include <algorithm>

namespace tpl{

template<class Enumerable>
class copied {
public:
	Enumerable &
	operator()(Enumerable &enumerable) {
		return enumerable;
	}

	const Enumerable &
	operator()(const Enumerable &enumerable) const {
		return enumerable;
	}
};

template<class Enumerable, class OutputIterator>
void
copy_to_function(const Enumerable &enumerable, OutputIterator &&outputIterator){
	using traits = meta::enumerable_traits<Enumerable>;
	std::copy(traits::begin(enumerable), traits::end(enumerable), outputIterator);
}

template<class OutputIterator>
class copy_to_factory {
public:
	explicit copy_to_factory(OutputIterator &&outputIterator) :
		m_outputIterator(std::forward<OutputIterator>(outputIterator)){}

	template<class Enumerable>
	sink<Enumerable, copied<Enumerable>>
	create(Enumerable &&enumerable) const & {
		copy_to_function(
			enumerable,
			m_outputIterator
		);
		return make_sink(
			std::forward<Enumerable>(enumerable),
		   	copied<Enumerable>()
		);
	}

	template<class Enumerable>
	sink<Enumerable, copied<Enumerable>>
	create(Enumerable &&enumerable) && {
		copy_to_function(
			enumerable,
			std::forward<OutputIterator>(m_outputIterator)
		);
		return make_sink(
			std::forward<Enumerable>(enumerable),
		   	copied<Enumerable>()
		);
	}
private:
	OutputIterator m_outputIterator;
};


/**
 * \brief Sink copying elements from input sequence to given output iterator.
 *
 * This is a sink, which means it can be used as final part of a pipeline.
 *
 * \tparam OutputIterator Type of output iterator. Its value_type must be
 *     constructible from input sequence value_type.
 *
 * \param outputIterator Iterator to which is copied input sequence.
 *
 *  **Example**
 *
 *     std::vector<int> v = {1, 2, 3, 4, 5};
 *     v | tpl::copy_to(std::ostream_iterator<int>(std::cout, ", "));
 *     //prints 1, 2, 3, 4, 5, 
 */
template<class OutputIterator>
copy_to_factory<OutputIterator>
copy_to(OutputIterator &&outputIterator){
	return copy_to_factory<OutputIterator>(
		std::forward<OutputIterator>(outputIterator)
	);
}

}
