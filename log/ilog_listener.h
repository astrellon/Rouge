#pragma once

#include "log_entry.h"

namespace am {
namespace log {

	class ILogListener 
	{
	public:
		virtual void onNewEntry(const LogEntry &entry) = 0;
		virtual void onLoggerDelete() = 0;
	};

}
}
