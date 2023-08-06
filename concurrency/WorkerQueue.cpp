#include "WorkerQueue.h"

namespace sdk {
    namespace concurrency {
        WorkerQueue::WorkerQueue(std::size_t thread_cnt) :
				m_thread_size{ thread_cnt == 0 ? 1 : thread_cnt }
			{
				for (std::size_t i = 0; i < m_thread_size; i++) {
					m_threads.emplace_back([&]() {
						std::unique_lock<std::mutex> lock(m_lock);

						do {
							m_cv.wait(lock, [this] {
								return (m_funcQueue.size() || m_quit);
							});

							if (!m_quit && m_funcQueue.size()) {
								auto op = std::move(m_funcQueue.front());
								m_funcQueue.pop();

								lock.unlock();
								op();
								lock.lock();
							}
						} while (!m_quit);
					});
				}
			}

        WorkerQueue::~WorkerQueue()
        {
            m_lock.lock();
            m_quit = true;
            m_lock.unlock();
            m_cv.notify_all();

            // detach all threads
#if __cplusplus < 202002L
            for (auto& th : m_threads) {
                if (th.joinable()) {
                    th.join();
                }
            }
#endif
        }
    }
}