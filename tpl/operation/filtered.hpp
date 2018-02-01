
#pragma once

#include "meta/is_enumerable.hpp"
#include "meta/is_associative.hpp"
#include "meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class SubIterator, class FilterPredicate>
class filtering_iterator :
	public detail::iterator_base<filtering_iterator<SubIterator, FilterPredicate>> {
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

template<class Enumerable, class FilterPredicate>
class filtered_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = filtering_iterator<typename enumerable_traits::const_iterator, FilterPredicate>;
	using iterator = filtering_iterator<typename enumerable_traits::iterator, FilterPredicate>;

	filtered_sequence(
		Enumerable &&enumerable,
		FilterPredicate &&predicate
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_filterPredicate(std::forward<FilterPredicate>(predicate)){}

	void
	swap(filtered_sequence &other){
		std::swap(m_enumerable, other.m_enumerable);
		std::swap(m_filterPredicate, other.m_filterPredicate);
	}

	iterator
	begin() {
		return iterator(
			std::begin(m_enumerable),
		   	std::end(m_enumerable),
			m_filterPredicate
		);
	}

	iterator
	end() {
		return iterator(
			std::end(m_enumerable),
		   	std::end(m_enumerable),
			m_filterPredicate
		);
	}

	const_iterator
	begin() const {
		return const_iterator(
			std::begin(m_enumerable),
		   	std::end(m_enumerable),
			m_filterPredicate
		);
	}

	const_iterator
	end() const {
		return const_iterator(
			std::end(m_enumerable),
		   	std::end(m_enumerable),
			m_filterPredicate
		);
	}
private:
	Enumerable m_enumerable;
	FilterPredicate m_filterPredicate;
};

template<class FilterPredicate>
class filter_factory {
public:
	filter_factory(FilterPredicate &&filterPredicate) :
		m_filterPredicate(std::forward<FilterPredicate>(filterPredicate)){}

	template<class Enumerable>
	filtered_sequence<Enumerable, FilterPredicate>
	create(Enumerable &&enumerable) const & {
		return filtered_sequence<Enumerable, FilterPredicate>(
			std::forward<Enumerable>(enumerable),
			m_filterPredicate
		);
	}

	template<class Enumerable>
	filtered_sequence<Enumerable, FilterPredicate>
	create(Enumerable &&enumerable) && {
		return filtered_sequence<Enumerable, FilterPredicate>(
			std::forward<Enumerable>(enumerable),
			std::forward<FilterPredicate>(m_filterPredicate)
		);
	}
private:
	FilterPredicate m_filterPredicate;
};

template<class FilterPredicate>
filter_factory<FilterPredicate>
filter(FilterPredicate &&filterPredicate){
	return filter_factory<FilterPredicate>(std::forward<FilterPredicate>(filterPredicate));
}

template<class Enumerable, class FilterPredicate>
filtered_sequence<Enumerable, FilterPredicate>
operator|(
	Enumerable &&enumerable,
   	filter_factory<FilterPredicate> &&factory
){
	return std::forward<filter_factory<FilterPredicate>>(factory).create(
			std::forward<Enumerable>(enumerable)
	);
}

}
