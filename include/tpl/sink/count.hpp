/**
 * \file
 * \brief File defining sink which counts number of elements compliant with
 *     given predicate.
 */

#pragma once

#include "../common/sink.hpp"
#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

#include "../meta/enumerable_traits.hpp"

#include <algorithm>

namespace tpl{

template<class Enumerable, class LogicalPredicate>
class count_compliant {
public:
	count_compliant(LogicalPredicate &&logicalPredicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	unsigned
	operator()(Enumerable &enumerable) {
		using traits = meta::enumerable_traits<Enumerable>;
		return static_cast<unsigned>(
			std::count_if(
				traits::begin(enumerable),
				traits::end(enumerable),
				m_logicalPredicate
		));
	}

	unsigned
	operator()(const Enumerable &enumerable) const {
		using traits = meta::enumerable_traits<Enumerable>;
		return static_cast<unsigned>(
				std::count_if(
				traits::begin(enumerable),
				traits::end(enumerable),
				m_logicalPredicate
		));
	}

private:
	LogicalPredicate m_logicalPredicate;
};

template<class Enumerable, class Predicate>
count_compliant<Enumerable, Predicate>
make_count(Predicate &&predicate){
	return count_compliant<Enumerable, Predicate>(
		std::forward<Predicate>(predicate)
	);
}

template<class LogicalPredicate>
class count_factory {
public:
	explicit count_factory(LogicalPredicate &&logicalPredicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	template<class Enumerable>
	sink<Enumerable, count_compliant<Enumerable, const LogicalPredicate &>>
	create(Enumerable &&enumerable) const & {
		return make_sink(
			std::forward<Enumerable>(enumerable),
			make_count<Enumerable>(m_logicalPredicate)
		);
	}

	template<class Enumerable>
	sink<Enumerable, count_compliant<Enumerable, LogicalPredicate>>
	create(Enumerable &&enumerable) && {
		return make_sink(
			std::forward<Enumerable>(enumerable),
			make_count<Enumerable>(
				std::forward<LogicalPredicate>(m_logicalPredicate)
			)
		);
	}
private:
	LogicalPredicate m_logicalPredicate;
};

/**
 * \brief Sink counting number of elements complying with given predicate.
 *
 * This is a sink, which means it can be used as final part of a pipeline.
 *
 * \tparam LogicalPredicate Type of output iterator. Its value_type must be
 *     constructible from input sequence value_type.
 *
 * \param logicalPredicate Predicate telling returning true when given 
 *     element is to be counted by sink and false otherwise.
 *
 *  **Example**
 *
 *     std::vector<int> v = {1, 2, 3, 4, 5};
 *     const unsigned out = v | tpl::count([](auto i){ return i > 3;});
 *     // out is 2
 */
template<class LogicalPredicate>
count_factory<LogicalPredicate>
count(LogicalPredicate &&logicalPredicate){
	return count_factory<LogicalPredicate>(
		std::forward<LogicalPredicate>(logicalPredicate)
	);
}

}
