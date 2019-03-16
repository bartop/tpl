
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <vector>

namespace tpl{

template<class Enumerable>
class cached_sequence : meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using cached_t = std::vector<value_type>;
	using const_iterator = typename cached_t::const_iterator;
	using iterator = typename cached_t::iterator;

	cached_sequence(
		Enumerable &&enumerable
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_wasFilled(false),
		m_cached() {}

	iterator
	begin() {
		fillCache(m_enumerable);
		return std::begin(m_cached);
	} 

	iterator
	end() {
		fillCache(m_enumerable);
		return std::end(m_cached);
	}

	const_iterator
	begin() const {
		fillCache(m_enumerable);
		return std::begin(m_cached);
	}

	const_iterator
	end() const {
		fillCache(m_enumerable);
		return std::end(m_cached);
	}

private:
	void
	fillCache(const Enumerable &enumerable) const {
		if(!m_wasFilled) {
			std::copy(
				enumerable_traits::begin(enumerable),
				enumerable_traits::end(enumerable),
				std::back_inserter(m_cached)
			);
			m_wasFilled = true;
		}
	}

	Enumerable m_enumerable;
	mutable bool m_wasFilled = false;
	mutable cached_t m_cached;
};

class cache_factory {
public:
	template<class Enumerable>
	cached_sequence<Enumerable>
	create(Enumerable &&enumerable) const {
		return cached_sequence<Enumerable>(
			std::forward<Enumerable>(enumerable)
		);
	}
} cache;

}
