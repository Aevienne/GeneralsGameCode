#pragma once
#include "Common/GameCommon.h"
#include <functional>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <queue>

class JobSystem {
public:
    using JobFunc = std::function<void()>;

    static JobSystem* getInstance();
    static void create();
    static void destroy();

    void init(int numWorkers = 0);
    void shutdown();

    void submit(JobFunc job);
    template <typename F>
    void parallelFor(int count, F&& func) {
        if (count <= 0) return;
        std::atomic<int> next{0};
        int workers = (int)m_workers.size() + 1;
        int chunk = (count + workers - 1) / workers;
        std::atomic<int> done{0};
        for (int w = 0; w < workers; ++w) {
            submit([&, w]{
                for (;;) {
                    int start = w * chunk;
                    int end = std::min(start + chunk, count);
                    if (start >= end) break;
                    for (int i = start; i < end; ++i) func(i);
                    break;
                }
                done.fetch_add(1);
            });
        }
        while (done.load() < workers) std::this_thread::yield();
        joinAll();
    }

    void joinAll();
    bool isActive() const { return m_running; }

private:
    JobSystem() = default;
    void workerLoop();

    std::vector<std::thread> m_workers;
    std::queue<JobFunc> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::condition_variable m_doneCv;
    std::atomic<int> m_pending{0};
    std::atomic<bool> m_running{false};
    bool m_shutdown = false;
};
#define TheJobSystem JobSystem::getInstance()
