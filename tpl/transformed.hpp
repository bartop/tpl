
#pragma once

#include "meta/is_enumerable.hpp"
#include "meta/is_associative.hpp"
#include "meta/enumerable_traits.hpp"

#include <iterator>
#include <type_traits>

namespace tpl{

namespace detail{

template<class T>
class pointer_proxy {
public:
	pointer_proxy(T &&value) : m_value(std::forward<T>(value)) {}
	T *operator ->() const { return &m_value; }

private:
	T m_value;
};

template<class T>
pointer_proxy<T>
make_pointer_proxy(T &&value){
	return pointer_proxy<T>(std::forward<T>(value));
}

}

template<class TransformPredicate>
struct transform_holder {
	transform_holder(TransformPredicate &&transformPredicate) :
		m_transformPredicate(std::forward<TransformPredicate>(transformPredicate)){}

	TransformPredicate m_transformPredicate;
};

template<class SubIterator, class TransformPredicate>
class transforming_iterator {
public:
	using sub_traits_t = std::iterator_traits<SubIterator>;
	using value_type = decltype(
		std::declval<TransformPredicate>()(
			std::declval<typename sub_traits_t::value_type>()
		)
	);
	using difference_type = typename sub_traits_t::difference_type;
	using reference = value_type;
	using pointer = detail::pointer_proxy<value_type>;
	using iterator_category = std::input_iterator_tag;

	~transforming_iterator() noexcept = default;

	transforming_iterator(
		SubIterator subIterator,
		TransformPredicate transformPredicate
	) :
		m_subIterator(std::move(subIterator)),
		m_transformPredicate(std::move(transformPredicate)) {}

	transforming_iterator &
	operator++() {
		++m_subIterator;
		return *this;
	}

	transforming_iterator
	operator++(int) {
		auto thisCopy = *this;
		++(*this);
		return thisCopy;
	}

	reference
	operator*() const {
		return m_transformPredicate(*m_subIterator);
	}

	pointer
	operator->() const {
		return detail::make_pointer_proxy(**this);
	}

	bool
	operator==(const transforming_iterator &transformingIterator) const {
		return this->m_subIterator == transformingIterator.m_subIterator;
	}

	bool
	operator!=(const transforming_iterator &transformingIterator) const {
		return this->m_subIterator != transformingIterator.m_subIterator;
	}

	void swap(transforming_iterator &transformingIterator) {
		std::swap(this->m_subIterator, transformingIterator.m_subIterator);
		std::swap(this->m_transformPredicate, transformingIterator.m_transformPredicate);
	}
private:
	SubIterator m_subIterator;
	TransformPredicate m_transformPredicate;
};


template<class Enumerable, class TransformPredicate>
class transformed_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type =
		decltype(
			std::declval<TransformPredicate>()(
				std::declval<typename std::remove_reference<Enumerable>::type::value_type>()
			)
		);
	using const_iterator = transforming_iterator<typename enumerable_traits::const_iterator, TransformPredicate>;
	using iterator = transforming_iterator<typename enumerable_traits::iterator, TransformPredicate>;

	transformed_sequence(
		Enumerable &&enumerable,
	   	TransformPredicate &&transformPredicate
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_transformPredicate(std::forward<TransformPredicate>(transformPredicate)){}

	void
	swap(transformed_sequence &other){
		std::swap(m_enumerable, other.m_enumerable);
		std::swap(m_transformPredicate, other.m_transformPredicate);
	}

	iterator
	begin() {
		return const_iterator(std::begin(m_enumerable), m_transformPredicate);
	}

	iterator
	end() {
		return const_iterator(std::end(m_enumerable), m_transformPredicate);
	}

	const_iterator
	begin() const {
		return const_iterator(std::begin(m_enumerable), m_transformPredicate);
	}

	const_iterator
	end() const {
		return const_iterator(std::end(m_enumerable), m_transformPredicate);
	}

private:
	Enumerable m_enumerable;
	TransformPredicate m_transformPredicate;
};

template<class TransformPredicate>
transform_holder<TransformPredicate>
transform(TransformPredicate &&transformPredicate){
	return transform_holder<TransformPredicate>(std::forward<TransformPredicate>(transformPredicate));
}

template<class Enumerable, class TransformPredicate>
transformed_sequence<Enumerable, TransformPredicate>
operator|(
	Enumerable &&enumerable,
   	transform_holder<TransformPredicate> &&holder
){
	return transformed_sequence<Enumerable, TransformPredicate>(
		std::forward<Enumerable>(enumerable),
	   	std::forward<transform_holder<TransformPredicate>>(holder).m_transformPredicate
	);
}

}
