/**
 * \file
 * \brief File defining operator which reverses order of elements in input 
 *     sequence.
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

/**
 * \brief Sequence reversing order of elements in input sequence.
 *
 * This class **cannot** be safely used with infinite sequences.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable Type of sequence which is to be reversed.
 */
template<class Enumerable>
class reversing_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;

	/**
	 * \brief Type of values returned from dereferencing iterators.
	 *
	 * This type is as the same as Enumerable::value_type.
	 */
	using value_type = typename enumerable_traits::value_type;

	//! Type of const_iterator.
	using const_iterator = std::reverse_iterator<
		typename enumerable_traits::const_iterator
	>;

	//! Type of iterator.
	using iterator = std::reverse_iterator<
		typename enumerable_traits::iterator
	>;

	/**
	 * \brief Creates new reversed_sequence from given sequence.
	 *
	 * **Complexity** 
	 * - O(1) for rvalue references of enumerable
	 * - O(N) for lvalue references of enumerable (where N is size of enumerable)
	 *
	 * \param enumerable Sequence which is to be reversed.
	 */
	reversing_sequence(Enumerable &&enumerable) :
		m_enumerable(std::forward<Enumerable>(enumerable)) {}

	/**
	 * \brief Creates and returns iterator pointing at the begin.
	 */
	iterator
	begin() {
		return meta::make_reverse_iterator(enumerable_traits::end(m_enumerable));
	}

	/**
	 * \brief Creates and returns iterator pointing at the end.
	 */
	iterator
	end() {
		return meta::make_reverse_iterator(enumerable_traits::begin(m_enumerable));
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the begin.
	 */
	const_iterator
	begin() const {
		return meta::make_reverse_iterator(enumerable_traits::end(m_enumerable));
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the end.
	 */
	const_iterator
	end() const {
		return meta::make_reverse_iterator(enumerable_traits::begin(m_enumerable));
	}

private:
	Enumerable m_enumerable;
};

class reverse_factory {
public:
	template<class Enumerable>
	reversing_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return reversing_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
};

/**
 * \brief Piping operator for reversing order of elements in input sequence.
 *
 * This operator **cannot** be used with infinite sequences.
 *
 * **Example**
 *
 *     std::vector<int> input = { 1, 2, 3, 4, 5 };
 *     const auto out = input | tpl::reverse;
 *     for (auto value : out) 
 *         std::cout << value << ", ";//output will be 5, 4, 3, 2, 1,
 */
const reverse_factory reverse;

}
