
#pragma once

#include <algorithm>

namespace tpl{
namespace generator{

template<class ValueType>
class infinite_iterator {
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

	infinite_iterator
	operator++(int) {
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
	operator==(const infinite_iterator &other) const {
		return false;
	}

	bool
	operator!=(const infinite_iterator &other) const {
		return true;
	}

	void swap(infinite_iterator &other) {
		std::swap(this->m_value, other.m_value);
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

	void
	swap(infinite_sequence &other){
		std::swap(m_value, other.m_value);
	}

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
}
