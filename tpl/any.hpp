
#pragma once

#include <iterator>

namespace tpl{

namespace detail{

template<class LogicalPredicate>
struct true_for_any_predicate_holder {
	explicit true_for_any_predicate_holder(LogicalPredicate logicalPredicate) :
		m_logicalPredicate(std::move(logicalPredicate)){}

	LogicalPredicate m_logicalPredicate;
};

}

template<class Container, class LogicalPredicate>
class true_for_any {
public:
	true_for_any(Container &&container, LogicalPredicate logicalPredicate) :
		m_container(std::forward<Container>(container)),
		m_logicalPredicate(std::move(logicalPredicate)){}

	true_for_any &
	operator=(true_for_any otherAny) {
		this->swap(otherAny);
		return *this;
	}

	operator bool() const {
		for(const auto &element : m_container) {
			if(m_logicalPredicate(element))
				return true;
		}
		return false;
	}

	void
	swap(true_for_any &other){
		std::swap(m_container, other.m_container);
		std::swap(m_logicalPredicate, other.m_logicalPredicate);
	}

private:
	Container m_container;
	LogicalPredicate m_logicalPredicate;
};

template<class LogicalPredicate>
detail::true_for_any_predicate_holder<LogicalPredicate>
any(LogicalPredicate logicalPredicate){
	return detail::true_for_any_predicate_holder<LogicalPredicate>(std::move(logicalPredicate));
}

template<class Container, class LogicalPredicate>
true_for_any<Container, LogicalPredicate>
operator|(
	Container &&container,
   	detail::true_for_any_predicate_holder<LogicalPredicate> holder
){
	return true_for_any<Container, LogicalPredicate>(
		std::forward<Container>(container),
		std::move(holder.m_logicalPredicate)
	);
}

}
