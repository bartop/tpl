
#pragma once

#include "apply_operator.hpp"

#include "../meta/is_enumerable.hpp"
#include "../meta/is_factory.hpp"

#include <type_traits>

namespace tpl{

template<class Factory1, class Factory2>
class composite_factory {
public:
	composite_factory(Factory1 &&factory1, Factory2 &&factory2)
		:
		   	m_factory1(std::forward<Factory1>(factory1)),
			m_factory2(std::forward<Factory2>(factory2)){}

	template<class Enumerable>
	auto
	create(Enumerable &&enumerable) const & {
		return m_factory2.create(
			m_factory1.create(
				std::forward<Enumerable>(enumerable)
			)
		);
	}

	template<class Enumerable>
	auto
	create(Enumerable &&enumerable) && {
		return std::forward<Factory2>(m_factory2).create(
			std::forward<Factory1>(m_factory1).create(
				std::forward<Enumerable>(enumerable)
			)
		);
	}
private:
	Factory1 m_factory1;
	Factory2 m_factory2;
};


template<class Factory1, class Factory2>
auto
make_composite(Factory1 &&factory1, Factory2 &&factory2){
	return composite_factory<Factory1, Factory2>(
		std::forward<Factory1>(factory1),
		std::forward<Factory2>(factory2)
	);
}

template<class Factory, class ...Factories>
auto
compose(Factory &&factory, Factories &&...factories){
	return make_composite(
		std::forward<Factory>(factory),
		compose(std::forward<Factories>(factories)...)
	);
}

template<class Factory>
decltype(auto)
compose(Factory &&factory){
	return std::forward<Factory>(factory);
}

template<
	class Factory1,
	class Factory2,
	class = typename std::enable_if<
		meta::is_factory<Factory1>::value && meta::is_factory<Factory2>::value
	>::type
>
composite_factory<Factory1, Factory2>
operator|(Factory1 &&factory1, Factory2 &&factory2){
	return make_composite(
		std::forward<Factory1>(factory1),
		std::forward<Factory2>(factory2)
	);
}

}
