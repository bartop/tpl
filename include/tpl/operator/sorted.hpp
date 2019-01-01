
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <set>

namespace tpl{

template<class Enumerable, class Comparison>
class sorted_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using sorted_t = std::multiset<value_type, Comparison>;
	using const_iterator = typename sorted_t::const_iterator;
	using iterator = typename sorted_t::iterator;

	template<class T>
	sorted_sequence(
		Enumerable &&enumerable,
	   	T &&op
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_sorted(std::forward<T>(op)) {}

	sorted_sequence &operator=(sorted_sequence &) = delete;

	iterator
	begin() {
		sort(m_enumerable);
		return std::begin(m_sorted);
	}

	iterator
	end() {
		return std::end(m_sorted);
	}

	const_iterator
	begin() const {
		sort(m_enumerable);
		return std::begin(m_sorted);
	}

	const_iterator
	end() const {
		return std::end(m_sorted);
	}

private:
	void
	sort(const Enumerable &enumerable) const {
		m_sorted.clear();
		std::copy(
			std::begin(enumerable),
			std::end(enumerable),
			std::inserter(m_sorted, m_sorted.begin())
		);
	}

	Enumerable m_enumerable;
	mutable sorted_t m_sorted;
};

template<class Enumerable, class Comparison>
sorted_sequence<Enumerable, Comparison>
make_sorted(Enumerable &&enumerable, Comparison &&predicate){
	return sorted_sequence<Enumerable, Comparison>(
		std::forward<Enumerable>(enumerable),
		std::forward<Comparison>(predicate)
	);
}

template<class Comparison>
class compare_factory {
public:
	explicit compare_factory(Comparison &&compareComparison) :
		m_comparison(std::forward<Comparison>(compareComparison)){}

	template<class Enumerable>
	sorted_sequence<Enumerable, const Comparison &>
	create(Enumerable &&enumerable) const & {
		return make_sorted(
			std::forward<Enumerable>(enumerable),
			m_comparison
		);
	}

	template<class Enumerable>
	sorted_sequence<Enumerable, Comparison>
	create(Enumerable &&enumerable) && {
		return make_sorted(
			std::forward<Enumerable>(enumerable),
			std::forward<Comparison>(m_comparison)
		);
	}
private:
	Comparison m_comparison;
};

template<class Comparison>
compare_factory<Comparison>
sort(Comparison &&compareComparison){
	return compare_factory<Comparison>(std::forward<Comparison>(compareComparison));
}

}
