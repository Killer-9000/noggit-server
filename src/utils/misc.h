#pragma once
#include <thread>

namespace utils
{
#if defined(__linux__)
inline void SetThreadName(std::thread* thread, const char* threadName)
{
   pthread_setname_np(thread->native_handle(), threadName);
}
#else
inline void SetThreadName(std::thread* thread, const char* threadName)
{
    // NOT IMPLEMENTED
}
#endif
}
