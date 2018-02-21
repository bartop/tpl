
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include "../common/composite_factory.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class SubIterator>
class keys_iterator : public detail::iterator_base<keys_iterator<SubIterator>> {
public:
	using sub_traits_t = std::iterator_traits<SubIterator>;
	using associative_traits_t = meta::associative_element_traits<typename sub_traits_t::value_type>;
	using value_type = typename associative_traits_t::key_type;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = value_type;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	~keys_iterator() noexcept = default;

	keys_iterator(
		SubIterator subIterator
	) :
		m_subIterator(std::move(subIterator)) { }

	keys_iterator &
	operator++() {
		++m_subIterator;
		return *this;
	}

	reference
	operator*() const {
		return associative_traits_t::key_value(*(this->m_subIterator));
	}

	pointer
	operator->() const {
		return &associative_traits_t::key_value(*(this->m_subIterator));
	}

	bool
	operator==(const keys_iterator &other) const {
		return this->m_subIterator == other.m_subIterator;
	}

private:
	SubIterator m_subIterator;
};

template<class Enumerable>
class keys_operator {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = keys_iterator<typename enumerable_traits::const_iterator>;
	using iterator = keys_iterator<typename enumerable_traits::iterator>;

	iterator
	first(Enumerable &enumerable) {
		return iterator(std::begin(enumerable));
	}

	iterator
	last(Enumerable &enumerable) {
		return iterator(std::end(enumerable));
	}

	const_iterator
	first(const Enumerable &enumerable) const {
		return const_iterator(std::begin(enumerable));
	}

	const_iterator
	last(const Enumerable &enumerable) const {
		return const_iterator(std::end(enumerable));
	}
};

template<class Enumerable>
class keys_sequence :
	meta::enforce_enumerable<Enumerable>,
	meta::enforce_associative<Enumerable> {
public:
	using operator_t = keys_operator<Enumerable>;
	using value_type = typename operator_t::value_type;
	using const_iterator = typename operator_t::const_iterator;
	using iterator = typename operator_t::iterator;

	keys_sequence(
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
	keys_operator<Enumerable> m_operator;
};

class keys_factory {
public:
   	template<class Enumerable>
	keys_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return keys_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
} keys;


template<class Enumerable>
keys_sequence<Enumerable>
operator|(Enumerable &&enumerable, const keys_factory &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Factory>
composite_factory<const keys_factory &, Factory>
operator|(const keys_factory &factory, Factory &&other){
	return make_composite(std::move(factory), std::forward<Factory>(other));
}

}
