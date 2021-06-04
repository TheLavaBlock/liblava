/**
 * @file iblava/util/thread.hpp
 * @brief Thread pool
 * @authors Lava Block OÜ and contributors
 * @copyright Copyright (c) 2018-present, MIT License
 */

#pragma once

#include <condition_variable>
#include <deque>
#include <liblava/core/id.hpp>
#include <liblava/core/time.hpp>
#include <mutex>
#include <thread>

namespace lava {

/**
 * @brief Sleep for milliseconds
 * 
 * @param time Milliseconds to sleep
 */
inline void sleep(ms time) {
    std::this_thread::sleep_for(time);
}

/**
 * @brief Sleep for seconds
 * 
 * @param time Seconds to sleep
 */
inline void sleep(seconds time) {
    std::this_thread::sleep_for(time);
}

/**
 * @brief Thread pool
 */
struct thread_pool {
    /// Task function (with thread id)
    using task = std::function<void(id::ref)>;

    /**
     * @brief Set up the thread pool
     * 
     * @param count Number of threads
     */
    void setup(ui32 count = 2) {
        for (auto i = 0u; i < count; ++i)
            workers.emplace_back(worker(*this));
    }

    /**
     * @brief Tear down the thread pool
     */
    void teardown() {
        stop = true;
        condition.notify_all();

        for (auto& worker : workers)
            worker.join();

        workers.clear();
    }

    /**
     * @brief Enqueue a task
     * 
     * @tparam F Type of task function
     * @param f Task function
     */
    template<typename F>
    void enqueue(F f) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            tasks.push_back(task(f));
        }
        condition.notify_one();
    }

private:
    /**
     * @brief Thread worker
     */
    struct worker {
        /**
         * @brief Construct a new worker
         * 
         * @param pool Thread pool
         */
        explicit worker(thread_pool& pool)
        : pool(pool) {}

        /**
         * @brief Eun task operator
         */
        void operator()() {
            auto thread_id = ids::next();

            task task;
            while (true) {
                {
                    std::unique_lock<std::mutex> lock(pool.queue_mutex);

                    while (!pool.stop && pool.tasks.empty())
                        pool.condition.wait(lock);

                    if (pool.stop)
                        break;

                    task = pool.tasks.front();
                    pool.tasks.pop_front();
                }

                task(thread_id);
            }

            ids::free(thread_id);
        }

    private:
        /// Thread pool
        thread_pool& pool;
    };

    /// List of workers
    std::vector<std::thread> workers;

    /// List of tasks
    std::deque<task> tasks;

    /// Queue mutex
    std::mutex queue_mutex;

    /// Condition variable
    std::condition_variable condition;

    /// Stop state
    bool stop = false;
};

} // namespace lava
