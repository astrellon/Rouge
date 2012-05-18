#include "log_entry.h"

namespace am {
namespace log {

	LogEntry::LogEntry(const char *type, const char *message)
	{
		mType = type;
		mMessage = message;
	}
	LogEntry::~LogEntry()
	{

	}

	const string &LogEntry::getType() const
	{
		return mType;
	}
	const string &LogEntry::getMessage() const
	{
		return mMessage;
	}
}
}
