
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

template<
	class Factory1,
	class Factory2,
   	class = typename std::enable_if<!meta::is_enumerable<Factory1>::value>::type
>
auto
operator|(Factory1 &&factory1, Factory2 &&factory2){
	return composite_factory<Factory1, Factory2>(
		std::forward<Factory1>(factory1),
		std::forward<Factory2>(factory2)
	);
}

}
