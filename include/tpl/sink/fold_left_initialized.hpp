
#pragma once

#include "../meta/enumerable_traits.hpp"

#include <iterator>
#include <numeric>

namespace tpl{

template<
	class Enumerable,
	class BinaryPredicate,
   	class InitialValue,
	class ConversionType = decltype(
		std::declval<BinaryPredicate>()(
			std::declval<InitialValue>(),
			std::declval<typename meta::enumerable_traits<Enumerable>::value_type>()
		)
	)
>
class initialized_fold_left {
public:
	initialized_fold_left(
		Enumerable &&enumerable,
		BinaryPredicate &&predicate,
	   	InitialValue &&initialValue
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_predicate(std::forward<BinaryPredicate>(predicate)),
		m_initialValue(std::forward<InitialValue>(initialValue)){}

	using conversion_type = ConversionType;

	operator conversion_type() const {
		return std::accumulate(std::begin(m_enumerable), std::end(m_enumerable), m_initialValue, m_predicate);
	}

private:
	Enumerable m_enumerable;
	BinaryPredicate m_predicate;
	InitialValue m_initialValue;
};

}
