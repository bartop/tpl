
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include <algorithm>

namespace tpl{

template<class Enumerable>
class cycle_iterator : public detail::input_iterator_base<cycle_iterator<Enumerable>> {
public:
	using enumerable_traits = meta::enumerable_traits<typename std::decay<Enumerable>::type>;
	using value_type = typename enumerable_traits::value_type;
	using difference_type = ptrdiff_t;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	~cycle_iterator() noexcept = default;

	cycle_iterator(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
   		m_currentIterator(enumerable.begin()) {}

	cycle_iterator &
	operator++() {
		++m_currentIterator;
		if (m_currentIterator == m_enumerable.end())
			m_currentIterator = m_enumerable.begin();

		return *this;
	}

	reference
	operator*() const {
		return *m_currentIterator;
	}

	pointer
	operator->() const {
		return m_currentIterator.operator->();
	}

	bool
	operator==(const cycle_iterator &other) const {
		return false;
	}

private:
	Enumerable m_enumerable;
	typename enumerable_traits::const_iterator m_currentIterator;
};

template<class Enumerable>
class cycle_sequence
	: meta::enforce_enumerable<Enumerable> {
public:
	using value_type = typename meta::enumerable_traits<typename std::decay<Enumerable>::type>::value_type;
	using const_iterator = cycle_iterator<Enumerable>;
	using iterator = const_iterator;

	cycle_sequence(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)){}

	iterator
	begin() {
		return iterator(m_enumerable);
	}

	iterator
	end() {
		return iterator(m_enumerable);
	}

	const_iterator
	begin() const {
		return iterator(m_enumerable);
	}

	const_iterator
	end() const {
		return iterator(m_enumerable);
	}

private:
	Enumerable m_enumerable;
};

template<class Enumerable>
cycle_sequence<Enumerable>
cycle(Enumerable &&enumerable){
	return cycle_sequence<Enumerable>(std::forward<Enumerable>(enumerable));
}

}
