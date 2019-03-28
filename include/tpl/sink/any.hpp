/**
 * \file
 * \brief File defining sink which checks if any element in sequence complies
 *     with given predicate.
 */

#pragma once

#include "../common/sink.hpp"
#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include "../meta/enumerable_traits.hpp"

#include <algorithm>

namespace tpl{

template<class Enumerable, class LogicalPredicate>
class true_for_any {
public:
	true_for_any(LogicalPredicate &&predicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(predicate)) {}

	bool
	operator()(Enumerable &enumerable) {
		using traits = meta::enumerable_traits<Enumerable>;
		return std::any_of(traits::begin(enumerable), traits::end(enumerable),
				m_logicalPredicate
		);
	}

	bool
	operator()(const Enumerable &enumerable) const {
		using traits = meta::enumerable_traits<Enumerable>;
		return std::any_of(traits::begin(enumerable), traits::end(enumerable),
				m_logicalPredicate
		);
	}

private:
	LogicalPredicate m_logicalPredicate;
};

template<class Enumerable, class Predicate>
true_for_any<Enumerable, Predicate>
make_true_for_any(Predicate &&predicate) {
	return true_for_any<Enumerable, Predicate>(
		std::forward<Predicate>(predicate)
	);
}

template<class Predicate>
class any_sink_factory {
public:
	explicit any_sink_factory(Predicate &&predicate) :
		m_predicate(std::forward<Predicate>(predicate)){}

	template<class Enumerable>
	sink<Enumerable, true_for_any<Enumerable, const Predicate &>>
	create(Enumerable &&enumerable) const & {
		return make_sink(
			std::forward<Enumerable>(enumerable),
			make_true_for_any<Enumerable>(m_predicate)
		);
	}

	template<class Enumerable>
	sink<Enumerable, true_for_any<Enumerable, Predicate>>
	create(Enumerable &&enumerable) && {
		return make_sink(
			std::forward<Enumerable>(enumerable),
			make_true_for_any<Enumerable>(
				std::forward<Predicate>(m_predicate)
			)
		);
	}

private:
	Predicate m_predicate;
};

/**
 * \brief Sink checking if any element in input sequence is compliant with
 *     given predicate.
 *
 * This is a sink, which means it can be used as final part of a pipeline.
 *
 * \tparam LogicalPredicate Type of function-like object which will be called
 *     on subsequent elements to check compliance with it. It must take one
 *     argument constructible from input sequence value_type and return value
 *     convertible to bool.
 *
 * \param logicalPredicate Function which will be tested on subsequent elements.
 *
 *  **Example**
 *
 *     std::vector<int> v = {1, 2, 3, 4, 5};
 *     const bool out1 = v | tpl::any([](auto i){ return i < 0; });
 *     const bool out2 = v | tpl::any([](auto i){ return i < 2; });
 *     // out1 == false and out2 == true
 */
template<class LogicalPredicate>
any_sink_factory<LogicalPredicate>
any(LogicalPredicate &&logicalPredicate) {
	return any_sink_factory<LogicalPredicate>(
		std::forward<LogicalPredicate>(logicalPredicate)
	);
}

}
