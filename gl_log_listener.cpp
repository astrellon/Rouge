#include "gl_log_listener.h"

#include "gl_text_list.h"

#include "logger.h"

namespace am {
namespace gfx {

	GlLogListener::GlLogListener(GlTextList *output)
	{
		mTextList = output;

		Logger::LogEntries entries;
		Logger::getMainLogger().getEntries(entries, 20, 0, false);

		Logger::LogEntries::iterator iter;
		for (iter = entries.begin(); iter != entries.end(); ++iter)
		{
			mTextList->addEntry(iter->getMessage().c_str(), iter->getType().c_str());
		}
	}
	GlLogListener::~GlLogListener()
	{

	}

	void GlLogListener::onNewEntry(const LogEntry &entry)
	{
		mTextList->addEntry(entry.getMessage().c_str(), entry.getType().c_str());
	}

}
}
