/**
 * \file
 * \brief File defining left fold operation.
 */

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

/**
 * \brief Sink performing on input sequence left fold. Left fold is also called
 *     reduce or accumulate.
 *
 * This is a sink, which means it can be used as final part of a pipeline.
 * 
 * In this version of sink default value of input sequence value_type is 
 * taken as initial fold value.
 *
 * \tparam Predicate Type of function-like object which will be used for fold.
 *     It must take two parameters constructible from input sequence
 *     value_type and return value convertible to its first argument.
 *
 * \param predicate Function which will be used for fold.
 *
 *  **Example**
 *
 *     std::vector<int> v = {1, 2, 3, 4, 5};
 *     const int out = v | tpl::fold_left([](auto j, auto i){ return i + j; });
 *     // out == 15
 */
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

/**
 * \brief Sink performing on input sequence left fold. Left fold is also called
 *     reduce or accumulate.
 *
 * This is a sink, which means it can be used as final part of a pipeline.
 * 
 * In this version of sink passed argument is taken as initial fold value.
 *
 * \tparam Predicate Type of function-like object which will be used for fold.
 *     It must take two parameters constructible from input sequence
 *     value_type and return value convertible to its first argument.
 * \tparam InitialValue Type of initial value of the fold. Must be convertible
 *     to type of first argument of the Predicate.
 *
 * \param predicate Function which will be used for fold.
 * \param initialValue Initial value of the fold operation.
 *
 *  **Example**
 *
 *     std::vector<int> v = {1, 2, 3, 4, 5};
 *     const int out = v | tpl::fold_left([](auto j, auto i){ return i + j; }, 5);
 *     // out == 20
 */
template<class Predicate, class InitialValue>
initialized_fold_left_factory<Predicate, InitialValue>
fold_left(Predicate &&predicate, InitialValue &&initialValue){
	return initialized_fold_left_factory<Predicate, InitialValue>(
		std::forward<Predicate>(predicate),
		std::forward<InitialValue>(initialValue)
	);
}

}
