
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class Enumerable>
class dropping_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = typename enumerable_traits::const_iterator;
	using iterator = typename enumerable_traits::iterator;

	dropping_sequence(
		Enumerable &&enumerable,
		unsigned toDrop
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_toDrop(toDrop) {}

	iterator
	begin() {
		return std::distance(enumerable_traits::begin(m_enumerable),
			enumerable_traits::end(m_enumerable)) > m_toDrop ?
				std::next(enumerable_traits::begin(m_enumerable), m_toDrop) :
				enumerable_traits::end(m_enumerable
		);
	}

	iterator
	end() {
		return enumerable_traits::end(m_enumerable);
	}

	const_iterator
	begin() const {
		return std::distance(enumerable_traits::begin(m_enumerable), 
			enumerable_traits::end(m_enumerable)) > m_toDrop ?
				std::next(enumerable_traits::begin(m_enumerable), m_toDrop) :
				enumerable_traits::end(m_enumerable
		);
	}

	const_iterator
	end() const {
		return enumerable_traits::end(m_enumerable);
	}

private:
	Enumerable m_enumerable;
	unsigned m_toDrop;
};

class drop_factory {
public:
	explicit drop_factory(unsigned toDrop) :
		m_toDrop(toDrop){}

	template<class Enumerable>
	dropping_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return dropping_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable),
			m_toDrop
		);
	}

private:
	unsigned m_toDrop;
};

drop_factory
drop(unsigned toDrop){
	return drop_factory(toDrop);
}

}
