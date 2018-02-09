
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../detail/pointer_proxy.hpp"
#include "../detail/iterator_base.hpp"

#include "../common/composite_factory.hpp"

#include <iterator>
#include <type_traits>

namespace tpl{

template<class SubIterator, class TransformPredicate>
class transforming_iterator :
	public detail::iterator_base<transforming_iterator<SubIterator, TransformPredicate>> {
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

template<class Enumerable, class Predicate>
auto
make_transformed(Enumerable &&enumerable, Predicate &&predicate){
	return transformed_sequence<Enumerable, Predicate>(
		std::forward<Enumerable>(enumerable),
		std::forward<Predicate>(predicate)
	);
}

template<class TransformPredicate>
class transform_factory {
public:
	explicit transform_factory(TransformPredicate &&transformPredicate) :
		m_transformPredicate(std::forward<TransformPredicate>(transformPredicate)){}

	template<class Enumerable>
	auto
	create(Enumerable &&enumerable) const & {
		return make_transformed(
			std::forward<Enumerable>(enumerable),
			m_transformPredicate
		);
	}

	template<class Enumerable>
	auto
	create(Enumerable &&enumerable) && {
		return make_transformed(
			std::forward<Enumerable>(enumerable),
			std::forward<TransformPredicate>(m_transformPredicate)
		);
	}
private:
	TransformPredicate m_transformPredicate;
};

template<class TransformPredicate>
transform_factory<TransformPredicate>
transform(TransformPredicate &&transformPredicate){
	return transform_factory<TransformPredicate>(std::forward<TransformPredicate>(transformPredicate));
}

template<
	class Enumerable,
   	class TransformPredicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
auto
operator|(Enumerable &&enumerable, const transform_factory<TransformPredicate> &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
   	class TransformPredicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
auto
operator|(Enumerable &&enumerable, transform_factory<TransformPredicate> &&factory){
	return std::move(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Factory, class TransformPredicate>
auto
operator|(const transform_factory<TransformPredicate> &factory, Factory &&other){
	return make_composite(
		factory,
	   	std::forward<Factory>(other)
	);
}

template<class Factory, class TransformPredicate>
auto
operator|(transform_factory<TransformPredicate> &&factory, Factory &&other){
	return make_composite(
		std::move(factory),
	   	std::forward<Factory>(other)
	);
}

}
