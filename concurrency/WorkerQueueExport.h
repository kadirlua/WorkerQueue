// MIT License

// Copyright (c) 2021-2025 kadirlua

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#ifndef WORKERQUEUE_EXPORT_H_
#define WORKERQUEUE_EXPORT_H_

/* Windows DLL export/import */
#if defined(_WIN32) && !defined(__GNUC__) && defined(WORKERQUEUE_COMPILE_DLL)
#if WORKERQUEUE_DLL_EXPORT
#define WORKERQUEUE_API __declspec(dllexport)
#else
#define WORKERQUEUET_API __declspec(dllimport)
#endif
#else
#if __GNUC__ >= 4
#define WORKERQUEUE_API __attribute__((visibility("default")))
#else
#define WORKERQUEUE_API
#endif
#endif

#endif