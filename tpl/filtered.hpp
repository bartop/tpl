
#pragma once

#include "meta/is_enumerable.hpp"
#include "meta/is_associative.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

namespace detail{

template<class FilterPredicate>
struct filter_holder {
	filter_holder(FilterPredicate filterPredicate) :
		m_filterPredicate(std::move(filterPredicate)){}

	FilterPredicate m_filterPredicate;
};

}

template<class SubIterator, class FilterPredicate>
class filtering_iterator {
public:
	using sub_traits_t = std::iterator_traits<SubIterator>;
	using value_type = typename sub_traits_t::value_type;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	~filtering_iterator() noexcept = default;

	filtering_iterator(
		SubIterator subIterator,
		SubIterator endIterator,
		FilterPredicate filterPredicate
	) :
		m_subIterator(std::find_if(subIterator, endIterator, filterPredicate)),
		m_endIterator(std::move(endIterator)),
		m_filterPredicate(std::move(filterPredicate)) {}

	filtering_iterator &
	operator++() {
		++m_subIterator;
		m_subIterator = std::find_if(m_subIterator, m_endIterator, m_filterPredicate);
		return *this;
	}

	filtering_iterator
	operator++(int) {
		const auto thisCopy = *this;
		++*this;
		return thisCopy;
	}

	reference
	operator*() const {
		return *(this->m_subIterator);
	}

	pointer
	operator->() const {
		return this->m_subIterator.operator->();
	}

	bool
	operator==(const filtering_iterator &filteringIterator) const {
		return this->m_subIterator == filteringIterator.m_subIterator;
	}

	bool
	operator!=(const filtering_iterator &filteringIterator) const {
		return this->m_subIterator != filteringIterator.m_subIterator;
	}

	void swap(filtering_iterator &filteringIterator) {
		std::swap(this->m_subIterator, filteringIterator.m_subIterator);
		std::swap(this->m_endIterator, filteringIterator.m_endIterator);
		std::swap(filteringIterator.m_filterPredicate, this->m_filterPredicate);
	}
private:
	SubIterator m_subIterator;
	SubIterator m_endIterator;
	FilterPredicate m_filterPredicate;
};

template<class Container, class FilterPredicate>
class filtered_sequence :
	public meta::choose_associative_base<typename std::remove_reference<Container>::type>,
	meta::enforce_enumerable<Container> {
public:
	using container_t = typename std::remove_reference<Container>::type;
	using value_type = typename container_t::value_type;
	using const_iterator = filtering_iterator<typename container_t::const_iterator, FilterPredicate>;
	using iterator = filtering_iterator<typename container_t::iterator, FilterPredicate>;

	filtered_sequence(
		Container &&container,
		FilterPredicate predicate
	) :
		m_container(std::forward<Container>(container)),
		m_filterPredicate(std::move(predicate)){}

	void
	swap(filtered_sequence &other){
		std::swap(m_container, other.m_container);
		std::swap(m_filterPredicate, other.m_filterPredicate);
	}

	iterator
	begin() {
		return iterator(std::begin(m_container), std::end(m_container), m_filterPredicate);
	}

	iterator
	end() {
		return iterator(std::end(m_container), std::end(m_container), m_filterPredicate);
	}

	const_iterator
	begin() const {
		return const_iterator(std::begin(m_container), std::end(m_container), m_filterPredicate);
	}

	const_iterator
	end() const {
		return const_iterator(std::end(m_container), std::end(m_container), m_filterPredicate);
	}
private:
	Container m_container;
	FilterPredicate m_filterPredicate;
};

template<class FilterPredicate>
detail::filter_holder<FilterPredicate>
filter(FilterPredicate filterPredicate){
	return detail::filter_holder<FilterPredicate>(filterPredicate);
}

template<class Container, class FilterPredicate>
filtered_sequence<Container, FilterPredicate>
operator|(
	Container &&container,
   	detail::filter_holder<FilterPredicate> holder
){
	return filtered_sequence<Container, FilterPredicate>(
		std::forward<Container>(container),
	   	std::move(holder.m_filterPredicate)
	);
}

}
