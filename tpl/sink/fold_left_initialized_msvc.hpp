
#pragma once

#include "meta/enumerable_traits.hpp"

#include <iterator>
#include <numeric>

namespace tpl{
namespace sink{

template<class BinaryPredicate, class InitialValue>
struct fold_left_arguments_holder {
	fold_left_arguments_holder(
		BinaryPredicate &&predicate,
		InitialValue &&initialValue
	) :
		m_predicate(std::forward<BinaryPredicate>(predicate)),
		m_initialValue(std::forward<InitialValue>(initialValue)){}

	BinaryPredicate m_predicate;
	InitialValue m_initialValue;
};

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

	operator InitialValue () const {
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

template<class BinaryPredicate, class InitialValue>
fold_left_arguments_holder<BinaryPredicate, InitialValue>
fold_left(BinaryPredicate &&predicate, InitialValue &&initialValue){
	return fold_left_arguments_holder<BinaryPredicate, InitialValue>(
		std::forward<BinaryPredicate>(predicate),
		std::forward<InitialValue>(initialValue)
	);
}

template<class Enumerable, class BinaryPredicate, class InitialValue>
initialized_fold_left<Enumerable, BinaryPredicate, InitialValue>
operator|(
	Enumerable &&enumerable,
   	fold_left_arguments_holder<BinaryPredicate, InitialValue> &&holder
){
	return initialized_fold_left<Enumerable, BinaryPredicate, InitialValue>(
		std::forward<Enumerable>(enumerable),
	   	std::forward<fold_left_arguments_holder<BinaryPredicate, InitialValue>>(holder).m_predicate,
	   	std::forward<fold_left_arguments_holder<BinaryPredicate, InitialValue>>(holder).m_initialValue
	);
}

}
}
