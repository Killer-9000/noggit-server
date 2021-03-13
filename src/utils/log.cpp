// This file is part of Noggit3, licensed under GNU General Public License (version 3).

#include "log.h"
#include "misc.h"

using namespace Logging;

CLogger::CLogger()
{
	stopThread = false;
	m_LoggingThread = std::thread(&CLogger::LogAvailibleMessages, this);
	utils::SetThreadName(&m_LoggingThread, "Logging Thread");
}

CLogger::~CLogger()
{
	// Need to cleanup properly, so we make sure we got all messages,
	// and so the queue and destroy without complaining.
	stopThread = true;
	m_LoggingThread.join();
	while (!m_Messages.empty())
	{
		std::string message = m_Messages.pull();
		printf("%s", message.c_str());
		if (m_File)
		{
			fprintf(m_File, "%s", message.c_str());
		}
	}
	m_Messages.close();
	fclose(m_File);
}

void CLogger::LogAvailibleMessages()
{
	m_File = fopen(m_LogFile.c_str(), "w+");
	if (m_File == nullptr)
	{
		printf("Failed to open log file.\n");
	}
	
	std::string message;
	while(!stopThread)
	{
		if (m_Messages.try_pull(message) == boost::concurrent::queue_op_status::success)
		{
			printf("%s", message.c_str());
			if (m_File)
			{
				fprintf(m_File, "%s", message.c_str());
			}
		}
	}
}

std::string CLogger::GetSeverityString(ELOG_SEVERITY severity)
{
	switch(severity)
	{
	case LOGSEVERITY_DEBUG:
		return "[Debug] ";
	case LOGSEVERITY_INFO:
		return "";
	case LOGSEVERITY_WARN:
		return "[Warn] ";
	case LOGSEVERITY_ERROR:
		return "[Error] ";
	case LOGSEVERITY_FATAL:
		return "[Fatal] ";
	default:
		return "[Unk] ";
	}
}
