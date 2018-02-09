
#pragma once

#include "meta/enumerable_traits.hpp"

#include <iterator>
#include <numeric>

namespace tpl{

template<
	class Enumerable,
	class BinaryPredicate,
   	class ConversionType = decltype(
		std::declval<BinaryPredicate>()(
		   	std::declval<typename meta::enumerable_traits<Enumerable>::value_type>(),
		   	std::declval<typename meta::enumerable_traits<Enumerable>::value_type>()
		)
	)
>
class default_fold_left {
public:
	default_fold_left(
		Enumerable &&enumerable,
		BinaryPredicate &&predicate
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_predicate(std::forward<BinaryPredicate>(predicate)) {}

	using conversion_type = ConversionType;

	operator conversion_type() const {
		return std::accumulate(
			std::begin(m_enumerable),
		   	std::end(m_enumerable),
		   	typename meta::enumerable_traits<Enumerable>::value_type(),
			m_predicate
		);
	}

	void
	swap(default_fold_left &other){
		std::swap(m_enumerable, other.m_enumerable);
		std::swap(m_predicate, other.m_predicate);
	}

private:
	Enumerable m_enumerable;
	BinaryPredicate m_predicate;
};

}
