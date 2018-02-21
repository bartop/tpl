
#pragma once

#include "../meta/is_enumerable.hpp"
#include "../meta/is_associative.hpp"
#include "../meta/enumerable_traits.hpp"

#include "../common/composite_factory.hpp"

#include <iterator>
#include <algorithm>

namespace tpl{

template<class Enumerable>
class taking_operator {
public:
	using enumerable_traits = meta::enumerable_traits<Enumerable>;
	using value_type = typename enumerable_traits::value_type;
	using const_iterator = typename enumerable_traits::const_iterator;
	using iterator = typename enumerable_traits::iterator;

	taking_operator(unsigned toTake) : m_toTake(toTake) {}

	iterator
	first(Enumerable &enumerable) {
		return std::begin(enumerable);
	}

	iterator
	last(Enumerable &enumerable) {
		return std::distance(std::begin(enumerable), std::end(enumerable)) > m_toTake ?
			std::next(std::begin(enumerable), m_toTake) :
			std::end(enumerable);
	}

	const_iterator
	first(const Enumerable &enumerable) const {
		return std::begin(enumerable);
	}

	const_iterator
	last(const Enumerable &enumerable) const {
		return std::distance(std::begin(enumerable), std::end(enumerable)) > m_toTake ?
			std::next(std::begin(enumerable), m_toTake) :
			std::end(enumerable);
	}

private:
	unsigned m_toTake;
};

template<class Enumerable>
class taken_sequence :
	meta::enforce_enumerable<Enumerable> {
public:
	using operator_t = taking_operator<Enumerable>;
	using value_type = typename operator_t::value_type;
	using const_iterator = typename operator_t::const_iterator;
	using iterator = typename operator_t::iterator;

	template<class T>
	taken_sequence(
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
	taking_operator<Enumerable> m_operator;
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

template<class Enumerable>
taken_sequence<Enumerable>
operator|(Enumerable &&enumerable, take_factory &&factory){
	return std::move(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Enumerable>
taken_sequence<Enumerable>
operator|(Enumerable &&enumerable, const take_factory &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Factory>
composite_factory<const take_factory &, Factory>
operator|(const take_factory &factory, Factory &&other){
	return make_composite(
		factory,
	   	std::forward<Factory>(other)
	);
}

template<class Factory>
composite_factory<take_factory, Factory>
operator|(take_factory &&factory, Factory &&other){
	return make_composite(
		std::move(factory),
	   	std::forward<Factory>(other)
	);
}

}
