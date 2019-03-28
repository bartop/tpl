/**
 * \file
 * \brief File defining operator which solely wraps the given sequence.
 */
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{


/**
 * \brief Sequence only wrapping given sequence.
 *
 * This class can be used as form of abstraction to reduce size of interface
 * of wrapped concrete sequence.
 *
 * This class can be safely used with infinite sequences.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable Type of sequence which is to be wrapped.
 */
template<class Enumerable>
class no_operation_sequence :
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
	using const_iterator = typename enumerable_traits::const_iterator;

	//! Type of iterator.
	using iterator = typename enumerable_traits::iterator;

	/**
	 * \brief Creates new no_operation_sequence from given sequence.
	 *
	 * **Complexity** 
	 * - O(1) for rvalue references of enumerable
	 * - O(N) for lvalue references of enumerable (where N is size of enumerable)
	 *
	 * \param enumerable Sequence which is to be wrapped.
	 */
	no_operation_sequence(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)) {}

	/**
	 * \brief Creates and returns iterator pointing at the begin.
	 */
	iterator
	begin() {
		return enumerable_traits::begin(m_enumerable);
	}

	/**
	 * \brief Creates and returns iterator pointing at the end.
	 */
	iterator
	end() {
		return enumerable_traits::end(m_enumerable);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the begin.
	 */
	const_iterator
	begin() const {
		return enumerable_traits::begin(m_enumerable);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the end.
	 */
	const_iterator
	end() const {
		return enumerable_traits::end(m_enumerable);
	}

private:
	Enumerable m_enumerable;
};

class no_operation_factory {
public:
	template<class Enumerable>
	no_operation_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return no_operation_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
};

/**
 * \brief Piping operator wrapping input sequence.
 *
 * This operator can be safely used with infinite sequences.
 *
 * **Example**
 *
 *     std::map<int, int> input = { 1, 2, 3, 4, 5 };
 *     const auto out = input | tpl::no_operation;
 *     for (auto value : out) 
 *         std::cout << value << ", ";//output will be 1, 2, 3, 4, 5
 */
const no_operation_factory no_operation;

}
