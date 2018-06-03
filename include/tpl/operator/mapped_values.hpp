
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
class mapped_values_iterator :
	public detail::iterator_base<mapped_values_iterator<SubIterator>> {
public:
	using sub_traits_t = std::iterator_traits<SubIterator>;
	using associative_traits_t = meta::associative_element_traits<typename sub_traits_t::value_type>;
	using value_type = typename associative_traits_t::key_type;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = value_type;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	~mapped_values_iterator() noexcept = default;

	mapped_values_iterator(
		SubIterator subIterator
	) :
		m_subIterator(std::move(subIterator)) { }

	mapped_values_iterator &
	operator++() {
		++m_subIterator;
		return *this;
	}

	reference
	operator*() const {
		return associative_traits_t::mapped_value(*(this->m_subIterator));
	}

	pointer
	operator->() const {
		return &associative_traits_t::mapped_value(*(this->m_subIterator));
	}

	bool
	operator==(const mapped_values_iterator &other) const {
		return this->m_subIterator == other.m_subIterator;
	}

private:
	SubIterator m_subIterator;
};

template<class Enumerable>
class mapped_values_sequence :
	meta::enforce_enumerable<Enumerable>,
	meta::enforce_associative<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = mapped_values_iterator<typename enumerable_traits::const_iterator>;
	using iterator = mapped_values_iterator<typename enumerable_traits::iterator>;

	mapped_values_sequence(
		Enumerable &&enumerable
	) : m_enumerable(std::forward<Enumerable>(enumerable)) {}

	iterator
	begin() {
		return iterator(std::begin(m_enumerable));
	}

	iterator
	end() {
		return iterator(std::begin(m_enumerable));
	}

	const_iterator
	begin() const {
		return const_iterator(std::begin(m_enumerable));
	}

	const_iterator
	end() const {
		return const_iterator(std::end(m_enumerable));
	}
private:
	Enumerable m_enumerable;
};

class mapped_values_factory {
public:
   	template<class Enumerable>
	mapped_values_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return mapped_values_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
} mapped_values;

template<class Enumerable>
mapped_values_sequence<Enumerable>
operator|(Enumerable &&enumerable, const mapped_values_factory &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Factory>
composite_factory<const mapped_values_factory &, Factory>
operator|(const mapped_values_factory &factory, Factory &&other){
	return make_composite(
		factory,
	   	std::forward<Factory>(other)
	);
}

}
