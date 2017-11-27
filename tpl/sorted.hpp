
#pragma once

#include "meta/is_enumerable.hpp"
#include "meta/is_associative.hpp"
#include "meta/enumerable_traits.hpp"

#include <iterator>
#include <set>

namespace tpl{

template<class ComparePredicate>
struct compare_holder {
	explicit compare_holder(ComparePredicate comparePredicate) :
		m_comparePredicate(std::move(comparePredicate)){}

	ComparePredicate m_comparePredicate;
};

template<class Enumerable, class ComparePredicate>
class sorted_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using sorted_t = std::multiset<value_type, ComparePredicate>;
	using const_iterator = typename sorted_t::const_iterator;
	using iterator = typename sorted_t::iterator;

	sorted_sequence(
		Enumerable &&enumerable,
	   	ComparePredicate comparePredicate
	) :
		m_sorted(std::move(comparePredicate)),
		m_enumerable(std::forward<Enumerable>(enumerable)){ }

	void
	swap(sorted_sequence &other){
		std::swap(m_enumerable, other.m_enumerable);
		std::swap(m_sorted, other.m_sorted);
	}

	iterator
	begin() {
		fillCache();
		return std::begin(m_sorted);
	}

	iterator
	end() {
		fillCache();
		return std::end(m_sorted);
	}

	const_iterator
	begin() const {
		fillCache();
		return std::begin(m_sorted);
	}

	const_iterator
	end() const {
		fillCache();
		return std::end(m_sorted);
	}

private:
	void fillCache() const {
		if(m_sorted.empty()) {
			std::copy(
				std::begin(m_enumerable),
				std::end(m_enumerable),
				std::inserter(m_sorted, m_sorted.begin())
			);
		}
	}

	mutable sorted_t m_sorted;
	Enumerable m_enumerable;
};

template<class ComparePredicate>
compare_holder<ComparePredicate>
sort(ComparePredicate comparePredicate){
	return compare_holder<ComparePredicate>(std::move(comparePredicate));
}

template<class Enumerable, class ComparePredicate>
sorted_sequence<Enumerable, ComparePredicate>
operator|(
	Enumerable &&enumerable,
   	compare_holder<ComparePredicate> holder
){
	return sorted_sequence<Enumerable, ComparePredicate>(
		std::forward<Enumerable>(enumerable),
	   	std::move(holder.m_comparePredicate)
	);
}

}
