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

//  Author: Kadir Altindag
//  Asynchronous Worker Queue based on modern C++.
//  This library allows you to give independent function signatures into the worker queue.
//  It's a cross-platform library which should compile on any operating system without any problem
//  (I have tested on Windows and Linux)
//  If you have any opinion or question, please do not hesitate to ask me :)

#ifndef WORKER_QUEUE_H_
#define WORKER_QUEUE_H_

#include "WorkerQueueExport.h"
#include <condition_variable>
#include <cstdlib>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

#if (__cplusplus >= 201703L)
#define NODISCARD [[nodiscard]]
#else
#define NODISCARD
#endif

namespace sdk {
	namespace concurrency {

		//	Type erasure idiom to make independent function signature
		struct ObjectWrapper {
			// move only
			ObjectWrapper(const ObjectWrapper& rhs) = delete;
			ObjectWrapper& operator=(const ObjectWrapper& rhs) = delete;
			ObjectWrapper(ObjectWrapper&& rhs) noexcept = default;
			ObjectWrapper& operator=(ObjectWrapper&& rhs) noexcept = default;

			// Constructor template to wrap any callable object
			template <typename T>
			ObjectWrapper(T obj) noexcept(std::is_nothrow_move_constructible_v<T>) :
				wrappedObject{ std::make_unique<Wrapper<T>>(std::move(obj)) }
			{
			}

			// Default destructor
			~ObjectWrapper() = default;

			// Base class for type erasure
			struct ObjectBase {
				// Default constructor
				ObjectBase() noexcept = default;

				// Pure virtual function to invoke the stored callable object
				virtual void operator()() = 0;

				// Default virtual destructor
				virtual ~ObjectBase() = default;

				// Default copy constructor
				ObjectBase(const ObjectBase&) = default;

				// Default copy assignment operator
				ObjectBase& operator=(const ObjectBase&) = default;

				// Default move constructor
				ObjectBase(ObjectBase&&) noexcept = default;

				// Default move assignment operator
				ObjectBase& operator=(ObjectBase&&) noexcept = default;
			};

			// Derived class template to wrap specific callable types
			template <typename T>
			struct Wrapper : public ObjectBase {
				// Constructor to wrap the callable object
				Wrapper(T&& obj) noexcept :
					wrappedObject{ std::move(obj) }
				{
				}

				void operator()() override
				{
					wrappedObject();
				}

				T wrappedObject;
			};

			void operator()() const
			{
#if (__cplusplus >= 201703L)
				std::invoke(*wrappedObject);
#else
				(*wrappedObject)();
#endif
			}

			std::unique_ptr<ObjectBase> wrappedObject;
		};

		class WORKERQUEUE_API WorkerQueue {
#if (__cplusplus >= 202002L)
			using worker_thread = std::jthread;
#else
			using worker_thread = std::thread;
#endif
			using size_type = std::queue<ObjectWrapper>::size_type;

		public:
			explicit WorkerQueue(std::size_t threadCnt = std::thread::hardware_concurrency());

			// non-copyable class
			WorkerQueue(const WorkerQueue& rhs) = delete;
			WorkerQueue& operator=(const WorkerQueue& rhs) = delete;
			WorkerQueue(WorkerQueue&& rhs) noexcept = delete;
			WorkerQueue& operator=(WorkerQueue&& rhs) noexcept = delete;

			virtual ~WorkerQueue();

			/// <summary>
			/// Push an operation into the main queue.
			/// </summary>
			template <typename Fn, typename... Args>
			void push(Fn&& func, Args&&... args)
			{
				{
					const std::lock_guard<std::mutex> lock{ m_lock };
					m_funcQueue.emplace(std::bind(std::forward<Fn>(func),
						std::forward<Args>(args)...));
				}
				m_cv.notify_one();
			}

			/// <summary>
			/// returns the number of worker threads.
			/// </summary>
			/// <returns>number of threads</returns>
			NODISCARD std::size_t size() const noexcept
			{
				return m_threadSize;
			}

			/// <summary>
			/// returns the number of jobs into queue.
			/// </summary>
			/// <returns>number of jobs</returns>
			NODISCARD size_type getQueueSize() const noexcept
			{
				return m_funcQueue.size();
			}

			/// <summary>
			/// Checks if the worker queue is empty.
			/// </summary>
			/// <returns>True if the queue is empty, otherwise false.</returns>
			NODISCARD bool empty() const noexcept
			{
				return m_funcQueue.empty();
			}

		private:
			bool m_quit{};            // quit flag to notify the threads waiting
			std::size_t m_threadSize; // max number of threads

			std::mutex m_lock;                     // locking operations
			std::condition_variable m_cv;          // notify the threads waiting
			std::queue<ObjectWrapper> m_funcQueue; // main queue to hold operations
			std::vector<worker_thread> m_threads;  // thread container
		};
	} // namespace concurrency
} // namespace sdk

#endif
