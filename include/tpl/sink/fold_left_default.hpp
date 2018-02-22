
#pragma once

#include "../meta/enumerable_traits.hpp"

#include <iterator>
#include <numeric>

namespace tpl{

template<
	class Enumerable,
	class BinaryPredicate,
   	class ConversionType = decltype(
		std::declval<BinaryPredicate>()(
		   	std::declval<typename meta::enumerable_traits<Enumerable>::value_type>(),
		   	std::declval<typename meta::enumerable_traits<Enumerable>::value_type>()
		)
	)
>
class default_fold_left {
public:
	default_fold_left(
		BinaryPredicate &&predicate
	) :
		m_predicate(std::forward<BinaryPredicate>(predicate)) {}

	using conversion_type = ConversionType;

	conversion_type
	operator()(Enumerable &enumerable) {
		return std::accumulate(
			std::begin(enumerable),
		   	std::end(enumerable),
		   	typename meta::enumerable_traits<Enumerable>::value_type(),
			m_predicate
		);
	}

	conversion_type
	operator()(const Enumerable &enumerable) const{
		return std::accumulate(
			std::begin(enumerable),
		   	std::end(enumerable),
		   	typename meta::enumerable_traits<Enumerable>::value_type(),
			m_predicate
		);
	}

private:
	BinaryPredicate m_predicate;
};

}
