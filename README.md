# WorkerQueue
Asynchronous Worker Queue based on modern C++. This library allows you to give independent function signatures to the worker queue. It's a cross-platform library that should compile on any operating system without any problem (I have tested on Windows and Linux).

# Features
- Cross platform (Windows, MacOS, Linux, Android)
- C++14 and later are supported.
- Async function calls

# Prerequisites
- C++14 or later supported compiler
    - msvc
    - gcc
    - clang
- CMake (Optional)

# Cloning the library
You can clone the library using git. Just run the command as shown below:

```
  > git clone https://github.com/kadirlua/WorkerQueue.git
```

# Building the library
You can build the library using vcpkg or your own environment. You can use Visual Studio, VSCode or CLion IDEs for building.

## Compile the library using cmake
```
  > mkdir build
  > cmake -B build -S .
  > cmake --build build
```

Or with make option
```
  > mkdir build
  > cd build
  > cmake ..
  > make
```

If you want to build as static library (default is shared), you can pass the argument as shown below to cmake for configuration:
```
  > cmake -B build -S . -DBUILD_SHARED_LIBS=OFF
```

# Use the library
You can use the library into your project. It's easy to integrate into your project using cmake configuration. Insert the necessary codes into your project as shown below:

CMakeLists.txt:
``` cmake

cmake_minimum_required(VERSION 3.22.1)

project(TestProject VERSION 1.0 LANGUAGES CXX)

find_package(WorkerQueue REQUIRED)    # It's required to find the library

add_executable(TestProject main.cpp)

target_link_libraries(TestProject PRIVATE WorkerQueue::WorkerQueue)    # link the library if It's found
```

main.cpp:

``` cpp
#include <iostream>
#include <WorkerQueue.h>

int main()
{
    sdk::concurrency::WorkerQueue w1;
    w1.push([](){
        std::cout << "Test worker message!\n";
    });
    // wait until worker queue has finished
    while (!w1.empty()) {
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return 0;
}
```

# Conclusion
If you have any opinions or questions, please do not hesitate to ask me :)
