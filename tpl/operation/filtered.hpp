
#pragma once

#include "meta/is_enumerable.hpp"
#include "meta/is_associative.hpp"
#include "meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include "../common/composite_factory.hpp"

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

template<
	class Enumerable,
	class FilterPredicate,
   	class = typename std::enable_if<meta::is_enumerable<Enumerable>::value>::type
>
filtered_sequence<Enumerable, FilterPredicate>
operator|(Enumerable &&enumerable, const filter_factory<FilterPredicate> &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
	class FilterPredicate,
   	class = typename std::enable_if<meta::is_enumerable<Enumerable>::value>::type
>
filtered_sequence<Enumerable, FilterPredicate>
operator|(Enumerable &&enumerable, filter_factory<FilterPredicate> &&factory){
	return std::move(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Factory, class FilterPredicate>
composite_factory<const filter_factory<FilterPredicate> &, Factory>
operator|(const filter_factory<FilterPredicate> &factory, Factory &&other){
	return make_composite(factory, std::forward<Factory>(other));
}

template<class Factory, class FilterPredicate>
composite_factory<filter_factory<FilterPredicate>, Factory>
operator|(filter_factory<FilterPredicate> &&factory, Factory &&other){
	return make_composite(std::move(factory), std::forward<Factory>(other));
}

}
