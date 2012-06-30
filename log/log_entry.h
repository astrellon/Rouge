#pragma once

#include <string>

using namespace std;

namespace am {
namespace log {

	class Logger;

	class LogEntry {
	public:
		LogEntry(const char *type, const char *message);
		~LogEntry();

		const string &getType() const;
		const string &getMessage() const;

		friend class Logger;

	protected:
		string mType;
		string mMessage;

	};
}
}
