#include "gfx_log_listener.h"

#include "gfx_text_list.h"

#include <log/logger.h>
using namespace am::log;

namespace am {
namespace gfx {

	GfxLogListener::GfxLogListener(TextList *output)
	{
		mTextList = output;
		output->addEventListener("deinit", this);

		Logger::LogEntries entries;
		Logger::getMainLogger()->getEntries(entries, 20, 0, false);

		Logger::LogEntries::iterator iter;
		for (iter = entries.begin(); iter != entries.end(); ++iter)
		{
			mTextList->addEntry(iter->getMessage().c_str(), iter->getType().c_str());
		}
	}
	GfxLogListener::~GfxLogListener()
	{
		Logger *logger = Logger::getMainLogger();
		if (logger)
		{
			logger->removeLogListener(this);
		}
	}

	void GfxLogListener::onEvent(ui::Event *e)
	{
		if (e->getEventTarget() == mTextList.get())
		{
			mTextList = nullptr;
		}
	}

	void GfxLogListener::onNewEntry(const log::LogEntry &entry)
	{
		if (mTextList.get())
		{
			mTextList->addEntry(entry.getMessage().c_str(), entry.getType().c_str());
		}
	}
	void GfxLogListener::onLoggerDelete()
	{
		//delete this;
	}

}
}
