
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
class no_operation_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = typename enumerable_traits::const_iterator;
	using iterator = typename enumerable_traits::iterator;

	no_operation_sequence(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)) {}

	iterator
	begin() {
		return enumerable_traits::begin(m_enumerable);
	}

	iterator
	end() {
		return enumerable_traits::end(m_enumerable);
	}

	const_iterator
	begin() const {
		return enumerable_traits::begin(m_enumerable);
	}

	const_iterator
	end() const {
		return enumerable_traits::end(m_enumerable);
	}

private:
	Enumerable m_enumerable;
};

class no_operation_factory {
public:
	template<class Enumerable>
	no_operation_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return no_operation_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
};

constexpr no_operation_factory no_operation;

}
