
#pragma once

#include <memory>

namespace tpl{
namespace detail{

template<class Type>
class cache {
public:
	cache() noexcept = default;
	template<class ...Args>
	cache(Args &&...args) :
		m_valid(true) {
		new (&m_cache) Type(std::forward<Args>(args)...);
	}

	bool is_valid() const{
		return m_valid;
	}

	template<class ...Args>
	void write(Args &&...args) {
		if(m_valid)
			reinterpret_cast<Type *>(&m_cache)->~Type();

		new (&m_cache) Type(std::forward<Args>(args)...);
	}

	Type &cachedValue() {
		return *reinterpret_cast<Type *>(&m_cache);
	}

	const Type &cachedValue() const {
		return *reinterpret_cast<Type *>(&m_cache);
	}

	Type *cachePointer() {
		return reinterpret_cast<Type *>(&m_cache);
	}

	const Type *cachePointer() const {
		return reinterpret_cast<Type *>(&m_cache);
	}

private:
	bool m_valid = false;
	typename std::aligned_storage<sizeof(Type), alignof(Type)>::type m_cache;
};

}
}
