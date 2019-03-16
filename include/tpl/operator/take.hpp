
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class Enumerable>
class taken_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = typename enumerable_traits::const_iterator;
	using iterator = typename enumerable_traits::iterator;

	taken_sequence(
		Enumerable &&enumerable,
		unsigned toTake
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_toTake(toTake) {}

	iterator
	begin() {
		return enumerable_traits::begin(m_enumerable);
	}

	iterator
	end() {
		return std::distance(enumerable_traits::begin(m_enumerable), enumerable_traits::end(m_enumerable)) > m_toTake ?
			std::next(enumerable_traits::begin(m_enumerable), m_toTake) :
			enumerable_traits::end(m_enumerable);
	}

	const_iterator
	begin() const {
		return enumerable_traits::begin(m_enumerable);
	}

	const_iterator
	end() const {
		return std::distance(enumerable_traits::begin(m_enumerable), enumerable_traits::end(m_enumerable)) > m_toTake ?
			std::next(enumerable_traits::begin(m_enumerable), m_toTake) :
			enumerable_traits::end(m_enumerable);
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

}
