
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <algorithm>
#include <functional>

namespace tpl{

template<class SubIterator, class Enumerable, class FilterPredicate>
class filtering_iterator :
	public detail::bidirectional_iterator_base<
		filtering_iterator<SubIterator, Enumerable, FilterPredicate>
	> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using sub_traits_t = std::iterator_traits<SubIterator>;
	using value_type = typename sub_traits_t::value_type;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	filtering_iterator() = default;

	~filtering_iterator() noexcept = default;

	filtering_iterator(
		SubIterator subIterator,
		const Enumerable *enumerable,
		const FilterPredicate &filterPredicate
	) :
		m_subIterator(std::find_if(subIterator, enumerable_traits::end(*enumerable), filterPredicate)),
		m_enumerable(enumerable),
		m_filterPredicate(&filterPredicate) {}

	filtering_iterator &
	next() {
		++m_subIterator;
		m_subIterator = std::find_if(m_subIterator, enumerable_traits::end(*m_enumerable), *m_filterPredicate);
		return *this;
	}

	filtering_iterator &
	previous() {
		auto found = std::find_if(
				std::reverse_iterator<SubIterator>(m_subIterator),
				std::reverse_iterator<SubIterator>(enumerable_traits::begin(*m_enumerable)),
				*m_filterPredicate
		);
		m_subIterator = --found.base();
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
private:
	SubIterator m_subIterator;
	const Enumerable *m_enumerable;
	const FilterPredicate *m_filterPredicate;
};

template<class Enumerable, class FilterPredicate>
class filtered_sequence : meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = filtering_iterator<
		typename enumerable_traits::const_iterator,
		typename enumerable_traits::enumerable_type,
		typename std::remove_reference<FilterPredicate>::type
	>;
	using iterator = filtering_iterator<
		typename enumerable_traits::iterator,
		typename enumerable_traits::enumerable_type,
		typename std::remove_reference<FilterPredicate>::type
	>;

	template<class T>
	filtered_sequence(
		Enumerable &&enumerable,
		T &&op
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_filterPredicate(std::forward<T>(op)) {}

	iterator
	begin() {
		return iterator(
			enumerable_traits::begin(m_enumerable),
		   	&m_enumerable,
			m_filterPredicate
		);
	}

	iterator
	end() {
		return iterator(
			enumerable_traits::end(m_enumerable),
		   	&m_enumerable,
			m_filterPredicate
		);
	}

	const_iterator
	begin() const {
		return const_iterator(
			enumerable_traits::begin(m_enumerable),
		   	&m_enumerable,
			m_filterPredicate
		);
	}

	const_iterator
	end() const {
		return const_iterator(
			enumerable_traits::end(m_enumerable),
		   	&m_enumerable,
			m_filterPredicate
		);
	}

private:
	Enumerable m_enumerable;
	FilterPredicate m_filterPredicate;
};

template<class Enumerable, class FilterPredicate>
filtered_sequence<Enumerable, FilterPredicate>
make_filtered(Enumerable &&enumerable, FilterPredicate &&predicate){
	return filtered_sequence<Enumerable, FilterPredicate>(
		std::forward<Enumerable>(enumerable),
		std::forward<FilterPredicate>(predicate)
	);
}

template<class FilterPredicate>
class filter_factory {
public:
	filter_factory(FilterPredicate &&filterPredicate) :
		m_filterPredicate(std::forward<FilterPredicate>(filterPredicate)){}

	template<class Enumerable>
	filtered_sequence<Enumerable, const FilterPredicate &>
	create(Enumerable &&enumerable) const & {
		return make_filtered(
			std::forward<Enumerable>(enumerable),
			m_filterPredicate
		);
	}

	template<class Enumerable>
	filtered_sequence<Enumerable, FilterPredicate>
	create(Enumerable &&enumerable) && {
		return make_filtered(
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

}
