
#pragma once

#include "../meta/enumerable_traits.hpp"

#include <algorithm>
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
		BinaryPredicate &&predicate,
	   	InitialValue &&initialValue
	) :
		m_predicate(std::forward<BinaryPredicate>(predicate)),
		m_initialValue(std::forward<InitialValue>(initialValue)){}

	using conversion_type = ConversionType;

	conversion_type
	operator()(const Enumerable &enumerable) const {
		using traits = meta::enumerable_traits<Enumerable>;
		return std::accumulate(
			traits::begin(enumerable),
		   	traits::end(enumerable),
		   	m_initialValue,
		   	m_predicate
		);
	}

	conversion_type
	operator()(Enumerable &enumerable) {
		using traits = meta::enumerable_traits<Enumerable>;
		return std::accumulate(
			traits::begin(enumerable),
		   	traits::end(enumerable),
		   	m_initialValue,
		   	m_predicate
		);
	}

private:
	BinaryPredicate m_predicate;
	InitialValue m_initialValue;
};

}
