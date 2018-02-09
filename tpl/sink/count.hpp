
#pragma once

#include <iterator>

#include "../common/composite_factory.hpp"

namespace tpl{

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

template<class Enumerable, class Predicate>
count_compliant<Enumerable, Predicate>
make_count(Enumerable &&enumerable, Predicate &&predicate){
	return count_compliant<Enumerable, Predicate>(
		std::forward<Enumerable>(enumerable),
		std::forward<Predicate>(predicate)
	);
}

template<class LogicalPredicate>
class count_factory {
public:
	explicit count_factory(LogicalPredicate &&logicalPredicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	template<class Enumerable>
	count_compliant<Enumerable, const LogicalPredicate &>
	create(Enumerable &&enumerable) const & {
		return make_count(
			std::forward<Enumerable>(enumerable),
			m_logicalPredicate
		);
	}

	template<class Enumerable>
	count_compliant<Enumerable, LogicalPredicate>
	create(Enumerable &&enumerable) && {
		return make_count(
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

template<
	class Enumerable,
	class LogicalPredicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
	count_compliant<Enumerable, LogicalPredicate>
operator|(Enumerable &&enumerable, count_factory<LogicalPredicate> &&factory){
	return std::forward<count_factory<LogicalPredicate>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
	class LogicalPredicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
count_compliant<Enumerable, const LogicalPredicate &>
operator|(Enumerable &&enumerable, const count_factory<LogicalPredicate> &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

}
