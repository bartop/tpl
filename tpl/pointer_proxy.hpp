
#include <utility>

namespace detail{

template<class T>
class pointer_proxy {
public:
	pointer_proxy(T &&value) : m_value(std::forward<T>(value)) {}
	T *operator ->() const { return &m_value; }

private:
	T m_value;
};

template<class T>
pointer_proxy<T>
make_pointer_proxy(T &&value){
	return pointer_proxy<T>(std::forward<T>(value));
}

}
