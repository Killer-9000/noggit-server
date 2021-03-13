// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#pragma once

#include <string>
#include <fmt/printf.h>
#include <thread>
#include <boost/thread/sync_queue.hpp>

namespace Logging
{
  enum ELOG_SEVERITY
  {
    // Lowest severity
    LOGSEVERITY_EVERYTHING,
    LOGSEVERITY_DEBUG,
    LOGSEVERITY_INFO,
    LOGSEVERITY_WARN,
    LOGSEVERITY_ERROR,
    LOGSEVERITY_FATAL,
    // Highest severity
  };

  class CLogger
  {
  private:
    FILE* m_File = nullptr;
    std::thread m_LoggingThread;
    std::string m_LogFile = "./Log.txt";
    boost::sync_queue<std::string> m_Messages;
    std::atomic_bool stopThread;

    CLogger();
    ~CLogger();

    void LogAvailibleMessages();
    std::string GetSeverityString(ELOG_SEVERITY severity);

  public:
    static CLogger* Instance()
    {
      static CLogger instance;
      return &instance;
    }

    template<typename Format, typename... Args>
    inline void LogMessage(ELOG_SEVERITY severity, 
      const std::string& fileName, int lineNumber, 
      Format&& format, Args&&... args)
    {
      std::string msg = fmt::sprintf(format, args...);
      std::string severityMsg = GetSeverityString(severity);

      std::string message = fmt::sprintf("%i - (%s:%i): %s%s\n",
        (clock() * 1000 / CLOCKS_PER_SEC),
        fileName.c_str(), lineNumber,
        severityMsg.c_str(),
        msg);

      m_Messages.push(message);
    }
  };

  static constexpr const char* past_last_slash(const char* str, 
        const char* last_slash)
  {
    return
      *str == '\0' ? last_slash :
      *str == '/' || *str == '\\' ? past_last_slash(str + 1, str + 1) :
      past_last_slash(str + 1, last_slash);
  }

  static constexpr const char* past_last_slash(const char* str)
  {
    return past_last_slash(str, str);
  }
}

#define LOGGER Logging::CLogger::Instance()

#define __SHORT_FILE__ Logging::past_last_slash(__FILE__)

#define LOG_DEBUG(...) LOGGER->LogMessage(Logging::LOGSEVERITY_DEBUG, __SHORT_FILE__, __LINE__, __VA_ARGS__)
#define LOG_INFO(...)  LOGGER->LogMessage(Logging::LOGSEVERITY_INFO,  __SHORT_FILE__, __LINE__, __VA_ARGS__)
#define LOG_WARN(...)  LOGGER->LogMessage(Logging::LOGSEVERITY_WARN,  __SHORT_FILE__, __LINE__, __VA_ARGS__)
#define LOG_ERROR(...) LOGGER->LogMessage(Logging::LOGSEVERITY_ERROR, __SHORT_FILE__, __LINE__, __VA_ARGS__)
#define LOG_FATAL(...) LOGGER->LogMessage(Logging::LOGSEVERITY_FATAL, __SHORT_FILE__, __LINE__, __VA_ARGS__)
