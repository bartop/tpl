
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
class equality_comparable : public recurring_template_pattern_base<Derived> {
public:
	bool operator!=(const Derived &other) const {
		return !(this->derived_this() == other);
	}

protected:
	~equality_comparable() noexcept = default;

};

template<class Derived>
class input_iterator_base : public equality_comparable<Derived> {
public:
	Derived operator++(int) {
		auto temp = this->derived_this();
		++(this->derived_this());
		return temp;
	}
protected:
	~input_iterator_base() noexcept = default;
};

template<class Derived>
class bidirectional_iterator_base : public input_iterator_base<Derived> {
public:
	Derived operator--(int) {
		auto temp = this->derived_this();
		--(this->derived_this());
		return temp;
	}

protected:
	~bidirectional_iterator_base() noexcept = default;
};

}
}
