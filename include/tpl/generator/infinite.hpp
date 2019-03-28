/**
 * \file
 * \brief File defining generator infinitely repeating same value.
 */
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

	infinite_iterator() = default;

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

/**
 * \brief Infinite sequence repeating indefinitely one value. E.g. for given 
 *		value 5 the output sequence is `{ 5, 5, 5,... }`.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam ValueType Objects of this type will be returned from iterator. This type
 *      must be copy-constructible.
 */
template<class ValueType>
class infinite_sequence {
public:
	//! Type of values returned from dereferencing iterators.
	using value_type = ValueType;

	//! Type of const_iterator.
	using const_iterator = infinite_iterator<ValueType>;
	//
	//! Type of iterator.
	using iterator = const_iterator;

	/**
	 * \brief Creates new generated_sequence from given value.
	 *
	 * \param value Value that is returned by the iterators of this sequence.
	 */
	infinite_sequence(
		ValueType &&value
	) :
		m_value(std::forward<ValueType>(value)){}

	//! Creates and returns iterator pointing at the begin.
	iterator
	begin() {
		return iterator(m_value);
	}

	//! Creates and returns iterator pointing at the end.
	iterator
	end() {
		return iterator(m_value);
	}

	//! Creates and returns const_iterator pointing at the begin.
	const_iterator
	begin() const {
		return iterator(m_value);
	}

	//! Creates and returns const_iterator pointing at the end.
	const_iterator
	end() const {
		return iterator(m_value);
	}
private:
	value_type m_value;
};

/**
 * \brief Piping generator which repeats indefinitely one value. E.g. for given
 *		value 5 the output sequence is `{ 5, 5, 5,... }`.
 *
 * \tparam ValueType Objects of this type will be returned from iterator. This type
 *      must be copy-constructible.
 *
 * \param value Value that is returned by the iterators of this sequence.
 *
 * \return infinite_sequence repeating all the time given value.
 */
template<class ValueType>
infinite_sequence<ValueType>
infinite(ValueType &&value){
	return infinite_sequence<ValueType>(std::forward<ValueType>(value));
}

}
