
#pragma once

#include <iterator>
#include <type_traits>

namespace tpl{

namespace detail{

template<class TransformPredicate>
struct transform_holder {
	transform_holder(TransformPredicate transformPredicate) :
		m_transformPredicate(std::move(transformPredicate)){}

	TransformPredicate m_transformPredicate;
};

template<class T>
class pointer_proxy {
public:
	pointer_proxy(T &&value) : m_value(value) {}
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

template<class Container, class TransformPredicate>
class transformed_sequence {
public:
	using container_t = typename std::remove_reference<Container>::type;
	using value_type = typename container_t::value_type;
	using const_iterator = transforming_iterator<typename container_t::const_iterator, TransformPredicate>;
	using iterator = transforming_iterator<typename container_t::iterator, TransformPredicate>;

	transformed_sequence(
		Container &&container,
	   	TransformPredicate transformPredicate
	) :
		m_container(std::forward<Container>(container)),
		m_transformPredicate(std::move(transformPredicate)){}

	void
	swap(transformed_sequence &other){
		std::swap(m_container, other.m_container);
		std::swap(m_transformPredicate, other.m_transformPredicate);
	}

	iterator
	begin() {
		return const_iterator(std::begin(m_container), m_transformPredicate);
	}

	iterator
	end() {
		return const_iterator(std::end(m_container), m_transformPredicate);
	}

	const_iterator
	begin() const {
		return const_iterator(std::begin(m_container), m_transformPredicate);
	}

	const_iterator
	end() const {
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
operator|(
	Container &&container,
   	detail::transform_holder<TransformPredicate> holder
){
	return transformed_sequence<Container, TransformPredicate>(std::forward<Container>(container), std::move(holder.m_transformPredicate));
}

}
