# tpl: Template library for piping operations

Tpl is C++ header-only library designed to simplify basic collections/containers operations. It strongly relies on C++ templates allowing the compiler to do heavy-lifting and generate efficient code.

Using ideas from functional languages and some more modern imperative languages (like [Kotlin](https://kotlinlang.org/)) combined with power of C++ gives many benefits:
- value semantics
- type safety
- speed
- syntax simplification via operator overloads
- readability and elegance
- ~~meaningful error messaeges~~ ```/include/c++/7.2.0/bits/atomic_base.h|74 col 3 error| note: candidate: constexpr std::memory_order std::operator|(std::memory_order, std::__memory_order_modifier)^M [cpp/gcc]```

