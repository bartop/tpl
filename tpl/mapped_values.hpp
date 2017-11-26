
#pragma once

#include "meta/is_enumerable.hpp"
#include "meta/is_associative.hpp"
#include "meta/enumerable_traits.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class SubIterator>
class mapped_values_iterator {
public:
	using sub_traits_t = std::iterator_traits<SubIterator>;
	using associative_traits_t = meta::associative_element_traits<typename sub_traits_t::value_type>;
	using value_type = typename associative_traits_t::key_type;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = value_type;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	~mapped_values_iterator() noexcept = default;

	mapped_values_iterator(
		SubIterator subIterator
	) :
		m_subIterator(std::move(subIterator)) { }

	mapped_values_iterator &
	operator++() {
		++m_subIterator;
		return *this;
	}

	mapped_values_iterator
	operator++(int) {
		const auto thisCopy = *this;
		++*this;
		return thisCopy;
	}

	reference
	operator*() const {
		return associative_traits_t::mapped_value(*(this->m_subIterator));
	}

	pointer
	operator->() const {
		return &associative_traits_t::mapped_value(*(this->m_subIterator));
	}

	bool
	operator==(const mapped_values_iterator &other) const {
		return this->m_subIterator == other.m_subIterator;
	}

	bool
	operator!=(const mapped_values_iterator &other) const {
		return !(*this == other);
	}

	void swap(mapped_values_iterator &filteringIterator) {
		std::swap(this->m_subIterator, filteringIterator.m_subIterator);
	}

private:
	SubIterator m_subIterator;
};

template<class Container>
class mapped_values_sequence :
	meta::enforce_enumerable<Container>,
	meta::enforce_associative<Container>{
public:
	using enumerable_traits = meta::enumerable_traits<Container>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = mapped_values_iterator<typename enumerable_traits::const_iterator>;
	using iterator = mapped_values_iterator<typename enumerable_traits::iterator>;

	mapped_values_sequence(
		Container &&container
	) :
		m_container(std::forward<Container>(container)) { }

	void
	swap(mapped_values_sequence &other){
		std::swap(m_container, other.m_container);
	}

	iterator
	begin() {
		return iterator(std::begin(m_container));
	}

	iterator
	end() {
		return iterator(std::end(m_container));
	}

	const_iterator
	begin() const {
		return const_iterator(std::begin(m_container));
	}

	const_iterator
	end() const {
		return const_iterator(std::end(m_container));
	}
private:
	Container m_container;
};

struct mapped_values_tag {} mapped_values;

template<class Container>
mapped_values_sequence<Container>
operator|(Container &&container, const mapped_values_tag &) {
	return mapped_values_sequence<Container>(
		std::forward<Container>(container)
	);
}

}
