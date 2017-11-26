
#pragma once

#include "meta/is_enumerable.hpp"
#include "meta/is_associative.hpp"
#include "meta/enumerable_traits.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class OuterIterator, class InnerIterator>
class flattening_iterator {
public:
	using sub_traits_t = std::iterator_traits<InnerIterator>;
	using value_type = typename sub_traits_t::value_type;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	~flattening_iterator() noexcept = default;

	flattening_iterator(
		OuterIterator outerIterator,
		OuterIterator outerEnd
	) :
		m_outerIterator(std::move(outerIterator)),
		m_outerEnd(std::move(outerEnd)) {
		if(m_outerIterator != m_outerEnd)
			m_innerIterator = std::begin(*m_outerIterator);

		while(m_outerIterator != m_outerEnd && m_innerIterator == std::end(*m_outerIterator)) {
			++m_outerIterator;
			m_innerIterator = std::begin(*m_outerIterator);
		}
	}

	flattening_iterator &
	operator++() {
		if (m_innerIterator != std::end(*m_outerIterator)) {
			++m_innerIterator;
		}
		while(m_outerIterator != m_outerEnd && m_innerIterator == std::end(*m_outerIterator)) {
			++m_outerIterator;
			m_innerIterator = std::begin(*m_outerIterator);
		}
		return *this;
	}

	flattening_iterator
	operator++(int) {
		const auto thisCopy = *this;
		++*this;
		return thisCopy;
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

	bool
	operator!=(const flattening_iterator &other) const {
		return !(*this == other);
	}

	void swap(flattening_iterator &filteringIterator) {
		std::swap(this->m_outerIterator, filteringIterator.m_outerIterator);
		std::swap(this->m_outerEnd, filteringIterator.m_outerEnd);
		std::swap(this->m_innerIterator, filteringIterator.m_innerIterator);
	}

private:
	OuterIterator m_outerIterator;
	OuterIterator m_outerEnd;
	InnerIterator m_innerIterator;
};

template<class Container>
class flattened_sequence :
	meta::enforce_enumerable<Container>,
	meta::enforce_enumerable<typename meta::enumerable_traits<Container>::value_type>{
public:
	using enumerable_traits = meta::enumerable_traits<Container>;
	using internal_enumerable_traits = meta::enumerable_traits<typename meta::enumerable_traits<Container>::value_type>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = flattening_iterator<typename enumerable_traits::const_iterator, typename internal_enumerable_traits::const_iterator>;
	using iterator = flattening_iterator<typename enumerable_traits::iterator, typename internal_enumerable_traits::iterator>;

	flattened_sequence(
		Container &&container
	) :
		m_container(std::forward<Container>(container)) { }

	void
	swap(flattened_sequence &other){
		std::swap(m_container, other.m_container);
	}

	iterator
	begin() {
		return iterator(std::begin(m_container), std::end(m_container));
	}

	iterator
	end() {
		return iterator(std::end(m_container), std::end(m_container));
	}

	const_iterator
	begin() const {
		return const_iterator(std::begin(m_container), std::end(m_container));
	}

	const_iterator
	end() const {
		return const_iterator(std::end(m_container), std::end(m_container));
	}
private:
	Container m_container;
};

struct flatten_tag {} flatten;

template<class Container>
flattened_sequence<Container>
operator|(Container &&container, const flatten_tag &){
	return flattened_sequence<Container>(
		std::forward<Container>(container)
	);
}

}
