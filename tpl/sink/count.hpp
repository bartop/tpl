
#pragma once

#include <iterator>

namespace tpl{
namespace sink{

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
class count_factory {
public:
	explicit count_factory(LogicalPredicate &&logicalPredicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	template<class Enumerable>
	count_compliant<Enumerable, LogicalPredicate>
	create(Enumerable &&enumerable) const & {
		return count_compliant<Enumerable, LogicalPredicate>(
			std::forward<Enumerable>(enumerable),
			m_logicalPredicate
		);
	}

	template<class Enumerable>
	count_compliant<Enumerable, LogicalPredicate>
	create(Enumerable &&enumerable) && {
		return count_compliant<Enumerable, LogicalPredicate>(
			std::forward<Enumerable>(enumerable),
			std::forward<LogicalPredicate>(m_logicalPredicate)
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

template<class Enumerable, class LogicalPredicate>
count_compliant<Enumerable, LogicalPredicate>
operator|(
	Enumerable &&enumerable,
   	count_factory<LogicalPredicate> &&factory
){
	return std::forward<count_factory<LogicalPredicate>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

}
}
