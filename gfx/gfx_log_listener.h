#pragma once

#include <base/handle.h>
using namespace am::base;

#include <log/ilog_listener.h>
using namespace am::log;

#include <ui/ievent_listener.h>
using namespace am::ui;

namespace am {
namespace gfx {

	class TextList;

	class GfxLogListener : public ILogListener, public IEventListener {
	public:
		GfxLogListener(TextList *output);
		~GfxLogListener();

		virtual void onEvent(Event *e);

		virtual void onNewEntry(const LogEntry &entry);
		virtual void onLoggerDelete();

	protected:

		Handle<TextList> mTextList;

	};

}
}
