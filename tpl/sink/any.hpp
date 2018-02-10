
#pragma once

#include <iterator>

#include "../common/composite_factory.hpp"

namespace tpl{

template<class Enumerable, class LogicalPredicate>
class true_for_any {
public:
	true_for_any(Enumerable &&enumerable, LogicalPredicate &&logicalPredicate) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	operator bool() const {
		for(const auto &element : m_enumerable) {
			if(m_logicalPredicate(element))
				return true;
		}
		return false;
	}

private:
	Enumerable m_enumerable;
	LogicalPredicate m_logicalPredicate;
};

template<class Enumerable, class Predicate>
auto
make_true_for_any(Enumerable &&enumerable, Predicate &&predicate){
	return true_for_any<Enumerable, Predicate>(
		std::forward<Enumerable>(enumerable),
		std::forward<Predicate>(predicate)
	);
}

template<class LogicalPredicate>
class true_for_any_factory {
public:
	explicit true_for_any_factory(LogicalPredicate &&logicalPredicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(logicalPredicate)){}

	template<class Enumerable>
	true_for_any<Enumerable, const LogicalPredicate &>
	create(Enumerable &&enumerable) const & {
		return make_true_for_any(
			std::forward<Enumerable>(enumerable),
			m_logicalPredicate
		);
	}

	template<class Enumerable>
	true_for_any<Enumerable, LogicalPredicate>
	create(Enumerable &&enumerable) && {
		return make_true_for_any(
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

template<
	class Enumerable,
	class LogicalPredicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
true_for_any<Enumerable, const LogicalPredicate &>
operator|(Enumerable &&enumerable, const true_for_any_factory<LogicalPredicate> &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
	class LogicalPredicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
true_for_any<Enumerable, LogicalPredicate>
operator|(Enumerable &&enumerable, true_for_any_factory<LogicalPredicate> &&factory){
	return std::forward<true_for_any_factory<LogicalPredicate>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

}
