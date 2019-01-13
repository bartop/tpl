
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../detail/pointer_proxy.hpp"
#include "../detail/iterator_base.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <type_traits>
#include <functional>

namespace tpl{

template<class SubIterator, class Predicate>
class transforming_iterator :
	public detail::input_iterator_base<transforming_iterator<SubIterator, Predicate>> {
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
		m_transformPredicate(&transformPredicate) {}

	transforming_iterator &
	operator++() {
		++m_subIterator;
		return *this;
	}

	reference
	operator*() const {
		return (*m_transformPredicate)(*m_subIterator);
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
	const Predicate *m_transformPredicate;
};

template<class Enumerable, class Predicate>
class transformed_sequence : meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type =
		decltype(
			std::declval<Predicate>()(
				std::declval<typename std::remove_reference<Enumerable>::type::value_type>()
			)
		);
	using const_iterator = transforming_iterator<
		typename enumerable_traits::const_iterator,
		typename std::remove_reference<Predicate>::type
	>;
	using iterator = transforming_iterator<
		typename enumerable_traits::iterator,
		typename std::remove_reference<Predicate>::type
		>;

	template<class T>
	transformed_sequence(
		Enumerable &&enumerable,
	   	T &&op
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_predicate(std::forward<T>(op)){}

	iterator
	begin() {
		return const_iterator(std::begin(m_enumerable), m_predicate);
	}

	iterator
	end() {
		return const_iterator(std::end(m_enumerable), m_predicate);
	}

	const_iterator
	begin() const {
		return const_iterator(std::begin(m_enumerable), m_predicate);
	}

	const_iterator
	end() const {
		return const_iterator(std::end(m_enumerable), m_predicate);
	}

private:
	Enumerable m_enumerable;
	Predicate m_predicate;
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

}
