// MIT License

// Copyright (c) 2021-2026 kadirlua

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


#ifndef WORKER_QUEUE_H_
#define WORKER_QUEUE_H_

#include <condition_variable>
#include <cstdlib>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>
#include <utility>
#include <vector>

#include <workerqueue/WorkerQueueExport.h>

#if (__cplusplus >= 201703L)
#define NODISCARD [[nodiscard]]
#else
#define NODISCARD
#endif

namespace sdk {
	namespace concurrency {

		// Type erasure idiom to make independent function signature
		struct ObjectWrapper {
			// move only
			ObjectWrapper(const ObjectWrapper& rhs) = delete;
			ObjectWrapper& operator=(const ObjectWrapper& rhs) = delete;
			ObjectWrapper(ObjectWrapper&& rhs) noexcept = default;
			ObjectWrapper& operator=(ObjectWrapper&& rhs) noexcept = default;

			// Constructor template to wrap any callable object
			template <typename T>
			explicit ObjectWrapper(T&& obj) noexcept(std::is_nothrow_constructible<typename std::decay<T>::type, T&&>::value) :
				wrappedObject{ new Wrapper<typename std::decay<T>::type>(std::forward<T>(obj)) }
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
				template <typename U>
				explicit Wrapper(U&& obj) noexcept(std::is_nothrow_constructible<T, U&&>::value) :
					wrappedObject{ std::forward<U>(obj) }
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
			using worker_thread = std::thread;
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
				static_cast<void>(tryPush(std::forward<Fn>(func), std::forward<Args>(args)...));
			}

			/// <summary>
			/// Try to push an operation into the main queue.
			/// </summary>
			/// <returns>True if the operation was queued, otherwise false.</returns>
			template <typename Fn, typename... Args>
			NODISCARD bool tryPush(Fn&& func, Args&&... args)
			{
				ObjectWrapper task{ std::bind(std::forward<Fn>(func), std::forward<Args>(args)...) };

				{
					const std::lock_guard<std::mutex> lock{ m_lock };
					if (m_quit) {
						return false;
					}

					m_funcQueue.emplace(std::move(task));
				}

				m_cv.notify_one();
				return true;
			}

			/// <summary>
			/// Wait until all queued and running jobs are finished.
			/// </summary>
			void wait() noexcept;

			/// <summary>
			/// Stop accepting new jobs and stop the workers.
			/// </summary>
			/// <param name="waitForTasks">If true, queued jobs are completed before stopping.</param>
			void shutdown(bool waitForTasks = true) noexcept;

			/// <summary>
			/// returns the number of worker threads.
			/// </summary>
			/// <returns>number of threads</returns>
			NODISCARD std::size_t size() const noexcept
			{
				return m_threadSize;
			}

			/// <summary>
			/// returns the number of queued jobs.
			/// </summary>
			/// <returns>number of queued jobs</returns>
			NODISCARD size_type getQueueSize() const noexcept;

			/// <summary>
			/// Checks if the worker queue has no queued or running jobs.
			/// </summary>
			/// <returns>True if the queue has no work, otherwise false.</returns>
			NODISCARD bool empty() const noexcept;

			/// <summary>
			/// Checks if the worker queue is stopped.
			/// </summary>
			/// <returns>True if the queue is stopped, otherwise false.</returns>
			NODISCARD bool stopped() const noexcept;

		private:
			void joinThreads() noexcept;

			bool m_quit{};                         // quit flag to notify the threads waiting
			std::size_t m_threadSize;              // max number of threads
			std::size_t m_activeWorkers{};         // number of worker threads currently running jobs
			mutable std::mutex m_lock;             // locking operations
			std::condition_variable m_cv;          // notify the threads waiting
			std::condition_variable m_finishedCv;  // notify waiters when all work has finished
			std::queue<ObjectWrapper> m_funcQueue; // main queue to hold operations
			std::vector<worker_thread> m_threads;  // thread container
		};

	} // namespace concurrency
} // namespace sdk

#endif
