
#pragma once

#include "../meta/is_enumerable.hpp"

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
	class Enumerable,
   	class SubFactory1,
	class SubFactory2,
   	class = typename std::enable_if<meta::is_enumerable<Enumerable>::value>::type
>
auto
operator|(Enumerable &&enumerable, const composite_factory<SubFactory1, SubFactory2> &factory){
	return factory.create(std::forward<Enumerable>(enumerable));
}

template<
	class Enumerable,
	class SubFactory1,
	class SubFactory2,
	class = typename std::enable_if<meta::is_enumerable<Enumerable>::value>::type
>
auto
operator|(Enumerable &&enumerable, composite_factory<SubFactory1, SubFactory2> &&factory) {
	return std::move(factory).create(std::forward<Enumerable>(enumerable));
}

template<
	class Factory,
	class SubFactory1,
	class SubFactory2
>
auto
operator|(const composite_factory<SubFactory1, SubFactory2> &factory, Factory &&other) {
	return make_composite(
		factory,
		std::forward<Factory>(other)
	);
}

template<
	class Factory,
   	class SubFactory1,
	class SubFactory2
>
auto
operator|(composite_factory<SubFactory1, SubFactory2> &&factory, Factory &&other){
	return make_composite(
		std::forward<composite_factory<SubFactory1, SubFactory2>>(factory),
	   	std::forward<Factory>(other)
	);
}

}
