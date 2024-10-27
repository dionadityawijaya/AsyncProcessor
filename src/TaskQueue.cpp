#include "TaskQueue.h"

TaskQueue::TaskQueue(size_t numThreads) : stopFlag(false) {
    for (size_t i = 0; i < numThreads; ++i) {
        workers.emplace_back([this] { worker(); });
    }
}

TaskQueue::~TaskQueue() {
    stop();
}

void TaskQueue::enqueue(const std::function<void()>& task) {
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.push(task);
    }
    condition.notify_one();
}

void TaskQueue::stop() {
    stopFlag = true;
    condition.notify_all();
    for (std::thread &worker : workers) {
        worker.join();
    }
}

void TaskQueue::worker() {
    while (!stopFlag) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            condition.wait(lock, [this] { return stopFlag || !tasks.empty(); });
            if (stopFlag && tasks.empty()) {
                return;
            }
            task = std::move(tasks.front());
            tasks.pop();
        }
        task();
    }
}
