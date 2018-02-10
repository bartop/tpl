
#pragma once

namespace tpl{
namespace detail{

template<class Derived>
class iterator_base {
public:

	#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Weffc++"
	#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Weffc++"
	#endif

	Derived operator++(int) {
		auto temp = derivedThis();
		++derivedThis();
		return temp;
	}

	#if defined(__clang__)
	#pragma clang diagnostic pop
	#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
	#endif

	bool operator!=(const Derived &other) const {
		return !(derivedThis() == other);
	}

protected:
	~iterator_base() noexcept = default;

private:
	const Derived &derivedThis() const {
		return static_cast<const Derived &>(*this);
	}

	Derived &derivedThis() {
		return static_cast<Derived &>(*this);
	}
};

}
}
