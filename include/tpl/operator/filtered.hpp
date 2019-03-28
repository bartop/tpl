/**
 * \file
 * \brief File defining operator which filters given sequence.
 */
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"
#include "../meta/iterators.hpp"

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
	using iterator_category = typename meta::demote_to_bidirectional_tag<
		sub_traits_t
	>::type;

	filtering_iterator() = default;
	filtering_iterator(const filtering_iterator &) = default;
	filtering_iterator(filtering_iterator &&) = default;

	filtering_iterator &
	operator=(const filtering_iterator &) = default;
	filtering_iterator &
	operator=(filtering_iterator &&) = default;

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
				meta::make_reverse_iterator(m_subIterator),
				meta::make_reverse_iterator(enumerable_traits::begin(*m_enumerable)),
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

/**
 * \brief Sequence filtering given sequence using supplied function.
 *
 * This class can be safely used with infinite sequences.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable Type of sequence which is to be filtered. Must satisfy
 *      is_enumerable trait.
 * \tparam FilterPredicate Function-like type which will be used for filtering.
 *      It should take one argument convertible to Enumerable::value_type and
 *      return value of type convertible to bool.
 */
template<class Enumerable, class FilterPredicate>
class filtered_sequence : meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;

	//! Type of values returned from dereferencing iterators.
	using value_type = typename enumerable_traits::value_type;

	//! Type of const_iterator.
	using const_iterator = filtering_iterator<
		typename enumerable_traits::const_iterator,
		typename enumerable_traits::enumerable_type,
		typename std::remove_reference<FilterPredicate>::type
	>;

	//! Type of iterator.
	using iterator = filtering_iterator<
		typename enumerable_traits::iterator,
		typename enumerable_traits::enumerable_type,
		typename std::remove_reference<FilterPredicate>::type
	>;

	/**
	 * \brief Creates new filtered_sequence from given sequence and filtering
	 *      function.
	 *
	 * **Complexity** 
	 * - O(1) for rvalue references of enumerable
	 * - O(N) for lvalue references of enumerable (where N is size of enumerable)
	 *
	 * \param enumerable Sequence which is to be filtered.
	 * \param op Function returning false when element is to be filtered out and
	 *      false otherwise. This function CANNOT have side-effects.
	 */
	template<class T>
	filtered_sequence(
		Enumerable &&enumerable,
		T &&op
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_filterPredicate(std::forward<T>(op)) {}

	/**
	 * \brief Creates and returns iterator pointing at the begin.
	 */
	iterator
	begin() {
		return iterator(
			enumerable_traits::begin(m_enumerable),
		   	&m_enumerable,
			m_filterPredicate
		);
	}

	/**
	 * \brief Creates and returns iterator pointing at the end.
	 */
	iterator
	end() {
		return iterator(
			enumerable_traits::end(m_enumerable),
		   	&m_enumerable,
			m_filterPredicate
		);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the begin.
	 */
	const_iterator
	begin() const {
		return const_iterator(
			enumerable_traits::begin(m_enumerable),
		   	&m_enumerable,
			m_filterPredicate
		);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the end.
	 */
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


/**
 * \brief Piping operator filtering input sequence using supplied
 *     function.
 *
 * This operator can be safely used with infinite sequences.
 *
 * \tparam FilterPredicate Function-like type which will be used for filtering.
 *      It should take one argument convertible to value_type of input sequence
 *      and return value of type convertible to bool.
 *
 * \param predicate Function returning false when element is to be
 *     filtered out and false otherwise. Accepted type of this function
 *     should be constructible from value_type of input sequence. This
 *     function CANNOT have side-effects.
 *
 * **Example**
 *     
 *     std::vector<int> v = {1, 2, 3, 4, 5};
 *     const auto filtered = v | tpl::filter([](auto i){ return i > 2; });
 *     for (auto val : filtered)
 *         std::cout << val << ", "; //displays 3, 4, 5, 
 */
template<class Predicate>
filter_factory<Predicate>
filter(Predicate &&predicate){
	return filter_factory<Predicate>(
		std::forward<Predicate>(predicate)
	);
}

}
