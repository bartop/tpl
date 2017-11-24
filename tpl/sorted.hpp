
#pragma once

#include "meta/is_enumerable.hpp"
#include "meta/is_associative.hpp"

#include <iterator>
#include <set>

namespace tpl{

namespace detail{

template<class ComparePredicate>
struct compare_holder {
	compare_holder(ComparePredicate comparePredicate) :
		m_comparePredicate(std::move(comparePredicate)){}

	ComparePredicate m_comparePredicate;
};

}

template<class Container, class ComparePredicate>
class sorted_sequence :
	public meta::choose_associative_base<typename std::remove_reference<Container>::type>,
	meta::enforce_enumerable<Container> {
public:
	using container_t = typename std::remove_reference<Container>::type;
	using value_type = typename container_t::value_type;
	using sorted_t = std::multiset<value_type, ComparePredicate>;
	using const_iterator = typename sorted_t::const_iterator;
	using iterator = typename sorted_t::iterator;

	sorted_sequence(
		Container &&container,
	   	ComparePredicate comparePredicate
	) :
		m_container(std::forward<Container>(container)),
		m_sorted(std::move(comparePredicate)){}

	void
	swap(sorted_sequence &other){
		std::swap(m_container, other.m_container);
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
				std::begin(m_container),
				std::end(m_container),
				std::inserter(m_sorted, m_sorted.begin())
			);
		}
	}

	mutable sorted_t m_sorted;
	Container m_container;
};

template<class ComparePredicate>
detail::compare_holder<ComparePredicate>
sort(ComparePredicate comparePredicate){
	return detail::compare_holder<ComparePredicate>(std::move(comparePredicate));
}

template<class Container, class ComparePredicate>
sorted_sequence<Container, ComparePredicate>
operator|(
	Container &&container,
   	detail::compare_holder<ComparePredicate> holder
){
	return sorted_sequence<Container, ComparePredicate>(
		std::forward<Container>(container),
	   	std::move(holder.m_comparePredicate)
	);
}

}
