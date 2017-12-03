
#pragma once

#include "meta/enumerable_traits.hpp"

#include <iterator>
#include <numeric>

namespace tpl{


template<class BinaryPredicate>
struct fold_left_predicate_holder {
	fold_left_predicate_holder(
		BinaryPredicate &&predicate
	) :
		m_predicate(std::forward<BinaryPredicate>(predicate)){}

	BinaryPredicate m_predicate;
};

template<class Enumerable, class BinaryPredicate>
class default_fold_left {
public:
	default_fold_left(
		Enumerable &&enumerable,
		BinaryPredicate &&predicate
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_predicate(std::forward<BinaryPredicate>(predicate)) {}

	using conversion_type = decltype(
		std::declval<BinaryPredicate>()(
		   	std::declval<typename meta::enumerable_traits<Enumerable>::value_type>() ,
		   	std::declval<typename meta::enumerable_traits<Enumerable>::value_type>()
		)
	);

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


template<class BinaryPredicate>
fold_left_predicate_holder<BinaryPredicate>
fold_left(BinaryPredicate &&predicate){
	return fold_left_predicate_holder<BinaryPredicate>(
		std::forward<BinaryPredicate>(predicate)
	);
}

template<class Enumerable, class BinaryPredicate>
default_fold_left<Enumerable, BinaryPredicate>
operator|(
	Enumerable &&enumerable,
   	fold_left_predicate_holder<BinaryPredicate> &&holder
){
	return default_fold_left<Enumerable, BinaryPredicate>(
		std::forward<Enumerable>(enumerable),
	   	std::forward<fold_left_predicate_holder<BinaryPredicate>>(holder).m_predicate
	);
}

}
