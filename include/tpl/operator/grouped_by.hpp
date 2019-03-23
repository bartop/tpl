
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include <iterator>
#include <vector>
#include <unordered_map>

namespace tpl{

template<class Enumerable, class Grouping>
class grouped_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using key_type = decltype(std::declval<Grouping>()(
		std::declval<typename enumerable_traits::value_type>())
	);
	using mapped_type = std::vector<typename enumerable_traits::value_type>;
	using grouped_t = std::unordered_map<key_type, mapped_type>;
	using value_type = typename grouped_t::value_type;
	using iterator = typename grouped_t::iterator;
	using const_iterator = typename grouped_t::const_iterator;

	template<class T>
	grouped_sequence(
		Enumerable &&enumerable,
	   	T &&op
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_grouped(),
		m_groupingFunction(op){}

	iterator
	begin() {
		group(m_enumerable);
		return std::begin(m_grouped);
	}

	iterator
	end() {
		return std::end(m_grouped);
	}

	const_iterator
	begin() const {
		group(m_enumerable);
		return std::begin(m_grouped);
	}

	const_iterator
	end() const {
		return std::end(m_grouped);
	}

private:
	void
	group(const Enumerable &enumerable) const {
		m_grouped.clear();
		for (const auto &value : enumerable)
			m_grouped[m_groupingFunction(value)].push_back(value);
	}

	Enumerable m_enumerable;
	mutable grouped_t m_grouped;
	Grouping m_groupingFunction;
};

template<class Enumerable, class Grouping>
grouped_sequence<Enumerable, Grouping>
make_grouped(Enumerable &&enumerable, Grouping &&predicate){
	return grouped_sequence<Enumerable, Grouping>(
		std::forward<Enumerable>(enumerable),
		std::forward<Grouping>(predicate)
	);
}

template<class Grouping>
class grouping_factory {
public:
	explicit grouping_factory(Grouping &&grouping) :
		m_grouping(std::forward<Grouping>(grouping)){}

	template<class Enumerable>
	grouped_sequence<Enumerable, const Grouping &>
	create(Enumerable &&enumerable) const & {
		return make_grouped(
			std::forward<Enumerable>(enumerable),
			m_grouping
		);
	}

	template<class Enumerable>
	grouped_sequence<Enumerable, Grouping>
	create(Enumerable &&enumerable) && {
		return make_grouped(
			std::forward<Enumerable>(enumerable),
			std::forward<Grouping>(m_grouping)
		);
	}
private:
	Grouping m_grouping;
};

template<class Grouping>
grouping_factory<Grouping>
group_by(Grouping &&grouping){
	return grouping_factory<Grouping>(std::forward<Grouping>(grouping));
}

}
