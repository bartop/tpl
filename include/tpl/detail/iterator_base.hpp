
#pragma once

namespace tpl{
namespace detail{

template<class Derived>
class recurring_template_pattern_base{
protected:
	const Derived &derived_this() const noexcept {
		return static_cast<const Derived &>(*this);
	}

	Derived &derived_this() noexcept {
		return static_cast<Derived &>(*this);
	}

	~recurring_template_pattern_base() noexcept = default;
};

template<class Derived>
class input_iterator_base : public recurring_template_pattern_base<Derived> {
public:
	#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Weffc++"
	#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Weffc++"
	#endif

	Derived operator++(int) {
		auto temp = this->derived_this();
		++(this->derived_this());
		return temp;
	}

	#if defined(__clang__)
	#pragma clang diagnostic pop
	#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
	#endif

	bool operator!=(const Derived &other) const {
		return !(this->derived_this() == other);
	}

protected:
	~input_iterator_base() noexcept = default;
};

template<class Derived>
class bidirectional_iterator_base : public input_iterator_base<Derived> {
public:
	#if defined(__clang__)
	#pragma clang diagnostic push
	#pragma clang diagnostic ignored "-Weffc++"
	#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Weffc++"
	#endif

	Derived operator++(int) {
		auto temp = this->derived_this();
		--(this->derived_this());
		return temp;
	}

	#if defined(__clang__)
	#pragma clang diagnostic pop
	#elif defined(__GNUC__) || defined(__GNUG__)
	#pragma GCC diagnostic pop
	#endif

protected:
	~bidirectional_iterator_base() noexcept = default;
};

}
}
