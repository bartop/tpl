
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

template<class Enumerable, class LogicalPredicate>
class true_for_any {
public:
	true_for_any(Enumerable &&enumerable, LogicalPredicate logicalPredicate) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_logicalPredicate(std::move(logicalPredicate)){}

	true_for_any &
	operator=(true_for_any otherAny) {
		this->swap(otherAny);
		return *this;
	}

	operator bool() const {
		for(const auto &element : m_enumerable) {
			if(m_logicalPredicate(element))
				return true;
		}
		return false;
	}

	void
	swap(true_for_any &other){
		std::swap(m_enumerable, other.m_enumerable);
		std::swap(m_logicalPredicate, other.m_logicalPredicate);
	}

private:
	Enumerable m_enumerable;
	LogicalPredicate m_logicalPredicate;
};

template<class LogicalPredicate>
detail::true_for_any_predicate_holder<LogicalPredicate>
any(LogicalPredicate logicalPredicate){
	return detail::true_for_any_predicate_holder<LogicalPredicate>(std::move(logicalPredicate));
}

template<class Enumerable, class LogicalPredicate>
true_for_any<Enumerable, LogicalPredicate>
operator|(
	Enumerable &&enumerable,
   	detail::true_for_any_predicate_holder<LogicalPredicate> holder
){
	return true_for_any<Enumerable, LogicalPredicate>(
		std::forward<Enumerable>(enumerable),
		std::move(holder.m_logicalPredicate)
	);
}

}
