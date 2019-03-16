
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

template<class OuterIterator, class InnerIterator>
class flattening_iterator :
	public detail::input_iterator_base<flattening_iterator<OuterIterator, InnerIterator>> {
public:
	using sub_traits_t = std::iterator_traits<InnerIterator>;
	using value_type = typename sub_traits_t::value_type;
	using enumerable_traits = meta::enumerable_traits<decltype(*std::declval<OuterIterator>())>;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	flattening_iterator() = default;

	~flattening_iterator() noexcept = default;

	flattening_iterator(
		OuterIterator outerIterator,
		OuterIterator outerEnd
	) :
		m_outerIterator(std::move(outerIterator)),
		m_outerEnd(std::move(outerEnd)),
   		m_innerIterator() {
		if(m_outerIterator != m_outerEnd && enumerable_traits::begin(*m_outerIterator) != enumerable_traits::end(*m_outerIterator))
			m_innerIterator = enumerable_traits::begin(*m_outerIterator);
		else{
			m_outerIterator = std::find_if(
				m_outerIterator,
				m_outerEnd,
				[](const auto &container) { return enumerable_traits::begin(container) != enumerable_traits::end(container); }
			);

			if (m_outerIterator != m_outerEnd)
				m_innerIterator = enumerable_traits::begin(*m_outerIterator);
		}
	}

	flattening_iterator &
	operator++() {
		if (m_innerIterator != enumerable_traits::end(*m_outerIterator))
			++m_innerIterator;

		if (m_outerIterator != m_outerEnd && m_innerIterator == enumerable_traits::end(*m_outerIterator))	{
			++m_outerIterator;

			m_outerIterator = std::find_if(
				m_outerIterator,
				m_outerEnd,
				[](const auto &container) { return enumerable_traits::begin(container) != enumerable_traits::end(container); }
			);
			if (m_outerIterator != m_outerEnd)
				m_innerIterator = enumerable_traits::begin(*m_outerIterator);
		}
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
		return this->m_outerIterator == other.m_outerIterator;
	}

private:
	OuterIterator m_outerIterator;
	OuterIterator m_outerEnd;
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
		typename internal_enumerable_traits::const_iterator
	>;
	using iterator = flattening_iterator<
		typename enumerable_traits::iterator,
		typename internal_enumerable_traits::iterator
	>;

	flattened_sequence(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)) {} 

	iterator
	begin() {
		return iterator(enumerable_traits::begin(m_enumerable), enumerable_traits::end(m_enumerable));
	}

	iterator
	end() {
		return iterator(enumerable_traits::end(m_enumerable), enumerable_traits::end(m_enumerable));
	}

	const_iterator
	begin() const {
		return const_iterator(enumerable_traits::begin(m_enumerable), enumerable_traits::end(m_enumerable));
	}

	const_iterator
	end() const {
		return const_iterator(enumerable_traits::end(m_enumerable), enumerable_traits::end(m_enumerable));
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
