#pragma once

#include <string>

namespace am {
namespace log {

	class Logger;

	class LogEntry 
	{
	public:
		LogEntry(const char *type, const char *message);
		~LogEntry();

		const std::string &getType() const;
		const std::string &getMessage() const;

		friend class Logger;

	protected:
		std::string mType;
		std::string mMessage;

	};
}
}
