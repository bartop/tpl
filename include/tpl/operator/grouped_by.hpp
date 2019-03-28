/**
 * \file
 * \brief File defining operator which groups given sequence.
 */
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <vector>
#include <unordered_map>

namespace tpl{

/**
 * \brief Sequence grouping input sequence into associative container of
 *     sequences using given predicate.
 *
 * This class cannnot be safely used with infinite sequences - it eagerly 
 * processes the input sequence.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable Type of sequence which is to be grouped. Must satisfy
 *      is_enumerable trait.
 * \tparam Grouping Type of function used to group elements from enumerable. 
 *      It must take one argument constructible from Enumerable::value_type
 *      and return any copy-constructible type.
 */
template<class Enumerable, class Grouping>
class grouped_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;

	/**
	 * \brief Type of .first value in pair returned from iterators. 
	 * 
	 * This type is the same as return type of Grouping type.
	 */
	using key_type = decltype(std::declval<Grouping>()(
		std::declval<typename enumerable_traits::value_type>())
	);

	/**
	 * \brief Type of .first second in pair returned from iterators. 
	 * 
	 * This type is a sequence of values of type Enumerable::value_type.
	 */
	using mapped_type = std::vector<typename enumerable_traits::value_type>;
	using grouped_t = std::unordered_map<key_type, mapped_type>;

	//! Type of values returned from dereferencing iterators.
	using value_type = typename grouped_t::value_type;

	//! Type of const_iterator.
	using const_iterator = typename grouped_t::const_iterator;

	//! Type of iterator.
	using iterator = typename grouped_t::iterator;

	/**
	 * \brief Creates new grouping_sequence from given sequence.
	 *
	 * **Complexity** 
	 * - O(1) for rvalue references of enumerable
	 * - O(N) for lvalue references of enumerable (where N is size of enumerable)
	 *
	 * \param enumerable Sequence which is to be grouped.
	 * \param op Function used to group elements in given input sequence.
	 */
	template<class T>
	grouped_sequence(
		Enumerable &&enumerable,
	   	T &&op
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_grouped(),
		m_groupingFunction(op){}

	/**
	 * \brief Creates and returns iterator pointing at the begin.
	 *
	 * This function causes the sequence to be eagerly grouped and returns the
	 * iterator.
	 *
	 * **Complexity** 
	 * O(N) (where N is size of internal sequence)
	 */
	iterator
	begin() {
		group(m_enumerable);
		return std::begin(m_grouped);
	}

	/**
	 * \brief Creates and returns iterator pointing at the end.
	 */
	iterator
	end() {
		return std::end(m_grouped);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the begin.
	 *
	 * This function causes the sequence to be eagerly grouped and returns the
	 * iterator.
	 *
	 * **Complexity** 
	 * O(N) (where N is size of internal sequence)
	 */
	const_iterator
	begin() const {
		group(m_enumerable);
		return std::begin(m_grouped);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the end.
	 */
	const_iterator
	end() const {
		return std::end(m_grouped);
	}

private:
	void
	group(const Enumerable &enumerable) const {
		m_grouped.clear();
		for (const auto &value : enumerable)
			m_grouped[m_groupingFunction(value)].push_back(value);
	}

	Enumerable m_enumerable;
	mutable grouped_t m_grouped;
	Grouping m_groupingFunction;
};

template<class Enumerable, class Grouping>
grouped_sequence<Enumerable, Grouping>
make_grouped(Enumerable &&enumerable, Grouping &&predicate){
	return grouped_sequence<Enumerable, Grouping>(
		std::forward<Enumerable>(enumerable),
		std::forward<Grouping>(predicate)
	);
}

template<class Grouping>
class grouping_factory {
public:
	explicit grouping_factory(Grouping &&grouping) :
		m_grouping(std::forward<Grouping>(grouping)){}

	template<class Enumerable>
	grouped_sequence<Enumerable, const Grouping &>
	create(Enumerable &&enumerable) const & {
		return make_grouped(
			std::forward<Enumerable>(enumerable),
			m_grouping
		);
	}

	template<class Enumerable>
	grouped_sequence<Enumerable, Grouping>
	create(Enumerable &&enumerable) && {
		return make_grouped(
			std::forward<Enumerable>(enumerable),
			std::forward<Grouping>(m_grouping)
		);
	}
private:
	Grouping m_grouping;
};

/**
 * \brief Piping operator grouping input sequence into map of sequences using
 *     given predicate.
 *
 * This operator cannnot be safely used with infinite sequences - it eagerly 
 * processes the input sequence.
 *
 * \tparam Grouping Type of function used to group elements from enumerable. 
 *     It must take one argument constructible from Enumerable::value_type
 *     and return any copy-constructible type.
 *
 * \param grouping Function used to group elements in given input sequence.
 *
 * **Example**
 *
 *     std::vector<int> input = { 1, 2, 3, 4, 5 };
 *     const auto grouped = input | group_by([](auto i){ i < 3; });
 *     for (const auto &pair : grouped) {
 *         std::cout << pair.first << ": {";
 *	       for (auto value : pair.second) 
 *	           std::cout << value << ",";
 *	       std::cout << "}" << std::endl;
 *	       // output will be 
 *         // true : { 1, 2, },
 *         // false : { 3, 4, 5, },
 *         // or the lines will be in reverse order
 *	   }
 */
template<class Grouping>
grouping_factory<Grouping>
group_by(Grouping &&grouping){
	return grouping_factory<Grouping>(std::forward<Grouping>(grouping));
}

}
