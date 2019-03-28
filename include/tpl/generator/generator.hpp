/**
 * \file
 * \brief File defining function-based sequence generator.
 */
#pragma once

#include "../detail/iterator_base.hpp"

#include <algorithm>

namespace tpl{

template<class GeneratingFunction, class ValueType>
class generating_iterator :
	public detail::input_iterator_base<generating_iterator<GeneratingFunction, ValueType>> {
public:
	using value_type = ValueType;
	using difference_type = ptrdiff_t;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	generating_iterator() = default;

	~generating_iterator() noexcept = default;

	generating_iterator(
		const GeneratingFunction &generatingFunction,
		ValueType initialValue
	) :
		m_generatingFunction(&generatingFunction),
   		m_currentValue(std::move(initialValue)){}

	generating_iterator &
	operator++() {
		m_currentValue = (*m_generatingFunction)(m_currentValue);
		return *this;
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
	operator==(const generating_iterator &) const {
		return false;
	}

private:
	const GeneratingFunction *m_generatingFunction;
	ValueType m_currentValue;
};

/**
 * \brief Infinite sequence generating elements utilizing function. E.g. for
 *      given function `[](int i){ return i + 2; }` and initial value 3 the
 *      generated sequence is `{ 3, 5, 7, ... }`.
 *
 * This class complies with is_enumerable trait, which allows it to be used in
 * a pipeline.
 *
 * \tparam GeneratingFunction Type of function used to generate subsequent elements.
 *      It should take one argument of type convertible to ValueType and return type 
 *      convertible to ValueType.
 *      If you are trying to use multiple iterators from this sequence the function
 *      cannot have side-effects or the behaviour is unspecified.
 * \tparam ValueType Objects of this type will be returned from iterator. This type
 *      must be copy-constructible.
 */
template<class GeneratingFunction, class ValueType>
class generated_sequence {
public:
	//! Type of values returned from dereferencing iterators.
	using value_type = ValueType;

	//! Type of const_iterator.
	using const_iterator = generating_iterator<GeneratingFunction, ValueType>;

	//! Type of iterator.
	using iterator = const_iterator;

	/**
	 * \brief Creates new generated_sequence from given enumerable.
	 *
	 * \param generatingFunction Function to be used to generate subsequent values.
	 * \param initialValue Value used to initialize the generator.
	 */
	generated_sequence(
		GeneratingFunction &&generatingFunction,
		ValueType &&initialValue
	) :
		m_generatingFunction(std::forward<GeneratingFunction>(generatingFunction)),
		m_initialValue(std::forward<ValueType>(initialValue)){}

	//! Creates and returns iterator pointing at the begin.
	iterator
	begin() {
		return iterator(m_generatingFunction, m_initialValue);
	}

	//! Creates and returns iterator pointing at the end.
	iterator
	end() {
		return iterator(m_generatingFunction, m_initialValue);
	}

	//! Creates and returns const_iterator pointing at the begin.
	const_iterator
	begin() const {
		return iterator(m_generatingFunction, m_initialValue);
	}

	//! Creates and returns const_iterator pointing at the end.
	const_iterator
	end() const {
		return iterator(m_generatingFunction, m_initialValue);
	}
private:
	GeneratingFunction m_generatingFunction;
	ValueType m_initialValue;
};

/**
 * \brief Piping generator based on a function. E.g. for given function
 *      `[](int i){ return i + 2; }` and initial value 3 the generated sequence
 *      is `{ 3, 5, 7, ... }`.
 *
 * \tparam GeneratingFunction Type of function used to generate subsequent elements.
 *      It should take one argument of type convertible to ValueType and return type 
 *      convertible to ValueType.
 *      If you are trying to use multiple iterators from this sequence the function
 *      cannot have side-effects or the behaviour is unspecified.
 * \tparam ValueType Objects of this type will be returned from iterator. This type
 *      must be copy-constructible.
 *
 * \param generatingFunction Function to be used to generate subsequent values.
 * \param initialValue Value used to initialize the generator.
 *
 * \return generator_sequence using given function and initialValue to generate 
 *      subsequent values.
 */
template<class GeneratingFunction, class ValueType>
generated_sequence<GeneratingFunction, ValueType>
generator(GeneratingFunction &&generatingFunction, ValueType &&initialValue){
	return generated_sequence<GeneratingFunction, ValueType>(
		std::forward<GeneratingFunction>(generatingFunction),
		std::forward<ValueType>(initialValue)
	);
}

}
