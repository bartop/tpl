
#include "../common/composite_factory.hpp"

#ifndef _MSC_VER
#include "fold_left_initialized.hpp"
#include "fold_left_default.hpp"
#else
#include "fold_left_initialized_msvc.hpp"
#include "fold_left_default_msvc.hpp"
#endif

namespace tpl{

template<class Enumerable, class Predicate>
auto
make_fold_left(Enumerable &&enumerable, Predicate &&predicate){
	return default_fold_left<Enumerable, Predicate>(
		std::forward<Enumerable>(enumerable),
		std::forward<Predicate>(predicate)
	);
}

template<class BinaryPredicate>
class fold_left_factory {
public:
	fold_left_factory(
		BinaryPredicate &&predicate
	) :
		m_predicate(std::forward<BinaryPredicate>(predicate)){}

	template<class Enumerable>
	default_fold_left<Enumerable, BinaryPredicate>
	create(Enumerable &&enumerable) const & {
		return make_fold_left(
			std::forward<Enumerable>(enumerable),
			m_predicate
		);
	}

	template<class Enumerable>
	default_fold_left<Enumerable, BinaryPredicate>
	create(Enumerable &&enumerable) && {
		return make_fold_left(
			std::forward<Enumerable>(enumerable),
			std::forward<BinaryPredicate>(m_predicate)
		);
	}
private:
	BinaryPredicate m_predicate;
};


template<class BinaryPredicate>
fold_left_factory<BinaryPredicate>
fold_left(BinaryPredicate &&predicate){
	return fold_left_factory<BinaryPredicate>(
		std::forward<BinaryPredicate>(predicate)
	);
}

template<
	class Enumerable,
	class BinaryPredicate,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
auto
operator|(Enumerable &&enumerable, fold_left_factory<BinaryPredicate> &&factory){
	return std::forward<fold_left_factory<BinaryPredicate>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Factory, class BinaryPredicate>
auto
operator|(fold_left_factory<BinaryPredicate> &&factory, Factory &&other){
	return make_composite(
		std::forward<fold_left_factory<BinaryPredicate>>(factory),
	   	std::forward<Factory>(other)
	);
}

template<class Enumerable, class Predicate, class InitialValue>
auto
make_initialized_fold_left(Enumerable &&enumerable, Predicate &&predicate, InitialValue &&init){
	return initialized_fold_left<Enumerable, Predicate, InitialValue>(
		std::forward<Enumerable>(enumerable),
		std::forward<Predicate>(predicate),
		std::forward<InitialValue>(init)
	);
}

template<class BinaryPredicate, class InitialValue>
class initialized_fold_left_factory {
public:
	initialized_fold_left_factory(
		BinaryPredicate &&predicate,
		InitialValue &&initialValue
	) :
		m_predicate(std::forward<BinaryPredicate>(predicate)),
		m_initialValue(std::forward<InitialValue>(initialValue)){}


	template<class Enumerable>
	auto
	create(Enumerable &&enumerable) const & {
		return make_initialized_fold_left(
			std::forward<Enumerable>(enumerable),
			m_predicate,
			m_initialValue
		);
	}

	template<class Enumerable>
	auto
	create(Enumerable &&enumerable) && {
		return make_initialized_fold_left(
			std::forward<Enumerable>(enumerable),
			std::forward<BinaryPredicate>(m_predicate),
			std::forward<InitialValue>(m_initialValue)
		);
	}
private:
	BinaryPredicate m_predicate;
	InitialValue m_initialValue;
};

template<class BinaryPredicate, class InitialValue>
initialized_fold_left_factory<BinaryPredicate, InitialValue>
fold_left(BinaryPredicate &&predicate, InitialValue &&initialValue){
	return initialized_fold_left_factory<BinaryPredicate, InitialValue>(
		std::forward<BinaryPredicate>(predicate),
		std::forward<InitialValue>(initialValue)
	);
}

template<
	class Enumerable,
	class BinaryPredicate,
	class InitialValue,
   	class = typename std::enable_if<meta::is_enumerable<std::decay_t<Enumerable>>::value>::type
>
auto
operator|(Enumerable &&enumerable, initialized_fold_left_factory<BinaryPredicate, InitialValue> &&factory){
	return std::forward<initialized_fold_left_factory<BinaryPredicate, InitialValue>>(factory).create(
		std::forward<Enumerable>(enumerable)
	);
}

template<class Factory, class BinaryPredicate, class InitialValue>
auto
operator|(initialized_fold_left_factory<BinaryPredicate, InitialValue> &&factory, Factory &&other){
	return make_composite(
		std::forward<initialized_fold_left_factory<BinaryPredicate, InitialValue>>(factory),
	   	std::forward<Factory>(other)
	);
}

}
