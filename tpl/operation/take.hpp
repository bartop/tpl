
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{
namespace operation{

template<class SubIterator>
class taking_iterator :
	public detail::iterator_base<taking_iterator<SubIterator>> {
public:
	using sub_traits_t = std::iterator_traits<SubIterator>;
	using value_type = typename sub_traits_t::value_type;
	using difference_type = typename sub_traits_t::difference_type;
	using reference = const value_type &;
	using pointer = const value_type *;
	using iterator_category = std::input_iterator_tag;

	~taking_iterator() noexcept = default;

	taking_iterator(
		SubIterator subIterator,
		unsigned toTake
	) :
		m_subIterator(std::move(subIterator)),
		m_toTake(toTake) {}

	taking_iterator &
	operator++() {
		++m_subIterator;
		return *this;
	}

	reference
	operator*() const {
		return *m_subIterator;
	}

	pointer
	operator->() const {
		return m_subIterator.operator->();
	}

	bool
	operator==(const taking_iterator &other) const {
		return this->m_subIterator == other.m_subIterator;
	}

	void swap(taking_iterator &other) {
		std::swap(this->m_subIterator, other.m_subIterator);
		std::swap(this->m_toTake, other.m_toTake);
	}
private:
	SubIterator m_subIterator;
	unsigned m_toTake;
};

template<class Enumerable>
class taken_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = taking_iterator<typename enumerable_traits::const_iterator>;
	using iterator = taking_iterator<typename enumerable_traits::iterator>;

	taken_sequence(
		Enumerable &&enumerable,
		unsigned toTake
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_toTake(toTake) {}

	void
	swap(taken_sequence &other){
		std::swap(m_enumerable, other.m_enumerable);
		std::swap(m_toTake, other.m_toTake);
	}

	iterator
	begin() {
		return iterator(
			std::begin(m_enumerable),
			m_toTake
		);
	}

	iterator
	end() {
		const auto end = std::distance(std::begin(m_enumerable), std::end(m_enumerable)) > m_toTake ?
			std::next(std::begin(m_enumerable), m_toTake) :
			std::end(m_enumerable);
		return iterator(
			end,
			m_toTake
		);
	}

	const_iterator
	begin() const {
		return const_iterator(
			std::begin(m_enumerable),
			m_toTake
		);
	}

	const_iterator
	end() const {
		const auto end = std::distance(std::begin(m_enumerable), std::end(m_enumerable)) > m_toTake ?
			std::next(std::begin(m_enumerable), m_toTake) :
			std::end(m_enumerable);
		return const_iterator(
			end,
			m_toTake
		);
	}
private:
	Enumerable m_enumerable;
	unsigned m_toTake;
};

class take_factory {
public:
	explicit take_factory(unsigned toTake) :
		m_toTake(toTake){}

	template<class Enumerable>
	taken_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return taken_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable),
			m_toTake
		);
	}

private:
	unsigned m_toTake;
};

take_factory
take(unsigned toTake){
	return take_factory(toTake);
}

template<class Enumerable>
taken_sequence<Enumerable>
operator|(
	Enumerable &&enumerable,
   	take_factory &&factory
){
	return factory.create<Enumerable>(
		std::forward<Enumerable>(enumerable)
	);
}

}
}
