#include "file_log_listener.h"

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
		if (mFile.is_open())
		{
			mFile.close();
		}
	}

	void FileLogListener::onNewEntry(const LogEntry &entry)
	{
		if (mFile.is_open())
		{
			mFile << entry.getType() << ":\t" << entry.getMessage() << '\n';
		}
	}

}
}
