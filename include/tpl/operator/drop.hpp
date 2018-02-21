
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../detail/iterator_base.hpp"

#include "../common/composite_factory.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class Enumerable>
class dropping_operator {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = typename enumerable_traits::const_iterator;
	using iterator = typename enumerable_traits::iterator;

	dropping_operator(unsigned toDrop) : m_toDrop(toDrop) {}

	iterator
	first(Enumerable &enumerable) {
		return std::distance(std::begin(enumerable), std::end(enumerable)) > m_toDrop ?
			std::next(std::begin(enumerable), m_toDrop) :
			std::end(enumerable);
	}

	iterator
	last(Enumerable &enumerable) {
		return std::end(enumerable);
	}

	const_iterator
	first(const Enumerable &enumerable) const {
		return std::distance(std::begin(enumerable), std::end(enumerable)) > m_toDrop ?
			std::next(std::begin(enumerable), m_toDrop) :
			std::end(enumerable);
	}

	const_iterator
	last(const Enumerable &enumerable) const {
		return std::end(enumerable);
	}

private:
	unsigned m_toDrop;
};

template<class Enumerable>
class dropping_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using operator_t = dropping_operator<Enumerable>;
	using value_type = typename operator_t::value_type;
	using const_iterator = typename operator_t::const_iterator;
	using iterator = typename operator_t::iterator;

	template<class T>
	dropping_sequence(
		Enumerable &&enumerable,
		T &&op
	) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_operator(std::forward<T>(op)) {}

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
	dropping_operator<Enumerable> m_operator;
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

template<class Enumerable>
dropping_sequence<Enumerable>
operator|(Enumerable &&enumerable, const drop_factory &factory){
	return factory.create(std::forward<Enumerable>(enumerable));
}

template<class Enumerable>
dropping_sequence<Enumerable>
operator|(Enumerable &&enumerable, drop_factory &&factory){
	return std::move(factory).create(std::forward<Enumerable>(enumerable));
}

template<class Factory>
composite_factory<const drop_factory &, Factory>
operator|(const drop_factory &factory, Factory &&other){
	return make_composite(factory, std::forward<Factory>(other));
}

template<class Factory>
composite_factory<drop_factory, Factory>
operator|(drop_factory &&factory, Factory &&other){
	return make_composite(std::move(factory), std::forward<Factory>(other));
}

}
