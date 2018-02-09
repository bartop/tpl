
#include "../common/composite_factory.hpp"

#include "fold_left_default.hpp"
#include "fold_left_initialized.hpp"

namespace tpl{

template<class Enumerable, class Predicate>
default_fold_left<Enumerable, Predicate>
make_fold_left(Enumerable &&enumerable, Predicate &&predicate){
	return default_fold_left<Enumerable, Predicate>(
		std::forward<Enumerable>(enumerable),
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
	default_fold_left<Enumerable, const Predicate &>
	create(Enumerable &&enumerable) const & {
		return make_fold_left(
			std::forward<Enumerable>(enumerable),
			m_predicate
		);
	}

	template<class Enumerable>
	default_fold_left<Enumerable, Predicate>
	create(Enumerable &&enumerable) && {
		return make_fold_left(
			std::forward<Enumerable>(enumerable),
			std::forward<Predicate>(m_predicate)
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
default_fold_left<Enumerable, Predicate>
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
default_fold_left<Enumerable, const Predicate &>
operator|(Enumerable &&enumerable, const fold_left_factory<Predicate> &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Enumerable, class Predicate, class InitialValue>
initialized_fold_left<Enumerable, Predicate, InitialValue>
make_initialized_fold_left(Enumerable &&enumerable, Predicate &&predicate, InitialValue &&init){
	return initialized_fold_left<Enumerable, Predicate, InitialValue>(
		std::forward<Enumerable>(enumerable),
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
	initialized_fold_left<Enumerable, const Predicate &, const InitialValue &>
	create(Enumerable &&enumerable) const & {
		return make_initialized_fold_left(
			std::forward<Enumerable>(enumerable),
			m_predicate,
			m_initialValue
		);
	}

	template<class Enumerable>
	initialized_fold_left<Enumerable, Predicate, InitialValue>
	create(Enumerable &&enumerable) && {
		return make_initialized_fold_left(
			std::forward<Enumerable>(enumerable),
			std::forward<Predicate>(m_predicate),
			std::forward<InitialValue>(m_initialValue)
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

template<
	class Enumerable,
	class Predicate,
	class InitialValue,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
initialized_fold_left<Enumerable, Predicate, InitialValue>
operator|(Enumerable &&enumerable, initialized_fold_left_factory<Predicate, InitialValue> &&factory){
	return std::forward<initialized_fold_left_factory<Predicate, InitialValue>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<
	class Enumerable,
	class Predicate,
	class InitialValue,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
initialized_fold_left<Enumerable, const Predicate &, const InitialValue &>
operator|(Enumerable &&enumerable, const initialized_fold_left_factory<Predicate, InitialValue> &factory){
	return factory.create(
		std::forward<Enumerable>(enumerable)
	);
}

}
