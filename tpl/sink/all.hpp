
#pragma once

#include <iterator>

namespace tpl{
namespace sink{

template<class Enumerable, class LogicalPredicate>
class true_for_all {
public:
	true_for_all(Enumerable &&enumerable, LogicalPredicate &&logicalPredicate) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	operator bool() const {
		for(const auto &element : m_enumerable) {
			if(!m_logicalPredicate(element))
				return false;
		}
		return true;
	}

	void
	swap(true_for_all &other){
		std::swap(m_enumerable, other.m_enumerable);
		std::swap(m_logicalPredicate, other.m_logicalPredicate);
	}

private:
	Enumerable m_enumerable;
	LogicalPredicate m_logicalPredicate;
};


template<class LogicalPredicate>
class true_for_all_factory {
public:
	explicit true_for_all_factory(LogicalPredicate &&logicalPredicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	template<class Enumerable>
	true_for_all<Enumerable, LogicalPredicate>
	create(Enumerable &&enumerable) const & {
		return true_for_all<Enumerable, LogicalPredicate>(
			std::forward<Enumerable>(enumerable),
			m_logicalPredicate
		);
	}

	template<class Enumerable>
	true_for_all<Enumerable, LogicalPredicate>
	create(Enumerable &&enumerable) && {
		return true_for_all<Enumerable, LogicalPredicate>(
			std::forward<Enumerable>(enumerable),
			std::forward<LogicalPredicate>(m_logicalPredicate)
		);
	}
private:
	LogicalPredicate m_logicalPredicate;
};

template<class LogicalPredicate>
true_for_all_factory<LogicalPredicate>
all(LogicalPredicate &&logicalPredicate){
	return true_for_all_factory<LogicalPredicate>(
		std::forward<LogicalPredicate>(logicalPredicate)
	);
}

template<class Enumerable, class LogicalPredicate>
true_for_all<Enumerable, LogicalPredicate>
operator|(
	Enumerable &&enumerable,
   	true_for_all_factory<LogicalPredicate> &&factory
){
	return std::forward<true_for_all_factory<LogicalPredicate>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

}
}
