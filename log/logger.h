#pragma once

#include "log_entry.h"

#include <string>
#include <sstream>
#include <vector>

#ifdef _VERBOSE_LOG
#	define am_log(type, msg) am::log::_log_verbose(type, msg, __FILE__, __LINE__)
#	define am_log_e(entry)	 am::log::_log_verbose(entry, __FILE__, __LINE__)
#else
#	define am_log(type, msg) am::log::_log(type, msg)
#	define am_log_e(entry)	 am::log::_log(entry)
#endif

namespace am {
namespace log {

	class ILogListener;

	class Logger 
	{
	public:

		typedef std::vector<LogEntry> LogEntries;

		Logger();
		~Logger();

		void log(const char *type, const char *message);
		void log(const char *type, const std::string &message);
		void log(const char *type, const std::stringstream &message);
		void log(const LogEntry &entry);

		void log_verbose(const char *type, const char *message, const char *file, int line);
		void log_verbose(const char *type, const std::string &message, const char *file, int line);
		void log_verbose(const char *type, const std::stringstream &message, const char *file, int line);
		void log_verbose(LogEntry &entry, const char *file, int line);

		void addLogListener(ILogListener *listener);
		void removeLogListener(ILogListener *listener);
		bool hasLogListener(ILogListener *listener);

		void clearListeners();

		void getEntries(LogEntries &entries, int num, int offset, bool fromStart);

		static void setMainLogger(Logger *logger);
		static Logger *getMainLogger();
		static void clearMainLogger();

	protected:
		
		LogEntries mEntries;
		int mMaxEntries;

		typedef std::vector<ILogListener *> LogListeners;
		LogListeners mListeners;

		LogListeners::iterator findListener(ILogListener *listener);
		void alertListeners(const LogEntry &entry);

		static Logger *sMainLogger;
	};

	void _log_verbose(const char *type, const char *message, const char *file, int line);
	void _log_verbose(const char *type, const std::string &message, const char *file, int line);
	void _log_verbose(const char *type, const std::stringstream &message, const char *file, int line);
	void _log_verbose(LogEntry &entry, const char *file, int line);

	void _log(const char *type, const char *message);
	void _log(const char *type, const std::string &message);
	void _log(const char *type, const std::stringstream &message);
	void _log(const LogEntry &entry);

}
}
