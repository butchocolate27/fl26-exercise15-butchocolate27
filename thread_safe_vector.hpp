#ifndef THREAD_SAFE_VECTOR_HPP
#define THREAD_SAFE_VECTOR_HPP

#include <condition_variable>
#include <mutex>
#include <vector>

template<typename T>
class ThreadSafeVector {
public:
    void push_back(const T& value) {
        std::unique_lock<std::mutex> lock(mutex);
        values.push_back(value);
        lock.unlock();

        // Notify one waiting thread
        data_ready.notify_one();
    }

    void wait_and_pop_back(T& value) {
        std::unique_lock<std::mutex> lock(mutex);

        // Wait for data
        while (values.empty()) {
            data_ready.wait(lock);
        }
        value = values.back();
        values.pop_back();
    }

private:
    std::vector<T> values;
    std::mutex mutex;
    std::condition_variable data_ready;
};

#endif
