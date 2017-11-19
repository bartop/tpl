
#pragma once

#include "cache.hpp"

#include <iterator>

namespace tpl{

namespace detail{

template<class TransformPredicate>
struct transform_holder {
	transform_holder(TransformPredicate transformPredicate) :
		m_transformPredicate(std::move(transformPredicate)){}

	TransformPredicate m_transformPredicate;
};

}

template<class SubIterator, class TransformPredicate>
class transforming_iterator :
	public std::iterator<std::input_iterator_tag, typename SubIterator::value_type> {
public:
	using self_t = transforming_iterator;
	using traits_t = std::iterator_traits<self_t>;

	~transforming_iterator() noexcept = default;

	transforming_iterator(
		SubIterator subIterator,
		TransformPredicate transformPredicate
	) :
		m_subIterator(std::move(subIterator)),
		m_transformPredicate(std::move(transformPredicate)) {}

	transforming_iterator(const transforming_iterator &transformingIterator) :
		m_cache(transformingIterator.m_cache),
		m_currentElementCached(transformingIterator.m_currentElementCached),
	   	m_subIterator(transformingIterator.m_subIterator),
		m_transformPredicate(transformingIterator.m_transformPredicate){}

	transforming_iterator(transforming_iterator &&other) :
		m_subIterator(std::move(other.m_subIterator)),
		m_transformPredicate(std::move(other.m_transformPredicate)),
		m_cache(std::move(other.m_cache)),
		m_currentElementCached(other.m_currentElementCached) { }

	transforming_iterator &
	operator=(transforming_iterator other){
		this->swap(other);
		return *this;
	}

	transforming_iterator &
	operator++() {
		m_currentElementCached = false;
		++m_subIterator;
		return *this;
	}

	transforming_iterator
	operator++(int) {
		m_currentElementCached = false;
		auto thisCopy = *this;
		++(*this);
		return thisCopy;
	}

	const typename traits_t::value_type &
	operator*() {
		if(!m_currentElementCached)
			m_cache.write(m_transformPredicate(*(this->m_subIterator)));

		return m_cache.cachedValue();
	}

	const typename traits_t::value_type *
	operator->() {
		if(!m_currentElementCached)
			m_cache.write(m_transformPredicate(*(this->m_subIterator)));

		return m_cache.cachedPointer();
	}

	const typename traits_t::value_type &
	operator*() const {
		if(!m_currentElementCached)
			m_cache.write(m_transformPredicate(*(this->m_subIterator)));

		return m_cache.cachedValue();
	}

	const typename traits_t::value_type *
	operator->() const {
		if(!m_currentElementCached)
			m_cache.write(m_transformPredicate(*(this->m_subIterator)));

		return m_cache.cachePointer();
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
		std::swap(this->m_cache, transformingIterator.m_cache);
		std::swap(this->m_currentElementCached, transformingIterator.m_currentElementCached);
	}
private:
	mutable bool m_currentElementCached = false;
	mutable detail::cache<typename traits_t::value_type> m_cache;
	SubIterator m_subIterator;
	TransformPredicate m_transformPredicate;
};

template<class Container, class TransformPredicate>
class transformed_sequence {
public:
	using const_iterator = transforming_iterator<typename Container::const_iterator, TransformPredicate>;
	transformed_sequence(const transformed_sequence &filteredSequence) :
	   	m_container(filteredSequence.m_container),
		m_transformPredicate(filteredSequence.m_transformPredicate){}

	transformed_sequence(transformed_sequence &&other) :
		m_container(std::move(other.m_container)),
		m_transformPredicate(std::move(other.m_transformPredicate)){}

	transformed_sequence(
		Container container,
	   	TransformPredicate transformPredicate
	) :
		m_container(std::move(container)),
		m_transformPredicate(std::move(transformPredicate)){}

	transformed_sequence &operator=(transformed_sequence other) {
		this->swap(other);
		return *this;
	}

	void
	swap(transformed_sequence &other){
		std::swap(m_container, other.m_container);
		std::swap(m_transformPredicate, other.m_transformPredicate);
	}

	const_iterator
	begin() const{
		return const_iterator(std::begin(m_container), m_transformPredicate);
	}

	const_iterator
	end() const{
		return const_iterator(std::end(m_container), m_transformPredicate);
	}
private:
	Container m_container;
	TransformPredicate m_transformPredicate;
};

template<class TransformPredicate>
detail::transform_holder<TransformPredicate>
transform(TransformPredicate transformPredicate){
	return detail::transform_holder<TransformPredicate>(std::move(transformPredicate));
}

template<class Container, class TransformPredicate>
transformed_sequence<Container, TransformPredicate>
operator>>(
	Container container,
   	detail::transform_holder<TransformPredicate> holder
){
	return transformed_sequence<Container, TransformPredicate>(std::move(container), std::move(holder.m_transformPredicate));
}

}
