#ifndef TASK_QUEUE_H
#define TAKS_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>
#include <atomic>

class TaskQueue {
    public:
        TaskQueue(size_t numThreads);
        ~TaskQueue();

        void enqueue(const std::function<void()>& task);
        void stop();

    private:
        void worker();

        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable condition;
        std::atomic<bool> stopFlag;
};

#endif