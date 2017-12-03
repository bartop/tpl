
#pragma once

#include "meta/is_enumerable.hpp"
#include "meta/is_associative.hpp"
#include "meta/enumerable_traits.hpp"
#include "pointer_proxy.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class Enumerable>
struct zipped_enumerable_holder {
	zipped_enumerable_holder(Enumerable &&enumerable) :
		m_enumerable(std::forward<Enumerable>(enumerable)){}

	Enumerable m_enumerable;
};

template<class SubIterator1, class SubIterator2>
class zipped_iterator {
public:
	using sub_traits_t1 = std::iterator_traits<SubIterator1>;
	using sub_traits_t2 = std::iterator_traits<SubIterator2>;
	using value_type = std::pair<typename sub_traits_t1::value_type, typename sub_traits_t2::value_type>;
	using difference_type = typename sub_traits_t1::difference_type;
	using reference = value_type;
	using pointer = detail::pointer_proxy<value_type>;
	using iterator_category = std::input_iterator_tag;

	~zipped_iterator() noexcept = default;

	zipped_iterator(
		SubIterator1 subIterator1,
		SubIterator2 subIterator2
	) :
		m_subIterator1(std::move(subIterator1)),
		m_subIterator2(std::move(subIterator2)) {}

	zipped_iterator &
	operator++() {
		++m_subIterator1;
		++m_subIterator2;
		return *this;
	}

	zipped_iterator
	operator++(int) {
		const auto thisCopy = *this;
		++*this;
		return thisCopy;
	}

	reference
	operator*() const {
		return std::make_pair(*m_subIterator1, *m_subIterator2);
	}

	pointer
	operator->() const {
		return detail::make_pointer_proxy(
			std::make_pair(*m_subIterator1, *m_subIterator2)
		);
	}

	bool
	operator==(const zipped_iterator &other) const {
		return this->m_subIterator1 == other.m_subIterator1 ||
			this->m_subIterator2 == other.m_subIterator2;
	}

	bool
	operator!=(const zipped_iterator &other) const {
		return !(*this == other);
	}

	void
	swap(zipped_iterator &filteringIterator) {
		std::swap(this->m_subIterator1, filteringIterator.m_subIterator1);
		std::swap(this->m_subIterator2, filteringIterator.m_subIterator2);
	}
private:
	SubIterator1 m_subIterator1;
	SubIterator2 m_subIterator2;
};

template<class Enumerable1, class Enumerable2>
class filtered_sequence :
	meta::enforce_enumerable<Enumerable1> {
public:
	using enumerable_traits1 = meta::enumerable_traits<Enumerable1>;
	using enumerable_traits2 = meta::enumerable_traits<Enumerable2>;
	using value_type = std::pair<
		typename enumerable_traits1::value_type,
		typename enumerable_traits2::value_type
	>;
	using const_iterator = zipped_iterator<
		typename enumerable_traits1::const_iterator,
		typename enumerable_traits2::const_iterator
	>;
	using iterator = zipped_iterator<
		typename enumerable_traits1::iterator,
		typename enumerable_traits2::iterator
	>;

	filtered_sequence(
		Enumerable1 &&enumerable1,
		Enumerable2 &&enumerable2
	) :
		m_enumerable1(std::forward<Enumerable1>(enumerable1)),
		m_enumerable2(std::forward<Enumerable2>(enumerable2)){}

	void
	swap(filtered_sequence &other){
		std::swap(m_enumerable1, other.m_enumerable1);
		std::swap(m_enumerable2, other.m_enumerable2);
	}

	iterator
	begin() {
		return iterator(
			std::begin(m_enumerable1),
		   	std::begin(m_enumerable2)
		);
	}

	iterator
	end() {
		return iterator(
			std::end(m_enumerable1),
		   	std::end(m_enumerable2)
		);
	}

	const_iterator
	begin() const {
		return const_iterator(
			std::begin(m_enumerable1),
		   	std::begin(m_enumerable2)
		);
	}

	const_iterator
	end() const {
		return const_iterator(
			std::end(m_enumerable1),
		   	std::end(m_enumerable2)
		);
	}
private:
	Enumerable1 m_enumerable1;
	Enumerable2 m_enumerable2;
};

template<class Enumerable>
zipped_enumerable_holder<Enumerable>
zip(Enumerable &&enumerable){
	return zipped_enumerable_holder<Enumerable>(std::forward<Enumerable>(enumerable));
}

template<class Enumerable1, class Enumerable2>
filtered_sequence<Enumerable1, Enumerable2>
operator|(
	Enumerable1 &&enumerable,
   	zipped_enumerable_holder<Enumerable2> &&holder
){
	return filtered_sequence<Enumerable1, Enumerable2>(
		std::forward<Enumerable1>(enumerable),
	   	std::forward<zipped_enumerable_holder<Enumerable2>>(holder).m_enumerable
	);
}

}
