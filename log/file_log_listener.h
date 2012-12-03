#pragma once

#include "ilog_listener.h"
#include "log_entry.h"

#include <fstream>
#include <ostream>

using namespace std;

namespace am {
namespace log {

	class FileLogListener : public ILogListener {
	public:
		FileLogListener(const char *file);
		~FileLogListener();

		virtual void onNewEntry(const LogEntry &entry);
		virtual void onLoggerDelete();

	protected:

		ofstream mFile;
	};

}
}
