
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

template<class SubIterator, class Predicate>
class transforming_iterator :
	public detail::iterator_base<transforming_iterator<SubIterator, Predicate>> {
public:
	using sub_traits_t = std::iterator_traits<SubIterator>;
	using value_type = decltype(
		std::declval<Predicate>()(
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
		const Predicate &transformPredicate
	) :
		m_subIterator(std::move(subIterator)),
		m_transformPredicate(transformPredicate) {}

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
	const Predicate &m_transformPredicate;
};


template<class Enumerable, class Predicate>
class transforming_operator {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type =
		decltype(
			std::declval<Predicate>()(
				std::declval<typename std::remove_reference<Enumerable>::type::value_type>()
			)
		);
	using const_iterator = transforming_iterator<typename enumerable_traits::const_iterator, Predicate>;
	using iterator = transforming_iterator<typename enumerable_traits::iterator, Predicate>;

	transforming_operator(Predicate &&predicate) :
		m_predicate(std::forward<Predicate>(predicate)){}

	iterator
	first(Enumerable &enumerable) {
		return const_iterator(std::begin(enumerable), m_predicate);
	}

	iterator
	last(Enumerable &enumerable) {
		return const_iterator(std::end(enumerable), m_predicate);
	}

	const_iterator
	first(const Enumerable &enumerable) const {
		return const_iterator(std::begin(enumerable), m_predicate);
	}

	const_iterator
	last(const Enumerable &enumerable) const {
		return const_iterator(std::end(enumerable), m_predicate);
	}

private:
	Predicate m_predicate;
};

template<class Enumerable, class Predicate>
class transformed_sequence : meta::enforce_enumerable<Enumerable> {
public:
	using operator_t = transforming_operator<Enumerable, Predicate>;
	using value_type = typename operator_t::value_type;
	using const_iterator = typename operator_t::const_iterator;
	using iterator = typename operator_t::iterator;

	template<class T>
	transformed_sequence(
		Enumerable &&enumerable,
	   	T &&op
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_operator(std::forward<T>(op)){}

	iterator
	begin() {
		return m_operator.first(m_enumerable);
	}

	iterator
	end() {
		return m_operator.last(m_enumerable);
	}

	const_iterator
	begin() const {
		return m_operator.first(m_enumerable);
	}

	const_iterator
	end() const {
		return m_operator.last(m_enumerable);
	}

private:
	Enumerable m_enumerable;
	transforming_operator<Enumerable, Predicate> m_operator;
};

template<class Enumerable, class Predicate>
transformed_sequence<Enumerable, Predicate>
make_transformed(Enumerable &&enumerable, Predicate &&predicate){
	return transformed_sequence<Enumerable, Predicate>(
		std::forward<Enumerable>(enumerable),
		std::forward<Predicate>(predicate)
	);
}

template<class Predicate>
class transform_factory {
public:
	explicit transform_factory(Predicate &&transformPredicate) :
		m_transformPredicate(std::forward<Predicate>(transformPredicate)){}

	template<class Enumerable>
	transformed_sequence<Enumerable, const Predicate &>
	create(Enumerable &&enumerable) const & {
		return make_transformed(
			std::forward<Enumerable>(enumerable),
			m_transformPredicate
		);
	}

	template<class Enumerable>
	transformed_sequence<Enumerable, Predicate>
	create(Enumerable &&enumerable) && {
		return make_transformed(
			std::forward<Enumerable>(enumerable),
			std::forward<Predicate>(m_transformPredicate)
		);
	}
private:
	Predicate m_transformPredicate;
};

template<class Predicate>
transform_factory<Predicate>
transform(Predicate &&transformPredicate){
	return transform_factory<Predicate>(std::forward<Predicate>(transformPredicate));
}

template<
	class Enumerable,
   	class Predicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
transformed_sequence<Enumerable, Predicate>
operator|(Enumerable &&enumerable, const transform_factory<Predicate> &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
   	class Predicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
transformed_sequence<Enumerable, Predicate>
operator|(Enumerable &&enumerable, transform_factory<Predicate> &&factory){
	return std::move(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Factory, class Predicate>
composite_factory<const transform_factory<Predicate> &, Factory>
operator|(const transform_factory<Predicate> &factory, Factory &&other){
	return make_composite(
		factory,
	   	std::forward<Factory>(other)
	);
}

template<class Factory, class Predicate>
composite_factory<transform_factory<Predicate>, Factory>
operator|(transform_factory<Predicate> &&factory, Factory &&other){
	return make_composite(
		std::move(factory),
	   	std::forward<Factory>(other)
	);
}

}
