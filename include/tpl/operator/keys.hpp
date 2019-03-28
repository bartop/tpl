/**
 * \file
 * \brief File defining operator which extracts keys from associative sequence.
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

namespace tpl{

template<class SubIterator>
class keys_iterator : public detail::bidirectional_iterator_base<keys_iterator<SubIterator>> {
public:
	using sub_traits_t = std::iterator_traits<SubIterator>;
	using associative_traits_t = meta::associative_element_traits<typename sub_traits_t::value_type>;
	using value_type = typename associative_traits_t::key_type;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = value_type;
	using pointer = const value_type *;
	using iterator_category = typename meta::demote_to_bidirectional_tag<
		sub_traits_t
	>::type;

	keys_iterator() = default;
	keys_iterator(const keys_iterator &) = default;
	keys_iterator(keys_iterator &&) = default;
	
	keys_iterator &
	operator=(const keys_iterator &) = default;
	keys_iterator &
	operator=(keys_iterator &&) = default;

	~keys_iterator() noexcept = default;

	keys_iterator(
		SubIterator subIterator
	) :
		m_subIterator(std::move(subIterator)) { }

	keys_iterator &
	next() {
		++m_subIterator;
		return *this;
	}

	keys_iterator &
	previous() {
		++m_subIterator;
		return *this;
	}

	reference
	operator*() const {
		return associative_traits_t::key_value(*(this->m_subIterator));
	}

	pointer
	operator->() const {
		return &associative_traits_t::key_value(*(this->m_subIterator));
	}

	bool
	operator==(const keys_iterator &other) const {
		return this->m_subIterator == other.m_subIterator;
	}

private:
	SubIterator m_subIterator;
};

/**
 * \brief Sequence extracting keys from input associative sequence.
 *
 * This class can be safely used with infinite sequences.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable Type of sequence from which keys are to be extracted. Must
 *      satisfy is_enumerable and is_associative trait.
 */
template<class Enumerable>
class keys_sequence :
	meta::enforce_enumerable<Enumerable>,
	meta::enforce_associative<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;

	/**
	 * \brief Type of values returned from dereferencing iterators.
	 *
	 * This type is as the same as Enumerable::key_type.
	 */
	using value_type = typename enumerable_traits::value_type;

	//! Type of const_iterator.
	using const_iterator = keys_iterator<typename enumerable_traits::const_iterator>;

	//! Type of iterator.
	using iterator = keys_iterator<typename enumerable_traits::iterator>;

	/**
	 * \brief Creates new keys_sequence from given sequence.
	 *
	 * **Complexity** 
	 * - O(1) for rvalue references of enumerable
	 * - O(N) for lvalue references of enumerable (where N is size of enumerable)
	 *
	 * \param enumerable Sequence from which keys are to be extracted.
	 */
	keys_sequence(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)) {}

	/**
	 * \brief Creates and returns iterator pointing at the begin.
	 */
	iterator
	begin() {
		return iterator(enumerable_traits::begin(m_enumerable));
	}

	/**
	 * \brief Creates and returns iterator pointing at the end.
	 */
	iterator
	end() {
		return iterator(enumerable_traits::end(m_enumerable));
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the begin.
	 */
	const_iterator
	begin() const {
		return const_iterator(enumerable_traits::begin(m_enumerable));
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the end.
	 */
	const_iterator
	end() const {
		return const_iterator(enumerable_traits::end(m_enumerable));
	}

private:
	Enumerable m_enumerable;
};

class keys_factory {
public:
   	template<class Enumerable>
	keys_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return keys_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
};

/**
 * \brief Piping operator extracting keys from input associative sequence.
 *
 * This operator can be safely used with infinite sequences.
 *
 * **Example**
 *
 *     std::map<int, int> input = { 
 *         { 1, 10 }, 
 *         { 2, 20 }, 
 *         { 3, 30 },
 *         { 4, 40 },
 *         { 5, 50 }
 *     };
 *     const auto out = input | tpl::keys;
 *     for (auto value : out) 
 *         std::cout << value << ", ";//output will be 1, 2, 3, 4, 5
 */
const keys_factory keys;

}
