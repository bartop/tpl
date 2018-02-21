
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include "../common/composite_factory.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class OuterIterator, class InnerIterator>
class flattening_iterator :
	public detail::iterator_base<flattening_iterator<OuterIterator, InnerIterator>> {
public:
	using sub_traits_t = std::iterator_traits<InnerIterator>;
	using value_type = typename sub_traits_t::value_type;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	~flattening_iterator() noexcept = default;

	flattening_iterator(
		OuterIterator outerIterator,
		OuterIterator outerEnd
	) :
		m_outerIterator(std::move(outerIterator)),
		m_outerEnd(std::move(outerEnd)),
   		m_innerIterator() {
		if(m_outerIterator != m_outerEnd && std::begin(*m_outerIterator) != std::end(*m_outerIterator))
			m_innerIterator = std::begin(*m_outerIterator);
		else{
			m_outerIterator = std::find_if(
				m_outerIterator,
				m_outerEnd,
				[](const auto &container) { return std::begin(container) != std::end(container); }
			);

			if (m_outerIterator != m_outerEnd)
				m_innerIterator = std::begin(*m_outerIterator);
		}
	}

	flattening_iterator &
	operator++() {
		if (m_innerIterator != std::end(*m_outerIterator))
			++m_innerIterator;

		if (m_outerIterator != m_outerEnd && m_innerIterator == std::end(*m_outerIterator))	{
			++m_outerIterator;

			m_outerIterator = std::find_if(
				m_outerIterator,
				m_outerEnd,
				[](const auto &container) { return std::begin(container) != std::end(container); }
			);
			if (m_outerIterator != m_outerEnd)
				m_innerIterator = std::begin(*m_outerIterator);
		}
		return *this;
	}

	reference
	operator*() const {
		return *(this->m_innerIterator);
	}

	pointer
	operator->() const {
		return this->m_innerIterator.operator->();
	}

	bool
	operator==(const flattening_iterator &other) const {
		return this->m_outerIterator == other.m_outerIterator;
	}

private:
	OuterIterator m_outerIterator;
	OuterIterator m_outerEnd;
	InnerIterator m_innerIterator;
};

template<class Enumerable>
class flattening_operator {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using internal_enumerable_traits = meta::enumerable_traits<
		typename meta::enumerable_traits<Enumerable>::value_type
	>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = flattening_iterator<
		typename enumerable_traits::const_iterator,
		typename internal_enumerable_traits::const_iterator
	>;
	using iterator = flattening_iterator<
		typename enumerable_traits::iterator,
		typename internal_enumerable_traits::iterator
	>;

	iterator
	first(Enumerable &enumerable) {
		return iterator(std::begin(enumerable), std::end(enumerable));
	}

	iterator
	last(Enumerable &enumerable) {
		return iterator(std::end(enumerable), std::end(enumerable));
	}

	const_iterator
	first(const Enumerable &enumerable) const {
		return const_iterator(std::begin(enumerable), std::end(enumerable));
	}

	const_iterator
	last(const Enumerable &enumerable) const {
		return const_iterator(std::end(enumerable), std::end(enumerable));
	}

};

template<class Enumerable>
class flattened_sequence :
	meta::enforce_enumerable<Enumerable>,
	meta::enforce_enumerable<typename meta::enumerable_traits<Enumerable>::value_type>{
public:
	using operator_t = flattening_operator<Enumerable>;
	using value_type = typename operator_t::value_type;
	using const_iterator = typename operator_t::const_iterator;
	using iterator = typename operator_t::iterator;

	flattened_sequence(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_operator() {}

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
	flattening_operator<Enumerable> m_operator;
};

class flatten_factory {
public:
   	template<class Enumerable>
	flattened_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return flattened_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
} flatten;


template<class Enumerable>
flattened_sequence<Enumerable>
operator|(Enumerable &&enumerable, const flatten_factory &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Factory>
composite_factory<const flatten_factory &, Factory>
operator|(const flatten_factory &factory, Factory &&other){
	return make_composite(std::move(factory), std::forward<Factory>(other));
}

}
