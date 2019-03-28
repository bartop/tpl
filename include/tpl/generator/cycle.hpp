/**
 * \file
 * \brief File defining cycle sequence generator.
 */
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include <algorithm>

namespace tpl{

template<class Enumerable>
class cycle_iterator : public detail::input_iterator_base<cycle_iterator<Enumerable>> {
public:
	using enumerable_traits = meta::enumerable_traits<typename std::decay<Enumerable>::type>;
	using value_type = typename enumerable_traits::value_type;
	using difference_type = ptrdiff_t;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	cycle_iterator() = default;

	~cycle_iterator() noexcept = default;

	cycle_iterator(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
   		m_currentIterator(enumerable.begin()) {}

	cycle_iterator &
	operator++() {
		++m_currentIterator;
		if (m_currentIterator == m_enumerable.end())
			m_currentIterator = m_enumerable.begin();

		return *this;
	}

	reference
	operator*() const {
		return *m_currentIterator;
	}

	pointer
	operator->() const {
		return m_currentIterator.operator->();
	}

	bool
	operator==(const cycle_iterator &) const {
		return false;
	}

private:
	Enumerable m_enumerable;
	typename enumerable_traits::const_iterator m_currentIterator;
};

/**
 * \brief Infinite sequence generating repeating sequence of elements. E.g. 
 *	    given sequence `{ 1, 2 }` the output sequence would be `{ 1, 2, 1, 2, 1,... }`.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable Type of sequence to be repeated. Must satisfy is_enumerable trait.
 */
template<class Enumerable>
class cycle_sequence : meta::enforce_enumerable<Enumerable> {
public:
	//! Type of values returned from dereferencing iterators.
	using value_type = typename meta::enumerable_traits<typename std::decay<Enumerable>::type>::value_type;

	//! Type of const_iterator.
	using const_iterator = cycle_iterator<Enumerable>;

	//! Type of iterator.
	using iterator = const_iterator;

	/**
	 * \brief Creates new cycle_sequence from given enumerable.
	 *
	 * Complexity:  
	 * - O(1) for rvalue references (std::move is prefered)
	 * - O(N) for lvalue references (where N is size of enumerable)
	 *
	 * \param enumerable Sequence to be infinitely repeated.
	 */
	cycle_sequence(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)){}

	//! Creates and returns iterator pointing at the begin.
	/**
	 * O(N) complexity (where N is size of stored enumerable)
	 */
	iterator
	begin() {
		return iterator(m_enumerable);
	}

	//! Creates and returns iterator pointing at the end.
	/**
	 * O(N) complexity (where N is size of stored enumerable)
	 */
	iterator
	end() {
		return iterator(m_enumerable);
	}

	//! Creates and returns const_iterator pointing at the begin.
	/**
	 * O(N) complexity (where N is size of stored enumerable)
	 */
	const_iterator
	begin() const {
		return iterator(m_enumerable);
	}

	//! Creates and returns const_iterator pointing at the end.
	/**
	 * O(N) complexity (where N is size of stored enumerable)
	 */
	const_iterator
	end() const {
		return iterator(m_enumerable);
	}

private:
	Enumerable m_enumerable;
};

/**
 * \brief Piping generator repeating infintely given sequence. E.g. given sequence 
 *      `{ 1, 2 }` the output sequence would be `{ 1, 2, 1, 2, 1,... }`.
 *
 * Complexity:
 * - O(1) for rvalue references (std::move is prefered) 
 * - O(N) for lvalue references (N is size of enumerable)
 *
 * \tparam Enumerable Type of sequence to be repeated. This type must satisfy is_enumerable trait.
 *
 * \param  enumerable Sequence to be repeated.
 *
 * \return cycle_equence infinitely repeating given sequence.
 */
template<class Enumerable>
cycle_sequence<Enumerable>
cycle(Enumerable &&enumerable){
	return cycle_sequence<Enumerable>(std::forward<Enumerable>(enumerable));
}

}
