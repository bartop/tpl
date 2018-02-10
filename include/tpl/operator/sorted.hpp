
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../common/composite_factory.hpp"

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

	sorted_sequence(
		Enumerable &&enumerable,
	   	Comparison &&compareComparison
	) :
		m_sorted(std::forward<Comparison>(compareComparison)),
		m_enumerable(std::forward<Enumerable>(enumerable)){ }

	sorted_sequence &operator=(sorted_sequence &) = delete;

	iterator
	begin() {
		sort();
		return std::begin(m_sorted);
	}

	iterator
	end() {
		return std::end(m_sorted);
	}

	const_iterator
	begin() const {
		sort();
		return std::begin(m_sorted);
	}

	const_iterator
	end() const {
		return std::end(m_sorted);
	}

private:
	void sort() const {
		m_sorted.clear();
		std::copy(
			std::begin(m_enumerable),
			std::end(m_enumerable),
			std::inserter(m_sorted, m_sorted.begin())
		);
	}

	mutable sorted_t m_sorted;
	Enumerable m_enumerable;
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

template<
	class Enumerable,
   	class Comparison,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
sorted_sequence<Enumerable, Comparison>
operator|(Enumerable &&enumerable, const compare_factory<Comparison> &factory) {
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
   	class Comparison,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
sorted_sequence<Enumerable, Comparison>
operator|(Enumerable &&enumerable, compare_factory<Comparison> &&factory){
	return std::move(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Factory, class Comparison>
composite_factory<const compare_factory<Comparison> &, Factory>
operator|(const compare_factory<Comparison> &factory, Factory &&other){
	return make_composite(
		factory,
	   	std::forward<Factory>(other)
	);
}

template<class Factory, class Comparison>
composite_factory<compare_factory<Comparison>, Factory>
operator|(compare_factory<Comparison> &&factory, Factory &&other){
	return make_composite(
		std::move(factory),
	   	std::forward<Factory>(other)
	);
}

}
