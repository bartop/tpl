/**
 * \file
 * \brief File defining operator which sorts elements in input sequence.
 */
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <set>

namespace tpl{

/**
 * \brief Sequence sorting elements in input sequence.
 *
 * This class **cannot** be safely used with infinite sequences.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable Type of sequence which is to be sorted. Must satisfy
 *      is_enumerable trait.
 * \tparam Comparison Type of function supplied to compare elements in input
 *     sequence. It must accept two arguments, each constructible from
 *     Enumerable::value_type and return bool. It must follow strict weak
 *     ordering.
 */
template<class Enumerable, class Comparison>
class sorted_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;

	/**
	 * \brief Type of values returned from dereferencing iterators.
	 *
	 * This type is as the same as Enumerable::value_type.
	 */
	using value_type = typename enumerable_traits::value_type;
	using sorted_t = std::multiset<value_type, Comparison>;

	//! Type of const_iterator.
	using const_iterator = typename sorted_t::const_iterator;

	//! Type of iterator.
	using iterator = typename sorted_t::iterator;

	/**
	 * \brief Creates new sorted_sequence from given sequence and comparison
	 *     function.
	 *
	 * **Complexity** 
	 * - O(1) for rvalue references of enumerable
	 * - O(N) for lvalue references of enumerable (where N is size of enumerable)
	 *
	 * \tparam T Type of function-like object passed to constructor. Must be
	 *     convertible to Comparison type.
	 *
	 * \param enumerable Sequence which is to be reversed.
	 * \param op Function-like object used to compare elements of enumerable
	 *     during sorting.
	 */
	template<class T>
	sorted_sequence(
		Enumerable &&enumerable,
	   	T &&op
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_sorted(std::forward<T>(op)) {}

	sorted_sequence &operator=(sorted_sequence &) = delete;

	/**
	 * \brief Sorts the sequence. Then creates and returns iterator pointing at
	 *     the begin.
	 *
	 * **Complexity**
	 * O(n log(n))
	 */
	iterator
	begin() {
		sort(m_enumerable);
		return std::begin(m_sorted);
	}

	/**
	 * \brief Creates and returns iterator pointing at the end.
	 */
	iterator
	end() {
		return std::end(m_sorted);
	}

	/**
	 * \brief Sorts the sequence. Then creates and returns const_iterator
	 *     pointing at the begin.
	 *
	 * **Complexity**
	 * O(n log(n))
	 */
	const_iterator
	begin() const {
		sort(m_enumerable);
		return std::begin(m_sorted);
	}

	/**
	 * \brief Creates and returns iterator pointing at the end.
	 */
	const_iterator
	end() const {
		return std::end(m_sorted);
	}

private:
	void
	sort(const Enumerable &enumerable) const {
		m_sorted.clear();
		std::copy(
			enumerable_traits::begin(enumerable),
			enumerable_traits::end(enumerable),
			std::inserter(m_sorted, m_sorted.begin())
		);
	}

	Enumerable m_enumerable;
	mutable sorted_t m_sorted;
};

template<class Enumerable, class Comparison>
sorted_sequence<Enumerable, Comparison>
make_sorted(Enumerable &&enumerable, Comparison &&predicate){
	return sorted_sequence<Enumerable, Comparison>(
		std::forward<Enumerable>(enumerable),
		std::forward<Comparison>(predicate)
	);
}

template<class Comparison>
class compare_factory {
public:
	explicit compare_factory(Comparison &&compareComparison) :
		m_comparison(std::forward<Comparison>(compareComparison)){}

	template<class Enumerable>
	sorted_sequence<Enumerable, const Comparison &>
	create(Enumerable &&enumerable) const & {
		return make_sorted(
			std::forward<Enumerable>(enumerable),
			m_comparison
		);
	}

	template<class Enumerable>
	sorted_sequence<Enumerable, Comparison>
	create(Enumerable &&enumerable) && {
		return make_sorted(
			std::forward<Enumerable>(enumerable),
			std::forward<Comparison>(m_comparison)
		);
	}
private:
	Comparison m_comparison;
};

/**
 * \brief Piping operator sorting elements in input sequence.
 *
 * This class **cannot** be used with infinite sequences.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Comparison Type of function supplied to compare elements in input
 *     sequence. It must accept two arguments, each constructible from
 *     value_type of input sequence and return bool. It must follow strict weak
 *     ordering.
 *
 * \param comparison Function-like object used to compare elements of enumerable
 *     during sorting.
 *
 * **Example**
 *
 *     std::vector<int> input = { 1, 3, 5, 2, 4 };
 *     const auto out = input | tpl::sort([](auto i, auto j){ return i < j; });
 *     for (auto value : out) 
 *         std::cout << value << ", ";//output will be 1, 2, 3, 4, 5,
 */
template<class Comparison>
compare_factory<Comparison>
sort(Comparison &&comparison){
	return compare_factory<Comparison>(std::forward<Comparison>(comparison));
}

}
