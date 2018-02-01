
#include "../common/composition_operator.hpp"

#ifndef _MSC_VER
#include "fold_left_initialized.hpp"
#include "fold_left_default.hpp"
#else
#include "fold_left_initialized_msvc.hpp"
#include "fold_left_default_msvc.hpp"
#endif

namespace tpl{

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
		return default_fold_left<Enumerable, const BinaryPredicate &>(
			std::forward<Enumerable>(enumerable),
			m_predicate
		);
	}

	template<class Enumerable>
	default_fold_left<Enumerable, BinaryPredicate>
	create(Enumerable &&enumerable) && {
		return default_fold_left<Enumerable, BinaryPredicate>(
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
	initialized_fold_left<Enumerable, BinaryPredicate, InitialValue>
	create(Enumerable &&enumerable) const & {
		return initialized_fold_left<Enumerable, const BinaryPredicate &, const InitialValue &>(
			std::forward<Enumerable>(enumerable),
			std::forward<BinaryPredicate>(m_predicate),
			m_initialValue
		);
	}

	template<class Enumerable>
	initialized_fold_left<Enumerable, BinaryPredicate, InitialValue>
	create(Enumerable &&enumerable) && {
		return initialized_fold_left<Enumerable, BinaryPredicate, InitialValue>(
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

}
