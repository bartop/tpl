#pragma once

#include "../meta/helpers.hpp"

#include <type_traits>

namespace tpl {

template<class Derived, class Result, class ConstResult>
class implicit_sink_base {
public:
	using result_t = Result;
	using const_result_t = ConstResult;

	operator result_t() {
		return static_cast<Derived &>(*this).result();
	}

	operator const_result_t() const {
		return static_cast<const Derived &>(*this).result();
	}
};

template<class Derived, class Result, class ConstResult>
class explicit_sink_base {
public:
	using result_t = Result;
	using const_result_t = ConstResult;

	explicit operator result_t() {
		return static_cast<Derived &>(*this).result();
	}

	explicit operator const_result_t() const {
		return static_cast<const Derived &>(*this).result();
	}
};

template<
	class Enumerable,
	class Functor,
	class Result = decltype(std::declval<Functor>()(std::declval<Enumerable>())),
	class ConstResult = decltype(std::declval<const Functor>()(std::declval<Enumerable>()))
>
class sink :
   	public std::conditional_t<
		std::is_integral<Result>::value,
	   	implicit_sink_base<sink<Enumerable, Functor, Result, ConstResult>, Result, ConstResult>,
		explicit_sink_base<sink<Enumerable, Functor, Result, ConstResult>, Result, ConstResult>
	>{
public:
	using result_t = Result;
	using const_result_t = ConstResult;

	sink(Enumerable &&enumerable, Functor &&functor) :
		m_enumerable(std::forward<Enumerable>(enumerable)),
		m_functor(std::forward<Functor>(functor)){}

	result_t
	result() {
		return m_functor(m_enumerable);
	}

	const_result_t
	result() const {
		return m_functor(m_enumerable);
	}

private:
	Enumerable m_enumerable;
	Functor m_functor;
};


template<class Enumerable, class Functor>
sink<Enumerable, Functor>
make_sink(Enumerable &&enumerable, Functor &&functor){
	return sink<Enumerable, Functor>(
		std::forward<Enumerable>(enumerable),
		std::forward<Functor>(functor)
	);
}


}
