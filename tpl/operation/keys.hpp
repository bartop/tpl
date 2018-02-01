
#pragma once

#include "meta/is_enumerable.hpp"
#include "meta/is_associative.hpp"
#include "meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include "../common/composition_operator.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class SubIterator>
class keys_iterator : public detail::iterator_base<keys_iterator<SubIterator>> {
public:
	using sub_traits_t = std::iterator_traits<SubIterator>;
	using associative_traits_t = meta::associative_element_traits<typename sub_traits_t::value_type>;
	using value_type = typename associative_traits_t::key_type;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = value_type;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	~keys_iterator() noexcept = default;

	keys_iterator(
		SubIterator subIterator
	) :
		m_subIterator(std::move(subIterator)) { }

	keys_iterator &
	operator++() {
		++m_subIterator;
		return *this;
	}

	reference
	operator*() const {
		return associative_traits_t::key_value(*(this->m_subIterator));
	}

	pointer
	operator->() const {
		return &associative_traits_t::key_value(*(this->m_subIterator));
	}

	bool
	operator==(const keys_iterator &other) const {
		return this->m_subIterator == other.m_subIterator;
	}

	void
	swap(keys_iterator &filteringIterator) {
		std::swap(this->m_subIterator, filteringIterator.m_subIterator);
	}

private:
	SubIterator m_subIterator;
};

template<class Enumerable>
class keys_sequence :
	meta::enforce_enumerable<Enumerable>,
	meta::enforce_associative<Enumerable>{
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = keys_iterator<typename enumerable_traits::const_iterator>;
	using iterator = keys_iterator<typename enumerable_traits::iterator>;

	keys_sequence(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)) { }

	void
	swap(keys_sequence &other){
		std::swap(m_enumerable, other.m_enumerable);
	}

	iterator
	begin() {
		return iterator(std::begin(m_enumerable));
	}

	iterator
	end() {
		return iterator(std::end(m_enumerable));
	}

	const_iterator
	begin() const {
		return const_iterator(std::begin(m_enumerable));
	}

	const_iterator
	end() const {
		return const_iterator(std::end(m_enumerable));
	}
private:
	Enumerable m_enumerable;
};

class keys_factory {
public:
   	template<class Enumerable>
	keys_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return keys_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
} keys;

}
