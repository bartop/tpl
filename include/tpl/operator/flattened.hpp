
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

template<class OuterIterator, class Enumerable, class InnerIterator>
class flattening_iterator :
	public detail::bidirectional_iterator_base<
		flattening_iterator<OuterIterator, Enumerable, InnerIterator>
	> {
public:
	using sub_traits_t = std::iterator_traits<InnerIterator>;
	using value_type = typename sub_traits_t::value_type;
	using external_enumerable_traits = meta::enumerable_traits<Enumerable>;
	using internal_enumerable_traits = meta::enumerable_traits<
		typename external_enumerable_traits::value_type
	>;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = typename meta::demote_to_bidirectional_tag<
		sub_traits_t
	>::type;

	flattening_iterator() = default;
	flattening_iterator(const flattening_iterator &) = default;
	flattening_iterator(flattening_iterator &&) = default;

	flattening_iterator &
	operator=(const flattening_iterator &) = default;
	flattening_iterator &
	operator=(flattening_iterator &&) = default;

	~flattening_iterator() noexcept = default;

	flattening_iterator(
		OuterIterator outerIterator,
		const Enumerable *enumerable
	) :
		m_outerIterator(std::move(outerIterator)),
		m_enumerable(enumerable),
   		m_innerIterator() {

		m_outerIterator = std::find_if(
			m_outerIterator,
			external_enumerable_traits::end(*m_enumerable),
			[](const auto &container) { 
				return internal_enumerable_traits::begin(container) !=
					internal_enumerable_traits::end(container); 
			}
		);

		if (m_outerIterator != external_enumerable_traits::end(*m_enumerable))
			m_innerIterator = internal_enumerable_traits::begin(*m_outerIterator);
	}

	flattening_iterator &
	next() {
		++m_innerIterator;

		if (m_innerIterator == internal_enumerable_traits::end(*m_outerIterator)) {
			++m_outerIterator;

			m_outerIterator = std::find_if(
				m_outerIterator,
				external_enumerable_traits::end(*m_enumerable),
				[](const auto &container) {
					return internal_enumerable_traits::begin(container) !=
						internal_enumerable_traits::end(container);
				}
			);

			if (m_outerIterator != external_enumerable_traits::end(*m_enumerable))
				m_innerIterator = internal_enumerable_traits::begin(*m_outerIterator);
		}
		return *this;
	}

	flattening_iterator &
	previous() {
		if (m_innerIterator != internal_enumerable_traits::begin(*m_outerIterator)) {
			--m_innerIterator;
			return *this;
		}

		auto reverseOuter = meta::make_reverse_iterator(m_outerIterator);
		const auto reverseEnd = meta::make_reverse_iterator(
				external_enumerable_traits::begin(*m_enumerable)
		);
		reverseOuter = std::find_if(
			reverseOuter,
			reverseEnd,
			[](const auto &container) {
				return internal_enumerable_traits::begin(container) !=
					internal_enumerable_traits::end(container);
			}
		);

		if (reverseOuter != reverseEnd)
			m_innerIterator = --internal_enumerable_traits::end(*reverseOuter);

		m_outerIterator = --(reverseOuter.base());

		return *this;
	}

	reference
	operator*() const {
		return *(this->m_innerIterator);
	}

	pointer
	operator->() const {
		return this->m_innerIterator.operator->();
	}

	bool
	operator==(const flattening_iterator &other) const {
		if (this->m_outerIterator == other.m_outerIterator) {
			if (external_enumerable_traits::end(*m_enumerable) == this->m_outerIterator)
				return true;
			else 
				return this->m_innerIterator == other.m_innerIterator;
		}
		return false;
	}

private:
	OuterIterator m_outerIterator;
	const Enumerable *m_enumerable = nullptr;
	InnerIterator m_innerIterator;
};

template<class Enumerable>
class flattened_sequence :
	meta::enforce_enumerable<Enumerable>,
	meta::enforce_enumerable<typename meta::enumerable_traits<Enumerable>::value_type>{
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using internal_enumerable_traits = meta::enumerable_traits<
		typename meta::enumerable_traits<Enumerable>::value_type
	>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = flattening_iterator<
		typename enumerable_traits::const_iterator,
		typename enumerable_traits::enumerable_type,
		typename internal_enumerable_traits::const_iterator
	>;
	using iterator = flattening_iterator<
		typename enumerable_traits::iterator,
		typename enumerable_traits::enumerable_type,
		typename internal_enumerable_traits::iterator
	>;

	flattened_sequence(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)) {} 

	iterator
	begin() {
		return iterator(enumerable_traits::begin(m_enumerable), &m_enumerable);
	}

	iterator
	end() {
		return iterator(enumerable_traits::end(m_enumerable), &m_enumerable);
	}

	const_iterator
	begin() const {
		return const_iterator(enumerable_traits::begin(m_enumerable), &m_enumerable);
	}

	const_iterator
	end() const {
		return const_iterator(enumerable_traits::end(m_enumerable), &m_enumerable);
	}
private:
	Enumerable m_enumerable;
};

class flatten_factory {
public:
   	template<class Enumerable>
	flattened_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return flattened_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
} flatten;

}
