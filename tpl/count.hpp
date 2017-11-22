
#pragma once

#include <iterator>

namespace tpl{

namespace detail{

template<class LogicalPredicate>
struct count_predicate_holder {
	count_predicate_holder(LogicalPredicate logicalPredicate) :
		m_logicalPredicate(std::move(logicalPredicate)){}

	LogicalPredicate m_logicalPredicate;
};

}

template<class Container, class LogicalPredicate>
class count_compliant {
public:
	count_compliant(Container &&container, LogicalPredicate logicalPredicate) :
		m_container(std::forward<Container>(container)),
		m_logicalPredicate(std::move(logicalPredicate)){}

	operator unsigned() const {
		unsigned i = 0;
		for(const auto &element : m_container) {
			if(m_logicalPredicate(element))
				++i;
		}
		return i;
	}

	void
	swap(count_compliant &other){
		std::swap(m_container, other.m_container);
		std::swap(m_logicalPredicate, other.m_logicalPredicate);
	}

private:
	Container m_container;
	LogicalPredicate m_logicalPredicate;
};

template<class LogicalPredicate>
detail::count_predicate_holder<LogicalPredicate>
count(LogicalPredicate logicalPredicate){
	return detail::count_predicate_holder<LogicalPredicate>(std::move(logicalPredicate));
}

template<class Container, class LogicalPredicate>
count_compliant<Container, LogicalPredicate>
operator|(
	Container &&container,
   	detail::count_predicate_holder<LogicalPredicate> holder
){
	return count_compliant<Container, LogicalPredicate>(
		std::forward<Container>(container),
	   	std::move(holder.m_logicalPredicate)
	);
}

}
