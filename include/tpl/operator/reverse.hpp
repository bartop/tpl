
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"
#include "../meta/iterators.hpp"

#include "../detail/iterator_base.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class Enumerable>
class reversing_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = std::reverse_iterator<
		typename enumerable_traits::const_iterator
	>;
	using iterator = std::reverse_iterator<
		typename enumerable_traits::iterator
	>;

	reversing_sequence(Enumerable &&enumerable) :
		m_enumerable(std::forward<Enumerable>(enumerable)) {}

	iterator
	begin() {
		return meta::make_reverse_iterator(enumerable_traits::end(m_enumerable));
	}

	iterator
	end() {
		return meta::make_reverse_iterator(enumerable_traits::begin(m_enumerable));
	}

	const_iterator
	begin() const {
		return meta::make_reverse_iterator(enumerable_traits::end(m_enumerable));
	}

	const_iterator
	end() const {
		return meta::make_reverse_iterator(enumerable_traits::begin(m_enumerable));
	}

private:
	Enumerable m_enumerable;
};

class reverse_factory {
public:
	template<class Enumerable>
	reversing_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return reversing_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
};

const reverse_factory reverse;

}
