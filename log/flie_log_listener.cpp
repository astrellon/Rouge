#include "file_log_listener.h"

#include "logger.h"

namespace am {
namespace log {

	FileLogListener::FileLogListener(const char *file)
	{
		mFile.open(file, std::ios_base::out);
		if (!mFile.is_open())
		{
			// Cry
		}
	}
	FileLogListener::~FileLogListener()
	{
		Logger *logger = Logger::getMainLogger();
		if (logger)
		{
			logger->removeLogListener(this);
		}
		mFile.flush();
		mFile.close();
	}

	void FileLogListener::onNewEntry(const LogEntry &entry)
	{
		if (mFile.is_open())
		{
			mFile << entry.getType() << ":\t" << entry.getMessage() << std::endl;
		}
	}

	void FileLogListener::onLoggerDelete()
	{
		//delete this;
	}

}
}
