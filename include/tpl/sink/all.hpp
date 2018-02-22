
#pragma once

#include "../common/sink.hpp"
#include "../common/composite_factory.hpp"

namespace tpl{

template<class Enumerable, class LogicalPredicate>
class true_for_all {
public:
	true_for_all(LogicalPredicate &&predicate) :
		m_logicalPredicate(std::forward<LogicalPredicate>(predicate)) {}

	bool
	operator()(Enumerable &enumerable) {
		for(auto &element : enumerable) {
			if(!m_logicalPredicate(element))
				return false;
		}
		return true;
	}

	bool
	operator()(const Enumerable &enumerable) const {
		for(const auto &element : enumerable) {
			if(!m_logicalPredicate(element))
				return false;
		}
		return true;
	}

private:
	LogicalPredicate m_logicalPredicate;
};

template<class Enumerable, class Predicate>
true_for_all<Enumerable, Predicate>
make_true_for_all(Predicate &&predicate) {
	return true_for_all<Enumerable, Predicate>(
		std::forward<Predicate>(predicate)
	);
}

template<class Predicate>
class all_sink_factory {
public:
	explicit all_sink_factory(Predicate &&predicate) :
		m_predicate(std::forward<Predicate>(predicate)){}

	template<class Enumerable>
	sink<Enumerable, true_for_all<Enumerable, const Predicate &>>
	create(Enumerable &&enumerable) const & {
		return make_sink(
			std::forward<Enumerable>(enumerable),
			make_true_for_all<Enumerable>(m_predicate)
		);
	}

	template<class Enumerable>
	sink<Enumerable, true_for_all<Enumerable, Predicate>>
	create(Enumerable &&enumerable) && {
		return make_sink(
			std::forward<Enumerable>(enumerable),
			make_true_for_all<Enumerable>(
				std::forward<Predicate>(m_predicate)
			)
		);
	}

private:
	Predicate m_predicate;
};

template<class LogicalPredicate>
all_sink_factory<LogicalPredicate>
all(LogicalPredicate &&logicalPredicate) {
	return all_sink_factory<LogicalPredicate>(
		std::forward<LogicalPredicate>(logicalPredicate)
	);
}

template<
	class Enumerable,
	class Predicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
sink<Enumerable, true_for_all<Enumerable, Predicate>>
operator|(Enumerable &&enumerable, all_sink_factory<Predicate> &&factory){
	return std::forward<all_sink_factory<Predicate>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
	class Predicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
sink<Enumerable, true_for_all<Enumerable, Predicate>>
operator|(Enumerable &&enumerable, const all_sink_factory<Predicate> &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

}
