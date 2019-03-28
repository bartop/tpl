/**
 * \file
 * \brief File defining operator which allows zipping together two sequences.
 */
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"
#include "../meta/iterators.hpp"

#include "../detail/pointer_proxy.hpp"
#include "../detail/iterator_base.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class SubIterator1, class SubIterator2>
class zipped_iterator :
	public detail::bidirectional_iterator_base<zipped_iterator<SubIterator1, SubIterator2>> {
public:
	using sub_traits_t1 = std::iterator_traits<SubIterator1>;
	using sub_traits_t2 = std::iterator_traits<SubIterator2>;
	using value_type = std::pair<typename sub_traits_t1::value_type, typename sub_traits_t2::value_type>;
	using difference_type = typename sub_traits_t1::difference_type;
	using reference = value_type;
	using pointer = detail::pointer_proxy<value_type>;
	using iterator_category = typename meta::demote_to_bidirectional_tag<
		sub_traits_t1
	>::type;

	zipped_iterator() = default;
	zipped_iterator(const zipped_iterator &) = default;
	zipped_iterator(zipped_iterator &&) = default;

	zipped_iterator &
	operator=(const zipped_iterator &) = default;
	zipped_iterator &
	operator=(zipped_iterator &&) = default;

	~zipped_iterator() noexcept = default;

	zipped_iterator(
		SubIterator1 subIterator1,
		SubIterator2 subIterator2
	) :
		m_subIterator1(std::move(subIterator1)),
		m_subIterator2(std::move(subIterator2)) {}

	zipped_iterator &
	next() {
		++m_subIterator1;
		++m_subIterator2;
		return *this;
	}

	zipped_iterator &
	previous() {
		--m_subIterator1;
		--m_subIterator2;
		return *this;
	}

	reference
	operator*() const {
		return std::make_pair(*m_subIterator1, *m_subIterator2);
	}

	pointer
	operator->() const {
		return detail::make_pointer_proxy(
			std::make_pair(*m_subIterator1, *m_subIterator2)
		);
	}

	bool
	operator==(const zipped_iterator &other) const {
		return this->m_subIterator1 == other.m_subIterator1 ||
			this->m_subIterator2 == other.m_subIterator2;
	}

private:
	SubIterator1 m_subIterator1;
	SubIterator2 m_subIterator2;
};

/**
 * \brief Sequence zipping together two sequences.
 *
 * This class can be safely used with infinite sequences.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable1 Type of first sequence to be zipped.
 * \tparam Enumerable2 Type of second sequence to be zipped.
 */
template<class Enumerable1, class Enumerable2>
class ziped_sequence : meta::enforce_enumerable<Enumerable1> {
public:
	using enumerable_traits1 = meta::enumerable_traits<Enumerable1>;
	using enumerable_traits2 = meta::enumerable_traits<Enumerable2>;

	
	//! Type of values returned from dereferencing iterators.
	using value_type = std::pair<
		typename enumerable_traits1::value_type,
		typename enumerable_traits2::value_type
	>;

	//! Type of const_iterator.
	using const_iterator = zipped_iterator<
		typename enumerable_traits1::const_iterator,
		typename enumerable_traits2::const_iterator
	>;

	//! Type of iterator.
	using iterator = zipped_iterator<
		typename enumerable_traits1::iterator,
		typename enumerable_traits2::iterator
	>;

	/**
	 * \brief Creates new ziped_sequence from given sequences.
	 *
	 * **Complexity** 
	 * - O(1) for rvalue references of enumerable
	 * - O(N) for lvalue references of enumerable (where N is size of enumerable)
	 *
	 * \param enumerable1 First sequence to be zipped.
	 * \param enumerable2 Second sequence to be zipped.
	 */
	ziped_sequence(
		Enumerable1 &&enumerable1,
		Enumerable2 &&enumerable2
	) :
		m_enumerable1(std::forward<Enumerable1>(enumerable1)),
		m_enumerable2(std::forward<Enumerable2>(enumerable2)){}

	/**
	 * \brief Creates and returns iterator pointing at the begin.
	 */
	iterator
	begin() {
		return iterator(
			enumerable_traits1::begin(m_enumerable1),
		   	enumerable_traits2::begin(m_enumerable2)
		);
	}

	/**
	 * \brief Creates and returns iterator pointing at the end.
	 */
	iterator
	end() {
		return iterator(
			enumerable_traits1::end(m_enumerable1),
		   	enumerable_traits2::end(m_enumerable2)
		);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the begin.
	 */
	const_iterator
	begin() const {
		return const_iterator(
			enumerable_traits1::begin(m_enumerable1),
		   	enumerable_traits2::begin(m_enumerable2)
		);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the end.
	 */
	const_iterator
	end() const {
		return const_iterator(
			enumerable_traits1::end(m_enumerable1),
		   	enumerable_traits2::end(m_enumerable2)
		);
	}

private:
	Enumerable1 m_enumerable1;
	Enumerable2 m_enumerable2;
};

template<class Enumerable1, class Enumerable2>
ziped_sequence<Enumerable1, Enumerable2>
make_zipped(Enumerable1 &&enumerable1, Enumerable2 &&enumerable2){
	return ziped_sequence<Enumerable1, Enumerable2>(
		std::forward<Enumerable1>(enumerable1),
	   	std::forward<Enumerable2>(enumerable2)
	);
}

template<class Enumerable>
class zipped_enumerable_factory {
public:
	zipped_enumerable_factory(Enumerable &&enumerable) :
		m_enumerable(std::forward<Enumerable>(enumerable)){}

	template<class Enumerable2>
	ziped_sequence<Enumerable2, const Enumerable &>
	create(Enumerable2 &&enumerable) const & {
		return make_zipped(
			std::forward<Enumerable2>(enumerable),
			m_enumerable
		);
	}

	template<class Enumerable2>
	ziped_sequence<Enumerable2, Enumerable>
	create(Enumerable2 &&enumerable) && {
		return make_zipped(
			std::forward<Enumerable2>(enumerable),
			std::forward<Enumerable>(m_enumerable)
		);
	}
private:
	Enumerable m_enumerable;
};

/**
 * \brief Piping operator zipping together two sequences the input one and the
 *     one passed as a parameter.
 *
 * This operator can be safely used with infinite sequences.
 *
 * \tparam Enumerable Type of sequence with which input sequence is to be
 *     zipped with.
 *
 * \param enumerable Sequence with which input enumerable is to be zipped with.
 *
 * **Example**
 *
 *     std::vector<int> input1 = { 1, 3, 5, 2, 4 };
 *     std::vector<int> input2 = { 4, 8, 0, 1, 5 };
 *     const auto out = input1 | tpl::zip(input2);
 *     for (auto value : out) 
 *         std::cout << value.first << ", " << value.second << ", ";
 *         //output will be 1, 4, 3, 8, 5, 0, 2, 1, 4, 5, 
 */
template<class Enumerable>
zipped_enumerable_factory<Enumerable>
zip(Enumerable &&enumerable){
	return zipped_enumerable_factory<Enumerable>(std::forward<Enumerable>(enumerable));
}

}
