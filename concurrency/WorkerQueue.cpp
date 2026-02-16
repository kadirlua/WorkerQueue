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

#include <cstddef>
#include <functional>
#include <mutex>
#include <utility>
#include <workerqueue/WorkerQueue.h>

namespace sdk {
	namespace concurrency {

		WorkerQueue::WorkerQueue(std::size_t threadCnt) :
			m_threadSize{ threadCnt == 0 ? 1 : threadCnt }
		{
			for (std::size_t i = 0; i < m_threadSize; i++) {
				m_threads.emplace_back([&]() {
					std::unique_lock<std::mutex> lock{ m_lock };

					while (!m_quit) {
						m_cv.wait(lock, [this] {
							return (!m_funcQueue.empty() || m_quit);
						});

						if (!m_quit && !m_funcQueue.empty()) {
							auto func = std::move(m_funcQueue.front());
							m_funcQueue.pop();

							lock.unlock();
#if (__cplusplus >= 201703L)
							std::invoke(func);
#else
							func();
#endif
							lock.lock();
						}
					};
				});
			}
		}

		WorkerQueue::~WorkerQueue()
		{
			{
				const std::lock_guard<std::mutex> lock{ m_lock };
				m_quit = true;
			}
			m_cv.notify_all();

			// detach all threads
#if (__cplusplus < 202002L)
			for (auto& tObj : m_threads) {
				if (tObj.joinable()) {
					tObj.join();
				}
			}
#endif
		}
	}
}