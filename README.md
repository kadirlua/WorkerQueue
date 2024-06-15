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

# Build the library
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

If you want to build as static library (default is shared), you can pass the argument as below to cmake for configuration;
```
  > cmake -B build -S . -DBUILD_SHARED_LIBS=OFF
```

# Conclusion
If you have any opinions or questions, please do not hesitate to ask me :)
