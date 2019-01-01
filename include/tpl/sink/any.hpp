
#pragma once

#include "../common/sink.hpp"
#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

namespace tpl{

template<class Enumerable, class LogicalPredicate>
class true_for_any {
public:
	true_for_any(LogicalPredicate &&predicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(predicate)) {}

	bool
	operator()(Enumerable &enumerable) {
		for(auto &element : enumerable) {
			if(m_logicalPredicate(element))
				return true;
		}
		return false;
	}

	bool
	operator()(const Enumerable &enumerable) const {
		for(const auto &element : enumerable) {
			if(m_logicalPredicate(element))
				return true;
		}
		return false;
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

template<class LogicalPredicate>
any_sink_factory<LogicalPredicate>
any(LogicalPredicate &&logicalPredicate) {
	return any_sink_factory<LogicalPredicate>(
		std::forward<LogicalPredicate>(logicalPredicate)
	);
}

}
