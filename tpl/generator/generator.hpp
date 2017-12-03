
#pragma once

#include <algorithm>

namespace tpl{

template<class GeneratingFunction, class ValueType>
class generating_iterator {
public:
	using value_type = ValueType;
	using difference_type = ptrdiff_t;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	~generating_iterator() noexcept = default;

	generating_iterator(
		GeneratingFunction generatingFunction,
		ValueType initialValue
	) :
		m_generatingFunction(std::move(generatingFunction)),
   		m_currentValue(std::move(initialValue)){}

	generating_iterator &
	operator++() {
		m_currentValue = m_generatingFunction(m_currentValue);
		return *this;
	}

	generating_iterator
	operator++(int) {
		const auto tmp = *this;
		++*this;
		return tmp;
	}

	reference
	operator*() const {
		return m_currentValue;
	}

	pointer
	operator->() const {
		return &m_currentValue;
	}

	bool
	operator==(const generating_iterator &other) const {
		return false;
	}

	bool
	operator!=(const generating_iterator &other) const {
		return true;
	}

	void swap(generating_iterator &other) {
		std::swap(this->m_generatingFunction, other.m_generatingFunction);
	}

private:
	GeneratingFunction m_generatingFunction;
	ValueType m_currentValue;
};

template<class GeneratingFunction, class ValueType>
class generated_sequence {
public:
	using value_type = ValueType;
	using const_iterator = generating_iterator<GeneratingFunction, ValueType>;
	using iterator = const_iterator;

	generated_sequence(
		GeneratingFunction &&generatingFunction,
		ValueType &&initialValue
	) :
		m_generatingFunction(std::forward<GeneratingFunction>(generatingFunction)),
		m_initialValue(std::forward<ValueType>(initialValue)){}

	void
	swap(generated_sequence &other){
		std::swap(m_generatingFunction, other.m_generatingFunction);
		std::swap(m_initialValue, other.m_initialValue);
	}

	iterator
	begin() {
		return iterator(m_generatingFunction, m_initialValue);
	}

	iterator
	end() {
		return iterator(m_generatingFunction, m_initialValue);
	}

	const_iterator
	begin() const {
		return iterator(m_generatingFunction, m_initialValue);
	}

	const_iterator
	end() const {
		return iterator(m_generatingFunction, m_initialValue);
	}
private:
	GeneratingFunction m_generatingFunction;
	ValueType m_initialValue;
};

template<class GeneratingFunction, class ValueType>
generated_sequence<GeneratingFunction, ValueType>
generator(GeneratingFunction &&generatingFunction, ValueType &&initialValue){
	return generated_sequence<GeneratingFunction, ValueType>(
		std::forward<GeneratingFunction>(generatingFunction),
		std::forward<ValueType>(initialValue)
	);
}

}
