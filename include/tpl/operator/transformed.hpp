/**
 * \file
 * \brief File defining operator which allows transforming elements in given
 *     sequence.
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
#include <type_traits>
#include <functional>

namespace tpl{

template<class SubIterator, class Predicate>
class transforming_iterator :
	public detail::bidirectional_iterator_base<transforming_iterator<SubIterator, Predicate>> {
public:
	using sub_traits_t = std::iterator_traits<SubIterator>;
	using value_type = decltype(
		std::declval<Predicate>()(
			std::declval<typename sub_traits_t::value_type>()
		)
	);
	using difference_type = typename sub_traits_t::difference_type;
	using reference = value_type;
	using pointer = detail::pointer_proxy<value_type>;
	using iterator_category = typename meta::demote_to_bidirectional_tag<
		sub_traits_t
	>::type;

	transforming_iterator() = default;
	transforming_iterator(const transforming_iterator &) = default;
	transforming_iterator(transforming_iterator &&) = default;

	transforming_iterator &
	operator=(const transforming_iterator &) = default;
	transforming_iterator &
	operator=(transforming_iterator &&) = default;

	~transforming_iterator() noexcept = default;

	transforming_iterator(
		SubIterator subIterator,
		const Predicate &transformPredicate
	) :
		m_subIterator(std::move(subIterator)),
		m_transformPredicate(&transformPredicate) {}

	transforming_iterator &
	next() {
		++m_subIterator;
		return *this;
	}

	transforming_iterator &
	previous() {
		--m_subIterator;
		return *this;
	}

	reference
	operator*() const {
		return (*m_transformPredicate)(*m_subIterator);
	}

	pointer
	operator->() const {
		return detail::make_pointer_proxy(**this);
	}

	bool
	operator==(const transforming_iterator &transformingIterator) const {
		return this->m_subIterator == transformingIterator.m_subIterator;
	}

private:
	SubIterator m_subIterator;
	const Predicate *m_transformPredicate;
};

/**
 * \brief Sequence transforming values from input sequence using given
 *     predicate.
 *
 * This class can be safely used with infinite sequences.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam Enumerable Type of sequence from which elements are to be
 *     transformed. Must satisfy is_enumerable trait.
 * \tparam Predicate Type of function-like object used to transform elements
 *     in input sequence. It must take one argument constructible from
 *     Enumerable::value_type and return type which is at least move
 *     constructible.
 */
template<class Enumerable, class Predicate>
class transformed_sequence : meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;

	/**
	 * \brief Type of values returned from dereferencing iterators.
	 *
	 * This type is as the return type of Predicate function type.
	 */
	using value_type =
		decltype(
			std::declval<Predicate>()(
				std::declval<typename std::remove_reference<Enumerable>::type::value_type>()
			)
		);

	//! Type of const_iterator.
	using const_iterator = transforming_iterator<
		typename enumerable_traits::const_iterator,
		typename std::remove_reference<Predicate>::type
	>;

	//! Type of iterator.
	using iterator = transforming_iterator<
		typename enumerable_traits::iterator,
		typename std::remove_reference<Predicate>::type
		>;

	/**
	 * \brief Creates new transformed_sequence from given sequence and number of
	 *     elements to take.
	 *
	 * **Complexity** 
	 * - O(1) for rvalue references of enumerable
	 * - O(N) for lvalue references of enumerable (where N is size of enumerable)
	 *
	 * \tparam T Type of passed function-like object. Must be convertible to 
	 *     Predicate.
	 *
	 * \param enumerable Sequence from which elements are to be transformed.
	 * \param op Object of transforming function-like object.
	 */
	template<class T>
	transformed_sequence(
		Enumerable &&enumerable,
	   	T &&op
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_predicate(std::forward<T>(op)){}

	/**
	 * \brief Creates and returns iterator pointing at the begin.
	 */
	iterator
	begin() {
		return const_iterator(enumerable_traits::begin(m_enumerable), m_predicate);
	}

	/**
	 * \brief Creates and returns iterator pointing at the end.
	 */
	iterator
	end() {
		return const_iterator(enumerable_traits::end(m_enumerable), m_predicate);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the begin.
	 */
	const_iterator
	begin() const {
		return const_iterator(enumerable_traits::begin(m_enumerable), m_predicate);
	}

	/**
	 * \brief Creates and returns const_iterator pointing at the end.
	 */
	const_iterator
	end() const {
		return const_iterator(enumerable_traits::end(m_enumerable), m_predicate);
	}

private:
	Enumerable m_enumerable;
	Predicate m_predicate;
};

template<class Enumerable, class Predicate>
transformed_sequence<Enumerable, Predicate>
make_transformed(Enumerable &&enumerable, Predicate &&predicate){
	return transformed_sequence<Enumerable, Predicate>(
		std::forward<Enumerable>(enumerable),
		std::forward<Predicate>(predicate)
	);
}

template<class Predicate>
class transform_factory {
public:
	explicit transform_factory(Predicate &&transformPredicate) :
		m_transformPredicate(std::forward<Predicate>(transformPredicate)){}

	template<class Enumerable>
	transformed_sequence<Enumerable, const Predicate &>
	create(Enumerable &&enumerable) const & {
		return make_transformed(
			std::forward<Enumerable>(enumerable),
			m_transformPredicate
		);
	}

	template<class Enumerable>
	transformed_sequence<Enumerable, Predicate>
	create(Enumerable &&enumerable) && {
		return make_transformed(
			std::forward<Enumerable>(enumerable),
			std::forward<Predicate>(m_transformPredicate)
		);
	}
private:
	Predicate m_transformPredicate;
};

/**
 * \brief Piping operator transforming intput sequence.
 *
 * This operator can be safely used with infinite sequences.
 *
 * \tparam Predicate Type of function-like object used to transform elements
 *     in input sequence. It must take one argument constructible from
 *     value_type of input sequence and return type which is at least move
 *     constructible.
 *
 * \param transformPredicate Function-like object used for transforming elemnts
 *     in input sequence.
 *
 * **Example**
 *
 *     std::vector<int> input = { 1, 3, 5, 2, 4 };
 *     const auto out = input | tpl::transform([](auto i){ return std::to_string(2 * i); });
 *     for (auto value : out) 
 *         std::cout << value << ", ";//output will be 2, 6, 10, 4, 8
 */
template<class Predicate>
transform_factory<Predicate>
transform(Predicate &&transformPredicate){
	return transform_factory<Predicate>(std::forward<Predicate>(transformPredicate));
}

}
