/**
 * \file
 * \brief File defining caching operator.
 */
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <vector>

namespace tpl{

/**
 * \brief Sequence caching any other sequence.
 *
 * This sequence is useful when using other operator which does any kind of
 * heavy or costly processing. It eagerly processes the iterators of given 
 * sequence and stores the results so that they are available immidiately.
 * This class due to its nature CANNOT be used with infinite sequences.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable Type of sequence to be cached. Must satisfy is_enumerable trait.
 */
template<class Enumerable>
class cached_sequence : meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;

	//! Type of values returned from dereferencing iterators.
	using value_type = typename enumerable_traits::value_type;
	using cached_t = std::vector<value_type>;

	//! Type of const_iterator.
	using const_iterator = typename cached_t::const_iterator;

	//! Type of iterator.
	using iterator = typename cached_t::iterator;

	/**
	 * \brief Creates new cached_sequence from given sequence.
	 *
	 * **Complexity**  
	 * - O(1) for rvalue references
	 * - O(N) for lvalue references (where N is size of enumerable)
	 *
	 * \param enumerable Sequence which is to be cached.
	 */
	cached_sequence(Enumerable &&enumerable) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_wasFilled(false),
		m_cached() {}

	/**
	 * \brief Creates and returns iterator pointing at the begin.
	 *
	 * **Complexity**  
	 * O(N) complexity when is called before any begin() or end(),
	 * O(1) on any subsequent call
	 */
	iterator
	begin() {
		fillCache(m_enumerable);
		return std::begin(m_cached);
	} 

	/**
	 * \brief Creates and returns iterator pointing at the end.
	 *
	 * **Complexity**  
	 * O(N) complexity when is called before any begin() or end(),
	 * O(1) on any subsequent call
	 */
	iterator
	end() {
		fillCache(m_enumerable);
		return std::end(m_cached);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the begin.
	 *
	 * **Complexity**  
	 * O(N) complexity when is called before any begin() or end(),
	 * O(1) on any subsequent call
	 */
	const_iterator
	begin() const {
		fillCache(m_enumerable);
		return std::begin(m_cached);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the end.
	 *
	 * **Complexity**  
	 * O(N) complexity when is called before any begin() or end(),
	 * O(1) on any subsequent call
	 */
	const_iterator
	end() const {
		fillCache(m_enumerable);
		return std::end(m_cached);
	}

private:
	void
	fillCache(const Enumerable &enumerable) const {
		if(!m_wasFilled) {
			std::copy(
				enumerable_traits::begin(enumerable),
				enumerable_traits::end(enumerable),
				std::back_inserter(m_cached)
			);
			m_wasFilled = true;
		}
	}

	Enumerable m_enumerable;
	mutable bool m_wasFilled = false;
	mutable cached_t m_cached;
};

class cache_factory {
public:
	template<class Enumerable>
	cached_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return cached_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
};

/**
 * \brief Piping operator used to cache given sequence.
 *
 * This operator is useful when using other operator which does any kind of
 * heavy or costly processing. It eagerly processes the iterators of given 
 * sequence and stores the results so that they are available immidiately.
 * This operator CANNOT be used with infinite sequences.
 *
 * **Complexity**  
 * - O(1) for rvalue references
 * - O(N) for lvalue references (N is size of enumerable)
 */
const cache_factory cache;

}
