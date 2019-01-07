
#pragma once

#include "../detail/iterator_base.hpp"

#include <algorithm>

namespace tpl{

template<class ValueType>
class infinite_iterator : public detail::input_iterator_base<infinite_iterator<ValueType>> {
public:
	using value_type = ValueType;
	using difference_type = ptrdiff_t;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	~infinite_iterator() noexcept = default;

	infinite_iterator(
		ValueType value
	) :
		m_value(std::move(value)) {}

	infinite_iterator &
	operator++() {
		return *this;
	}

	reference
	operator*() const {
		return m_value;
	}

	pointer
	operator->() const {
		return &m_value;
	}

	bool
	operator==(const infinite_iterator &) const {
		return false;
	}

private:
	ValueType m_value;
};

template<class ValueType>
class infinite_sequence {
public:
	using value_type = ValueType;
	using const_iterator = infinite_iterator<ValueType>;
	using iterator = const_iterator;

	infinite_sequence(
		ValueType &&value
	) :
		m_value(std::forward<ValueType>(value)){}

	iterator
	begin() {
		return iterator(m_value);
	}

	iterator
	end() {
		return iterator(m_value);
	}

	const_iterator
	begin() const {
		return iterator(m_value);
	}

	const_iterator
	end() const {
		return iterator(m_value);
	}
private:
	value_type m_value;
};

template<class ValueType>
infinite_sequence<ValueType>
infinite(ValueType &&value){
	return infinite_sequence<ValueType>(std::forward<ValueType>(value));
}

}
