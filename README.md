# tpl: Template library for piping operations 

## Overview

Tpl is C++ header-only library designed to simplify basic collections/containers operations. It strongly relies on C++ templates allowing the compiler to do heavy-lifting and generate efficient code.

Using ideas from functional languages and some more modern imperative languages (like [Kotlin](https://kotlinlang.org/)) combined with power of C++ gives many benefits:
- value semantics
- type safety
- speed
- lazy evaluation - nothing is ever computed if you don't ask for result
- syntax simplification via operator overloads
- readability and elegance
- ~~meaningful error messaeges~~ ```/include/c++/7.2.0/bits/atomic_base.h|74 col 3 error| note: candidate: constexpr std::memory_order std::operator|(std::memory_order, std::__memory_order_modifier)^M [cpp/gcc]```

## Build statuses

| Compiler | GCC5 | GCC6 | GCC7 | VS 2017 |
| -------- | ---- | ---- | ---- | ------- |
| Build status | [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/bartop/tpl/branches/master/1)](https://travis-ci.org/bartop/tpl) | [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/bartop/tpl/branches/master/2)](https://travis-ci.org/bartop/tpl) | [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/bartop/tpl/branches/master/3)](https://travis-ci.org/bartop/tpl) | [![Build status](https://ci.appveyor.com/api/projects/status/g52jere64wcb5lw6?svg=true)](https://ci.appveyor.com/project/bartop/tpl)



# Getting started

## Requirements
As tpl is header-only it does not require installation - just clone this repository and use the headers in tpl directory. Currently the tests are made under C++14 compliant compiler. Support for older standards is in plans, I will gladly accept any help - feel free send me an [e-mail](mailto:bartoszmiera@gmail.com).

## Sample code

### Filter collection

```C++
#include <tpl/filtered.hpp>
#include <iostream>

using namespace tpl;
using namespace std;

int main(){
  const auto collection = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const auto filtered = collection | filter([](const auto &i){ return i > 5 });
  for(const auto i : filtered)
    cout << i;
  //output will be 6, 7, 8, 9, 10
  return 0;
}
```

### Transform collection
```C++
#include <tpl/transformed.hpp>
#include <iostream>

using namespace tpl;
using namespace std;

int main(){
  const auto collection = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const auto transformed = collection | transform([](const auto &i){ return i > 5; });//transform to booleans
  for(const auto i : transformed)
    cout << i;
  //output will be 0, 0, 0, 0, 0, 1, 1, 1, 1, 1
  return 0;
}
```

### Sort collection
```C++
#include <tpl/sorted.hpp>
#include <iostream>

using namespace tpl;
using namespace std;

int main(){
  const auto collection = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const auto sorted = collection | sort([](const auto &i, const auto &j){ return i > j; });
  for(const auto i : sorted)
    cout << i;
  //output will be 10, 9, 8, 7, 6, 5, 4, 3, 2, 1
  return 0;
}
```
### Extract keys of the map 
```C++
#include <tpl/sorted.hpp>
#include <iostream>
#include <map>

using namespace tpl;
using namespace std;

int main(){
  const map<int, int> map = { {1, 2}, {3, 4}, {5, 6}, {7, 8}, {9, 10} };
  const auto keys = map | keys;
  for(const auto i : keys)
    cout << i;
  //output will be 1, 3, 5, 7, 9
  return 0;
}
```
### Flatten the collection of collections
```C++
#include <tpl/sorted.hpp>
#include <iostream>
#include <vector>

using namespace tpl;
using namespace std;

int main(){
  const vector<vector<int>> vec = { {1, 2, 3, 4}, {5, 6}, {7, 8, 9} , {10} };
  const auto flat = vec | flatten;
  for(const auto i : flat)
    cout << i;
  //output will be 1, 2, 3, 4, 5, 6, 7, 8, 9, 10
  return 0;
}
```


### More complex example
The whole power of this solutin is ease of extension. For example, if you want to filter, sort and transform you can do it just like that:
```C++
const auto afterComplexOperation = map
  | keys
  | filter([](const auto &i){ /* your filter here */ }
  | sort([](const auto &i, const auto &j){ /* your sort predicate */}
  | transform([](const auto &i){ /* apply your transformation */})
  | flatten;
```
