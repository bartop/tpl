
#pragma once

#include "meta/is_enumerable.hpp"
#include "meta/is_associative.hpp"
#include "meta/enumerable_traits.hpp"

#include "../common/composition_operator.hpp"

#include <iterator>
#include <set>

namespace tpl{

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
	   	ComparePredicate &&comparePredicate
	) :
		m_sorted(std::forward<ComparePredicate>(comparePredicate)),
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
class compare_factory {
public:
	explicit compare_factory(ComparePredicate &&comparePredicate) :
		m_comparePredicate(std::forward<ComparePredicate>(comparePredicate)){}

	template<class Enumerable>
	auto
	create(Enumerable &&enumerable) const & {
		return sorted_sequence<Enumerable, const ComparePredicate &>(
			std::forward<Enumerable>(enumerable),
			m_comparePredicate
		);
	}

	template<class Enumerable>
	sorted_sequence<Enumerable, ComparePredicate>
	create(Enumerable &&enumerable) && {
		return sorted_sequence<Enumerable, ComparePredicate>(
			std::forward<Enumerable>(enumerable),
			std::forward<ComparePredicate>(m_comparePredicate)
		);
	}

private:
	ComparePredicate m_comparePredicate;
};

template<class ComparePredicate>
compare_factory<ComparePredicate>
sort(ComparePredicate &&comparePredicate){
	return compare_factory<ComparePredicate>(std::forward<ComparePredicate>(comparePredicate));
}

}
