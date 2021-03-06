
//  Author: Kadir ALTINDAG
//  Asynchronous Worker Queue based on modern C++.
//  This library allows you to give independented function signatures into the worker queue.
//  It's a cross platform library which should compile on any operating system without any problem
//  (I have tested on Windows and Linux)
//  If you have any opinion or question, please do not hesitate to ask me :)

#ifndef WORKER_QUEUE_H_
#define WORKER_QUEUE_H_

#include <functional>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "version.h"

#if __cplusplus >= 201703L
#define NODISCARD   [[nodiscard]]
#else
#define NODISCARD
#endif

namespace sdk {
	namespace concurrency {

#define WORKERQUEUE_VERSION_MAJOR 1
#define WORKERQUEUE_VERSION_MINOR 0
#define WORKERQUEUE_VERSION_PATCH 0

		//	Type erasure idiom to make independented function signature
        struct ObjectWrapper
        {   
            // move only
            ObjectWrapper(const ObjectWrapper& rhs) = delete;
            ObjectWrapper& operator=(const ObjectWrapper& rhs) = delete;
            ObjectWrapper(ObjectWrapper&& rhs) = default;
            ObjectWrapper& operator=(ObjectWrapper&& rhs) = default;

            template <typename T>
            ObjectWrapper(T&& obj) :
                wrappedObject{ std::make_unique<Wrapper<T>>(std::forward<T>(obj)) } 
            { 
            }

            struct ObjectBase
            {
                virtual void operator()() = 0;
                virtual ~ObjectBase() = default;
            };

            template<typename T>
            struct Wrapper : public ObjectBase
            {
                Wrapper(T&& t) noexcept :
                    wrappedObject(std::forward<T>(t)) 
                {
                }

                void operator()() override
                {
                    wrappedObject();
                }

                T wrappedObject;
            };

            void operator()()
            {
#if __cplusplus >= 201703L
                std::invoke(*wrappedObject);
#else
                (*wrappedObject)();
#endif
            }

            std::unique_ptr<ObjectBase> wrappedObject;
        };

        class WorkerQueue
        {
#if __cplusplus >= 202002L
            using worker_thread = std::jthread;
#else
            using worker_thread = std::thread;
#endif
        public:
            WorkerQueue(std::size_t thread_cnt = std::thread::hardware_concurrency()) : 
                m_thread_size{ thread_cnt == 0 ? 1 : thread_cnt }
            {
                for (std::size_t i = 0; i < m_thread_size; i++)
                    m_threads.emplace_back([&](){                
                    
                        std::unique_lock<std::mutex> lock(m_lock);

                        do
                        {
                            m_cv.wait(lock, [this] { 
                                return (m_funcQueue.size() || m_quit); 
                                });

                            if (!m_quit && m_funcQueue.size())
                            {
                                auto op = std::move(m_funcQueue.front());
                                m_funcQueue.pop();

                                lock.unlock();
                                op();
                                lock.lock();
                            }
                        } while (!m_quit);
                    }
                );
            }

            // non-copyable class
            WorkerQueue(const WorkerQueue& rhs) = delete;
            WorkerQueue& operator=(const WorkerQueue& rhs) = delete;
            
            virtual ~WorkerQueue()
            {
                std::unique_lock<std::mutex> lock(m_lock);
                m_quit = true;
                lock.unlock();
                m_cv.notify_all();

                // detach all threads
#if __cplusplus < 202002L
                for (auto& th : m_threads)
                {
                    if (th.joinable())
                        th.join();
                }
#endif
            }

            /// <summary>
            /// Push an operation into the main queue.
            /// </summary>
            template<typename Fn, typename ...Args>
            void push(Fn&& fn, Args&& ...args) {
                std::unique_lock<std::mutex> lock(m_lock);
                m_funcQueue.emplace(std::bind(std::forward<Fn>(fn), 
                    std::forward<Args>(args)...));

                lock.unlock();
                m_cv.notify_one();
            }

            /// <summary>
            /// returns the version number of this library
            /// </summary>
            /// <returns>version string number</returns>
            NODISCARD static const char* getVersionStr() noexcept {
                return VERSION_STR(WORKERQUEUE_VERSION_MAJOR,
                    WORKERQUEUE_VERSION_MINOR, WORKERQUEUE_VERSION_PATCH);
            }

            /// <summary>
            /// returns the number of worker threads which are currently alive.
            /// </summary>
            /// <returns>number of threads</returns>
            NODISCARD std::size_t size() const noexcept {
                return m_thread_size;
            }

        private:
            bool m_quit{};  // quit flag to notify the threads waiting
            std::size_t m_thread_size;  // max number of threads

            std::mutex m_lock;  // locking operations
            std::condition_variable m_cv;   // notify the threads waiting
            std::queue<ObjectWrapper> m_funcQueue;  // main queue to hold operations
            std::vector<worker_thread> m_threads;   // thread container
        };   
    }
}

#endif