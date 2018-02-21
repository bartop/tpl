
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../common/composite_factory.hpp"

#include <iterator>
#include <vector>

namespace tpl{

template<class Enumerable>
class caching_operator{
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using cached_t = std::vector<value_type>;
	using const_iterator = typename cached_t::const_iterator;
	using iterator = typename cached_t::iterator;

	caching_operator() :
		m_wasFilled(false),
		m_cached() {}

	iterator
	first(Enumerable &enumerable) {
		fillCache(enumerable);
		return std::begin(m_cached);
	}

	iterator
	last(Enumerable &enumerable) {
		fillCache(enumerable);
		return std::end(m_cached);
	}

	const_iterator
	first(const Enumerable &enumerable) const {
		fillCache(enumerable);
		return std::begin(m_cached);
	}

	const_iterator
	last(const Enumerable &enumerable) const {
		fillCache(enumerable);
		return std::end(m_cached);
	}

private:
	void
	fillCache(const Enumerable &enumerable) const {
		if(!m_wasFilled) {
			std::copy(
				std::begin(enumerable),
				std::end(enumerable),
				std::back_inserter(m_cached)
			);
			m_wasFilled = true;
		}
	}

	mutable bool m_wasFilled = false;
	mutable cached_t m_cached;
};

template<class Enumerable>
class cached_sequence : meta::enforce_enumerable<Enumerable> {
public:
	using operator_t = caching_operator<Enumerable>;
	using enumerable_traits = typename operator_t::enumerable_traits;
	using value_type = typename operator_t::value_type;
	using const_iterator = typename operator_t::const_iterator;
	using iterator = typename operator_t::iterator;

	cached_sequence(
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
	caching_operator<Enumerable> m_operator;
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

template<
	class Enumerable,
   	class = typename std::enable_if<meta::is_enumerable<Enumerable>::value>::type
>
cached_sequence<Enumerable>
operator|(Enumerable &&enumerable, const cache_factory &factory) {
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Factory>
composite_factory<const cache_factory &, Factory>
operator|(const cache_factory &factory, Factory &&other){
	return make_composite(
		factory,
	   	std::forward<Factory>(other)
	);
}

}
