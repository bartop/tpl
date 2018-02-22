
#pragma once

#include "../common/sink.hpp"
#include "../common/composite_factory.hpp"

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
	std::copy(std::begin(enumerable), std::end(enumerable), outputIterator);
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
sink<Enumerable, copied<Enumerable>>
operator|(Enumerable &&enumerable, copy_to_factory<OutputIterator> &&factory){
	return std::forward<copy_to_factory<OutputIterator>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
	class OutputIterator,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
sink<Enumerable, copied<Enumerable>>
operator|(Enumerable &&enumerable, const copy_to_factory<OutputIterator> &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

}
