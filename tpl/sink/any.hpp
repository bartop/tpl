
#pragma once

#include <iterator>

#include "../common/composition_operator.hpp"

namespace tpl{

template<class Enumerable, class LogicalPredicate>
class true_for_any {
public:
	true_for_any(Enumerable &&enumerable, LogicalPredicate &&logicalPredicate) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

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
class true_for_any_factory {
public:
	explicit true_for_any_factory(LogicalPredicate &&logicalPredicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	template<class Enumerable>
	true_for_any<Enumerable, LogicalPredicate>
	create(Enumerable &&enumerable) const & {
		return true_for_any<Enumerable, const LogicalPredicate &>(
			std::forward<Enumerable>(enumerable),
			m_logicalPredicate
		);
	}

	template<class Enumerable>
	true_for_any<Enumerable, LogicalPredicate>
	create(Enumerable &&enumerable) && {
		return true_for_any<Enumerable, LogicalPredicate>(
			std::forward<Enumerable>(enumerable),
			std::forward<LogicalPredicate>(m_logicalPredicate)
		);
	}
private:
	LogicalPredicate m_logicalPredicate;
};


template<class LogicalPredicate>
true_for_any_factory<LogicalPredicate>
any(LogicalPredicate &&logicalPredicate){
	return true_for_any_factory<LogicalPredicate>(
		std::forward<LogicalPredicate>(logicalPredicate)
	);
}

}
