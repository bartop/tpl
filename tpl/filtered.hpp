
#pragma once

#include "cache.hpp"

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
class filtering_iterator :
	public std::iterator<std::input_iterator_tag, typename SubIterator::value_type> {
public:
	using self_t = filtering_iterator;
	using traits_t = std::iterator_traits<self_t>;

	~filtering_iterator() noexcept = default;

	filtering_iterator(
		SubIterator subIterator,
		SubIterator endIterator,
		FilterPredicate filterPredicate
	) :
		m_subIterator(std::find_if(subIterator, endIterator, filterPredicate)),
		m_endIterator(std::move(endIterator)),
		m_filterPredicate(std::move(filterPredicate)) {}

	filtering_iterator(const filtering_iterator &filteringIterator) :
	   	m_subIterator(filteringIterator.m_subIterator),
	   	m_endIterator(filteringIterator.m_endIterator),
		m_filterPredicate(filteringIterator.m_filterPredicate){}

	filtering_iterator(filtering_iterator &&other) :
		m_subIterator(std::move(other.m_subIterator)),
		m_endIterator(std::move(other.m_endIterator)),
		m_filterPredicate(std::move(other.m_filterPredicate)) {}

	filtering_iterator &
	operator=(filtering_iterator other) {
		this->swap(other);
		return *this;
	}

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

	const typename traits_t::value_type &
	operator*() {
		return *(this->m_subIterator);
	}

	const typename traits_t::value_type *
	operator->() {
		return this->m_subIterator.operator->();
	}

	const typename traits_t::value_type &
	operator*() const {
		return *(this->m_subIterator);
	}

	const typename traits_t::value_type *
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

template<class SubIterator, class FilterPredicate>
filtering_iterator<SubIterator, FilterPredicate>
make_filtering_iterator(
	SubIterator subIterator,
	SubIterator endIterator,
	FilterPredicate filterPredicate
){
	return filtering_iterator<SubIterator, FilterPredicate>(
		std::move(subIterator),
	   	std::move(endIterator),
	   	std::move(filterPredicate)
	);
}

template<class Container, class FilterPredicate>
class filtered_sequence {
public:
	using iterator = filtering_iterator<typename Container::iterator, FilterPredicate>;
	using const_iterator = filtering_iterator<typename Container::const_iterator, FilterPredicate>;

	filtered_sequence(const filtered_sequence &filteredSequence) :
	   	m_container(filteredSequence.m_container),
		m_filterPredicate(filteredSequence.m_filterPredicate){}

	filtered_sequence(filtered_sequence &&filteredSequence) :
		m_container(std::move(filteredSequence.m_container)),
		m_filterPredicate(std::move(filteredSequence.m_filterPredicate)){}

	filtered_sequence(
		Container container,
		FilterPredicate predicate
	) :
		m_container(std::move(container)),
		m_filterPredicate(std::move(predicate)){}

	filtered_sequence &
	operator=(filtered_sequence other) {
		this->swap(other);
		return *this;
	}

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
operator>>(
	Container container,
   	detail::filter_holder<FilterPredicate> holder
){
	return filtered_sequence<Container, FilterPredicate>(std::move(container), std::move(holder.m_filterPredicate));
}

}
