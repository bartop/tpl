
#pragma once

#include <iterator>

namespace tpl{

namespace detail{

template<class LogicalPredicate>
struct true_for_all_predicate_holder {
	explicit true_for_all_predicate_holder(LogicalPredicate logicalPredicate) :
		m_logicalPredicate(std::move(logicalPredicate)){}

	LogicalPredicate m_logicalPredicate;
};

}

template<class Enumerable, class LogicalPredicate>
class true_for_all {
public:
	true_for_all(Enumerable &&enumerable, LogicalPredicate logicalPredicate) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_logicalPredicate(std::move(logicalPredicate)){}

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
detail::true_for_all_predicate_holder<LogicalPredicate>
all(LogicalPredicate logicalPredicate){
	return detail::true_for_all_predicate_holder<LogicalPredicate>(std::move(logicalPredicate));
}

template<class Enumerable, class LogicalPredicate>
true_for_all<Enumerable, LogicalPredicate>
operator|(
	Enumerable &&enumerable,
   	detail::true_for_all_predicate_holder<LogicalPredicate> holder
){
	return true_for_all<Enumerable, LogicalPredicate>(
		std::forward<Enumerable>(enumerable),
	   	std::move(holder.m_logicalPredicate)
	);
}

}
