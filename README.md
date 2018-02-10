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
- [pure functions](https://en.wikipedia.org/wiki/Pure_function) that are easy to reason about

## Build statuses

### Linux gcc 4.9+

| gcc 4.9 | gcc 5 | gcc 6 | gcc 7 |
| ------- | ----- | ----- | ----- |
| [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/bartop/tpl/branches/master/4)](https://travis-ci.org/bartop/tpl) | [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/bartop/tpl/branches/master/1)](https://travis-ci.org/bartop/tpl) | [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/bartop/tpl/branches/master/2)](https://travis-ci.org/bartop/tpl) | [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/bartop/tpl/branches/master/3)](https://travis-ci.org/bartop/tpl) 

### Linux clang 3.9+

| clang 3.9 | clang 4 | clang 5 |
| --------- | ------- | ------- | 
| [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/bartop/tpl/branches/master/5)](https://travis-ci.org/bartop/tpl) | [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/bartop/tpl/branches/master/6)](https://travis-ci.org/bartop/tpl) | [![Build Status](https://travis-matrix-badges.herokuapp.com/repos/bartop/tpl/branches/master/7)](https://travis-ci.org/bartop/tpl) |

### Visual Studio

| Visual Studio 2015/2017 |
| ------------------ |
| [![Build status](https://ci.appveyor.com/api/projects/status/g52jere64wcb5lw6?svg=true)](https://ci.appveyor.com/project/bartop/tpl)| 

# Getting started
## Requirements
For library:
- C++14 compliant compiler (compilers that work for sure are listed above)

Catch framework is shipped with the library so for test You need:
- cmake tool

## Installation
tpl is a header only library. The only things You have to do are:
- clone this repository `git clone https://github.com/bartop/tpl.git`
- (optionally) setup additional include path in Your project to `tpl/include/`

If you want to build the tests You have to do the following:
- clone this repository `git clone https://github.com/bartop/tpl.git`
- enter main directory of the repository `cd tpl`
- make directory for build and enter it `mkdir build && cd build`
- generate files for desired build system using `cmake .. -G <name_of_cmake_generator>`

## Sample code

### Filter collection
```C++
#include <tpl.hpp>
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
#include <tpl.hpp>
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

### Pepare operations for further use
```C++
#include <tpl.hpp>
#include <iostream>

using namespace tpl;
using namespace std;

int main(){
  const auto collection1 = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const auto collection2 = { 1, 11 };
  const auto operation =
      filter([](const auto &i){ return i > 5 }) | 
      transform([](const auto &i){ return i + 1; });
  const auto result1 = collection1 | operation;
  const auto result2 = collection2 | operation;
  for(const auto i : result1)
    cout << i; //output will be 7, 8, 9, 10, 11
    
  for(const auto i : result2)
    cout << i; //output will be 12
    
  return 0;
}
```

### Extract keys of the map 
```C++
#include <tpl.hpp>
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
#include <tpl.hpp>
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

### Fold left the collection
```C++
#include <tpl.hpp>
#include <iostream>
#include <vector>

using namespace tpl;
using namespace std;

int main(){
  const auto collection = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
  const auto fold = vec | fold_left([](const auto &l, const auto &r) { return l + r; });
  cout << i;
  //output will be 55
  return 0;
}
```

### Generate 10 items from cyclic generator
```C++
#include <tpl.hpp>
#include <iostream>
#include <vector>

using namespace tpl;
using namespace std;

int main(){
  const auto collection = cycle(vector<int>{ 1, 2, 3 }) | take(10);//initializer list is not accepted, sadly
  for(const auto &i : collection)
    cout << i;
  //output will be 1, 2, 3, 1, 2, 3, 1, 2, 3, 1
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
