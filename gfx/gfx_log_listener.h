#pragma once

#include <base/handle.h>

#include <log/ilog_listener.h>

#include <ui/ievent_listener.h>

namespace am {
namespace gfx {

	class TextList;

	class GfxLogListener : public log::ILogListener, public ui::IEventListener
	{
	public:
		GfxLogListener(TextList *output);
		~GfxLogListener();

		virtual void onEvent(ui::Event *e);

		virtual void onNewEntry(const log::LogEntry &entry);
		virtual void onLoggerDelete();

	protected:

		base::Handle<TextList> mTextList;

	};

}
}
