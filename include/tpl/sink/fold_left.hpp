#pragma once

#include "fold_left_default.hpp"
#include "fold_left_initialized.hpp"

#include "../common/sink.hpp"
#include "../common/composite_factory.hpp"
#include "../common/apply_operator.hpp"

namespace tpl{

template<class Enumerable, class Predicate>
default_fold_left<Enumerable, Predicate>
make_fold_left(Predicate &&predicate){
	return default_fold_left<Enumerable, Predicate>(
		std::forward<Predicate>(predicate)
	);
}

template<class Predicate>
class fold_left_factory {
public:
	fold_left_factory(
		Predicate &&predicate
	) :
		m_predicate(std::forward<Predicate>(predicate)){}

	template<class Enumerable>
	sink<Enumerable, default_fold_left<Enumerable, const Predicate &>>
	create(Enumerable &&enumerable) const & {
		return make_sink(
			std::forward<Enumerable>(enumerable),
			make_fold_left<Enumerable, const Predicate &>(m_predicate)
		);
	}

	template<class Enumerable>
	sink<Enumerable, default_fold_left<Enumerable, Predicate>>
	create(Enumerable &&enumerable) && {
		return make_sink(
			std::forward<Enumerable>(enumerable),
			make_fold_left<Enumerable, Predicate>(
				std::forward<Predicate>(m_predicate)
			)
		);
	}
private:
	Predicate m_predicate;
};


template<class Predicate>
fold_left_factory<Predicate>
fold_left(Predicate &&predicate){
	return fold_left_factory<Predicate>(
		std::forward<Predicate>(predicate)
	);
}

template<
	class Enumerable,
	class Predicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
sink<Enumerable, default_fold_left<Enumerable, Predicate>>
operator|(Enumerable &&enumerable, fold_left_factory<Predicate> &&factory){
	return std::forward<fold_left_factory<Predicate>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
	class Predicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
sink<Enumerable, default_fold_left<Enumerable, Predicate>>
operator|(Enumerable &&enumerable, const fold_left_factory<Predicate> &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Enumerable, class Predicate, class InitialValue>
initialized_fold_left<Enumerable, Predicate, InitialValue>
make_initialized_fold_left(Predicate &&predicate, InitialValue &&init){
	return initialized_fold_left<Enumerable, Predicate, InitialValue>(
		std::forward<Predicate>(predicate),
		std::forward<InitialValue>(init)
	);
}

template<class Predicate, class InitialValue>
class initialized_fold_left_factory {
public:
	initialized_fold_left_factory(
		Predicate &&predicate,
		InitialValue &&initialValue
	) :
		m_predicate(std::forward<Predicate>(predicate)),
		m_initialValue(std::forward<InitialValue>(initialValue)){}


	template<class Enumerable>
	sink<Enumerable, initialized_fold_left<Enumerable, const Predicate &, const InitialValue &>>
	create(Enumerable &&enumerable) const & {
		return make_sink(
			std::forward<Enumerable>(enumerable),
			make_initialized_fold_left<Enumerable, const Predicate &, const InitialValue &>(
				m_predicate,
				m_initialValue
			)
		);
	}

	template<class Enumerable>
	sink<Enumerable, initialized_fold_left<Enumerable, Predicate, InitialValue>>
	create(Enumerable &&enumerable) && {
		return make_sink(
			std::forward<Enumerable>(enumerable),
			make_initialized_fold_left<Enumerable, Predicate, InitialValue>(
				std::forward<Predicate>(m_predicate),
				std::forward<InitialValue>(m_initialValue)
			)
		);
	}
private:
	Predicate m_predicate;
	InitialValue m_initialValue;
};

template<class Predicate, class InitialValue>
initialized_fold_left_factory<Predicate, InitialValue>
fold_left(Predicate &&predicate, InitialValue &&initialValue){
	return initialized_fold_left_factory<Predicate, InitialValue>(
		std::forward<Predicate>(predicate),
		std::forward<InitialValue>(initialValue)
	);
}

}
