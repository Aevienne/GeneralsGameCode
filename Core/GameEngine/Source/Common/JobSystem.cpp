#include "PreRTS.h"
#include "Common/JobSystem.h"

static JobSystem* s_instance = nullptr;

JobSystem* JobSystem::getInstance() { return s_instance; }
void JobSystem::create() { if (!s_instance) s_instance = new JobSystem; }
void JobSystem::destroy() { delete s_instance; s_instance = nullptr; }

void JobSystem::init(int numWorkers) {
    if (m_running) return;
    if (numWorkers <= 0) {
        numWorkers = (int)std::thread::hardware_concurrency();
        if (numWorkers > 1) numWorkers -= 1;
        else numWorkers = 1;
        if (numWorkers > 4) numWorkers = 4;
    }
    m_running = true;
    m_shutdown = false;
    m_pending = 0;
    for (int i = 0; i < numWorkers; ++i) {
        m_workers.emplace_back([this]{ workerLoop(); });
    }
}

void JobSystem::shutdown() {
    if (!m_running) return;
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_shutdown = true;
    }
    m_cv.notify_all();
    for (auto& t : m_workers) if (t.joinable()) t.join();
    m_workers.clear();
    m_running = false;
}

void JobSystem::submit(JobFunc job) {
    if (!m_running || !job) { job(); return; }
    {
        std::lock_guard<std::mutex> lk(m_mutex);
        m_queue.push(std::move(job));
        m_pending.fetch_add(1);
    }
    m_cv.notify_one();
}

void JobSystem::joinAll() {
    std::unique_lock<std::mutex> lk(m_mutex);
    m_doneCv.wait(lk, [this]{ return m_pending.load() == 0 && m_queue.empty(); });
}

void JobSystem::workerLoop() {
    while (true) {
        JobFunc job;
        {
            std::unique_lock<std::mutex> lk(m_mutex);
            m_cv.wait(lk, [this]{ return m_shutdown || !m_queue.empty(); });
            if (m_shutdown && m_queue.empty()) return;
            if (m_queue.empty()) continue;
            job = std::move(m_queue.front());
            m_queue.pop();
        }
        job();
        int left = m_pending.fetch_sub(1) - 1;
        if (left == 0) {
            std::lock_guard<std::mutex> lk(m_mutex);
            m_doneCv.notify_all();
        }
    }
}
