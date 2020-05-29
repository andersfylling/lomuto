[![Actions Status](https://github.com/TheLartians/ModernCppStarter/workflows/MacOS/badge.svg)](https://github.com/TheLartians/ModernCppStarter/actions)
[![Actions Status](https://github.com/TheLartians/ModernCppStarter/workflows/Windows/badge.svg)](https://github.com/TheLartians/ModernCppStarter/actions)
[![Actions Status](https://github.com/TheLartians/ModernCppStarter/workflows/Ubuntu/badge.svg)](https://github.com/TheLartians/ModernCppStarter/actions)
[![Actions Status](https://github.com/TheLartians/ModernCppStarter/workflows/Style/badge.svg)](https://github.com/TheLartians/ModernCppStarter/actions)
[![Actions Status](https://github.com/TheLartians/ModernCppStarter/workflows/Install/badge.svg)](https://github.com/TheLartians/ModernCppStarter/actions)
[![codecov](https://codecov.io/gh/TheLartians/ModernCppStarter/branch/master/graph/badge.svg)](https://codecov.io/gh/TheLartians/ModernCppStarter)

# Lomuto

A faster sorting option than std::sort.

This is a version of [github.com/andralex/lomuto](https://github.com/andralex/lomuto) with support for a more STL like interface. See the original blog post here [dlang.org/blog/2020/05/14/lomutos-comeback](https://dlang.org/blog/2020/05/14/lomutos-comeback/).
The basic sort function, is more or less a direct copy.

## Benchmark

```
----------------------------------------------------------------------------
Benchmark                  Time             CPU   Iterations   Label
----------------------------------------------------------------------------
BM_Sorting<false>     107707 ns       107649 ns         6376   lomuto::sort
BM_Sorting<true>      148321 ns       148270 ns         4693   std::sort
```

## Usage
```c++
#include <lomuto/lomuto.h>

int main() {
    std::vector<long> v(13425);
    std::iota(v.begin(), v.end(), 0);
    std::shuffle(v.begin(), v.end(), std::mt19937{std::random_device{}()});

    lomuto::sort(v.begin(), v.end());
}
```

### Build and run

#### Benchmark
```bash
cmake -Hbenchmark -Bbuild/bench -GNinja -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_LTO=true
ninja -C build/bench
./build/bench/Lomuto
```

#### Tests
```bash
cmake -Htest -Bbuild/test -GNinja
ninja -C build/test
./build/test/Lomuto
```

To collect code coverage information, run CMake with the `-DENABLE_TEST_COVERAGE=1` option.

#### Formatting

Use the following commands from the project's root directory to run clang-format (must be installed on the host system).

```bash
cmake -Htest -Bbuild/test

# view changes
cmake --build build/test --target format

# apply changes
cmake --build build/test --target fix-format
```

See [Format.cmake](https://github.com/TheLartians/Format.cmake) for more options.

### Additional tools

The project includes an [tools.cmake](cmake/tools.cmake) file that can be used to import additional tools on-demand through CMake configuration arguments.
The following are currently supported.

- `-DUSE_SANITIZER=<Address | Memory | MemoryWithOrigins | Undefined | Thread | Leak | 'Address;Undefined'>`
- `-DUSE_CCACHE=<YES | NO>`