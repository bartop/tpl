/**
 * \file
 * \brief File defining operator which drops given numer of elements.
 */
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

/**
 * \brief Sequence omitting given numer of elements from the begin. E.g.
 *      for sequence `{ 1, 2, 3, 4 }` and argument `toDrop` equal to 2 
 *      output sequence is `{ 3, 4 }`.
 *
 * This class can be safely used with infinite sequences.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable Type of sequence from which elements are to be dropped.
 *      Must satisfy is_enumerable trait.
 */
template<class Enumerable>
class dropping_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;

	//! Type of values returned from dereferencing iterators.
	using value_type = typename enumerable_traits::value_type;

	//! Type of const_iterator.
	using const_iterator = typename enumerable_traits::const_iterator;
	
	//! Type of iterator.
	using iterator = typename enumerable_traits::iterator;

	/**
	 * \brief Creates new dropping_sequence from given sequence and number of
	 *      elements to drop.
	 *
	 * **Complexity** 
	 * - O(1) for rvalue references of enumerable
	 * - O(N) for lvalue references of enumerable (where N is size of enumerable)
	 *
	 * \param enumerable Sequence from which values should be omitted.
	 * \param toDrop Number of elements that is to be omitted from the beginning
	 *      of the sequence. 
	 */
	dropping_sequence(
		Enumerable &&enumerable,
		unsigned toDrop
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_toDrop(toDrop) {}

	/**
	 * \brief Creates and returns iterator pointing at the begin.
	 *
	 * **Complexity**  
	 * - O(1) if wrapped sequence has uses random access iterator
	 * - O(k) otherwise (k is number of elements to drop) 
	 */
	iterator
	begin() {
		return std::distance(enumerable_traits::begin(m_enumerable),
			enumerable_traits::end(m_enumerable)) > m_toDrop ?
				std::next(enumerable_traits::begin(m_enumerable), m_toDrop) :
				enumerable_traits::end(m_enumerable
		);
	}

	/**
	 * \brief Creates and returns iterator pointing at the end.
	 *
	 * **Complexity**  
	 * O(1)
	 */
	iterator
	end() {
		return enumerable_traits::end(m_enumerable);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the begin.
	 *
	 * **Complexity**  
	 * - O(1) if wrapped sequence has uses random access iterator
	 * - O(k) otherwise (k is number of elements to drop) 
	 */
	const_iterator
	begin() const {
		return std::distance(enumerable_traits::begin(m_enumerable), 
			enumerable_traits::end(m_enumerable)) > m_toDrop ?
				std::next(enumerable_traits::begin(m_enumerable), m_toDrop) :
				enumerable_traits::end(m_enumerable
		);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the end.
	 *
	 * **Complexity**  
	 * O(1)
	 */
	const_iterator
	end() const {
		return enumerable_traits::end(m_enumerable);
	}

private:
	Enumerable m_enumerable;
	unsigned m_toDrop;
};

class drop_factory {
public:
	explicit drop_factory(unsigned toDrop) :
		m_toDrop(toDrop){}

	template<class Enumerable>
	dropping_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return dropping_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable),
			m_toDrop
		);
	}

private:
	unsigned m_toDrop;
};

/**
 * \brief Piping operator used to omit given numer of elements from the begin.
 *      E.g. for input sequence `{ 1, 2, 3, 4 }` and argument `toDrop` equal to 2 
 *      output sequence is `{ 3, 4 }`.
 *
 * This operator can be safely used with infinite sequences.
 *
 * \param toDrop Number of elements that is to be omitted from the beginning
 *      of the sequence. 
 *
 * **Example**
 *     
 *     std::vector<int> v = {1, 2, 3, 4, 5};
 *     const auto filtered = v | tpl::drop(3);
 *     for (auto val : filtered)
 *         std::cout << val << ", "; //displays 4, 5, 
 */
drop_factory
drop(unsigned toDrop){
	return drop_factory(toDrop);
}

}
