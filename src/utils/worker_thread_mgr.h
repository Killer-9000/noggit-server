#pragma once

#include <vector>
#include <thread>
#include <atomic>
#include <boost/thread/sync_queue.hpp>

class CWorkerThreadMgr
{
    CWorkerThreadMgr() { }
    ~CWorkerThreadMgr() { m_stop = true; }

public:
    static CWorkerThreadMgr& Instance()
    {
        static CWorkerThreadMgr inst;
        return inst;
    }

    void Initialize(size_t workerCount);

    void AddJob(std::function<void()> job)
    {
        m_jobs.push(job);
    }

private:
    void WorkerProcess();

    std::atomic_bool m_stop = false;
    std::vector<std::thread> m_workerThreads;
    boost::sync_queue<std::function<void()>> m_jobs;
};

#define sWorkerThreadMgr CWorkerThreadMgr::Instance()
