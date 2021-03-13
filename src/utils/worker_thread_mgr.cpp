#include "worker_thread_mgr.h"

void CWorkerThreadMgr::Initialize(size_t workerCount)
{
    for (int i = 0; i < workerCount; i++)
    {
        m_workerThreads.emplace_back(std::thread([&](){ WorkerProcess(); }));
    }
}

void CWorkerThreadMgr::WorkerProcess()
{
    std::function<void()> job;
    while(!m_stop)
    {
        if (m_jobs.try_pull(job) == boost::concurrent::queue_op_status::success)
        {
            job();
        }
    }
}
