
#pragma once

#include "../common/sink.hpp"
#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

namespace tpl{

template<class Enumerable, class LogicalPredicate>
class count_compliant {
public:
	count_compliant(LogicalPredicate &&logicalPredicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	unsigned
	operator()(Enumerable &enumerable) {
		unsigned elements_number = 0;
		for(auto &element : enumerable) {
			if(m_logicalPredicate(element))
				++elements_number;
		}
		return elements_number;
	}

	unsigned
	operator()(const Enumerable &enumerable) const {
		unsigned elements_number = 0;
		for(const auto &element : enumerable) {
			if(m_logicalPredicate(element))
				++elements_number;
		}
		return elements_number;
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

template<class LogicalPredicate>
count_factory<LogicalPredicate>
count(LogicalPredicate &&logicalPredicate){
	return count_factory<LogicalPredicate>(
		std::forward<LogicalPredicate>(logicalPredicate)
	);
}

}
