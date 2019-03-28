/**
 * \file
 * \brief File defining operator which takes given number of elements from
 *     input sequence.
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
 * \brief Sequence taking given number of elements from input sequence.
 *
 * This class can be safely used with infinite sequences. Actually, it is
 * designed to make finite sequences from infinite ones.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable Type of sequence from which elements are to be taken. Must
 *     satisfy is_enumerable trait.
 */
template<class Enumerable>
class taken_sequence :
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
	 * \brief Creates new taken_sequence from given sequence and number of
	 *     elements to take.
	 *
	 * **Complexity** 
	 * - O(1) for rvalue references of enumerable
	 * - O(N) for lvalue references of enumerable (where N is size of enumerable)
	 *
	 * \param enumerable Sequence from which elements are to be taken.
	 * \param toTake Numer of elements to take from enumerable.
	 */
	taken_sequence(
		Enumerable &&enumerable,
		unsigned toTake
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_toTake(toTake) {}

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
		return std::distance(enumerable_traits::begin(m_enumerable), enumerable_traits::end(m_enumerable)) > m_toTake ?
			std::next(enumerable_traits::begin(m_enumerable), m_toTake) :
			enumerable_traits::end(m_enumerable);
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
		return std::distance(enumerable_traits::begin(m_enumerable), enumerable_traits::end(m_enumerable)) > m_toTake ?
			std::next(enumerable_traits::begin(m_enumerable), m_toTake) :
			enumerable_traits::end(m_enumerable);
	}

private:
	Enumerable m_enumerable;
	unsigned m_toTake;
};

class take_factory {
public:
	explicit take_factory(unsigned toTake) :
		m_toTake(toTake){}

	template<class Enumerable>
	taken_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return taken_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable),
			m_toTake
		);
	}

private:
	unsigned m_toTake;
};

/**
 * \brief Piping operator taking given number of elements from intput sequence
 *     and creating a new sequence from them.
 *
 * This operator can be safely used with infinite sequences. Actually, it is
 * designed to make finite sequences from infinite ones.
 *
 * \param toTake Numer of elements that are to be taken from the beginning of
 *     the input sequence.
 *
 * **Example**
 *
 *     std::vector<int> input = { 1, 3, 5, 2, 4 };
 *     const auto out = input | tpl::take(3);
 *     for (auto value : out) 
 *         std::cout << value << ", ";//output will be 1, 3, 5,
 */
take_factory
take(unsigned toTake){
	return take_factory(toTake);
}

}
