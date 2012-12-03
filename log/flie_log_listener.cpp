#include "file_log_listener.h"

#include "logger.h"

namespace am {
namespace log {

	FileLogListener::FileLogListener(const char *file)
	{
		mFile.open(file, ios_base::out);
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
	}

	void FileLogListener::onNewEntry(const LogEntry &entry)
	{
		if (mFile.is_open())
		{
			mFile << entry.getType() << ":\t" << entry.getMessage() << endl;
		}
	}

	void FileLogListener::onLoggerDelete()
	{
		//delete this;
	}

}
}
