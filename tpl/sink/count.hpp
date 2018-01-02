
#pragma once

#include <iterator>

namespace tpl{
namespace sink{

template<class LogicalPredicate>
struct count_predicate_holder {
	explicit count_predicate_holder(LogicalPredicate &&logicalPredicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	LogicalPredicate m_logicalPredicate;
};

template<class Enumerable, class LogicalPredicate>
class count_compliant {
public:
	count_compliant(Enumerable &&enumerable, LogicalPredicate &&logicalPredicate) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	operator unsigned() const {
		unsigned i = 0;
		for(const auto &element : m_enumerable) {
			if(m_logicalPredicate(element))
				++i;
		}
		return i;
	}

	void
	swap(count_compliant &other){
		std::swap(m_enumerable, other.m_enumerable);
		std::swap(m_logicalPredicate, other.m_logicalPredicate);
	}

private:
	Enumerable m_enumerable;
	LogicalPredicate m_logicalPredicate;
};

template<class LogicalPredicate>
count_predicate_holder<LogicalPredicate>
count(LogicalPredicate &&logicalPredicate){
	return count_predicate_holder<LogicalPredicate>(std::forward<LogicalPredicate>(logicalPredicate));
}

template<class Enumerable, class LogicalPredicate>
count_compliant<Enumerable, LogicalPredicate>
operator|(
	Enumerable &&enumerable,
   	count_predicate_holder<LogicalPredicate> &&holder
){
	return count_compliant<Enumerable, LogicalPredicate>(
		std::forward<Enumerable>(enumerable),
	   	std::forward<count_predicate_holder<LogicalPredicate>>(holder).m_logicalPredicate
	);
}

}
}
