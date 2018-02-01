
#pragma once

#include "meta/enumerable_traits.hpp"

#include <iterator>
#include <numeric>

namespace tpl{

template<class Enumerable, class BinaryPredicate, class InitialValue>
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

	using conversion_type = decltype(
		std::declval<BinaryPredicate>()(
			std::declval<InitialValue>(),
		   	std::declval<typename meta::enumerable_traits<Enumerable>::value_type>()
		)
	);

	operator conversion_type() const {
		return std::accumulate(std::begin(m_enumerable), std::end(m_enumerable), m_initialValue, m_predicate);
	}

	void
	swap(initialized_fold_left &other){
		std::swap(m_enumerable, other.m_enumerable);
		std::swap(m_predicate, other.m_predicate);
		std::swap(m_initialValue, other.m_initialValue);
	}

private:
	Enumerable m_enumerable;
	BinaryPredicate m_predicate;
	InitialValue m_initialValue;
};

}
