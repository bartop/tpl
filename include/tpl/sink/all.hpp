
#pragma once

#include "../common/composite_factory.hpp"

#include <iterator>

namespace tpl{

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
private:
	Enumerable m_enumerable;
	LogicalPredicate m_logicalPredicate;
};


template<class Enumerable, class Predicate>
true_for_all<Enumerable, Predicate>
make_true_for_all(Enumerable &&enumerable, Predicate &&predicate){
	return true_for_all<Enumerable, Predicate>(
		std::forward<Enumerable>(enumerable),
		std::forward<Predicate>(predicate)
	);
}


template<class LogicalPredicate>
class true_for_all_factory {
public:
	explicit true_for_all_factory(LogicalPredicate &&logicalPredicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	template<class Enumerable>
	true_for_all<Enumerable, const LogicalPredicate &>
	create(Enumerable &&enumerable) const & {
		return make_true_for_all(
			std::forward<Enumerable>(enumerable),
			m_logicalPredicate
		);
	}

	template<class Enumerable>
	true_for_all<Enumerable, LogicalPredicate>
	create(Enumerable &&enumerable) && {
		return make_true_for_all(
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

template<
	class Enumerable,
	class LogicalPredicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
true_for_all<Enumerable, const LogicalPredicate &>
operator|(Enumerable &&enumerable, const true_for_all_factory<LogicalPredicate> &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
	class LogicalPredicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
true_for_all<Enumerable, LogicalPredicate>
operator|(Enumerable &&enumerable, true_for_all_factory<LogicalPredicate> &&factory){
	return std::forward<true_for_all_factory<LogicalPredicate>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

}