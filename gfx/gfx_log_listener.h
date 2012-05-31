#pragma once

#include "../ilog_listener.h"

using namespace am::log;

namespace am {
namespace gfx {

	class TextList;

	class GfxLogListener : public ILogListener {
	public:
		GfxLogListener(TextList *output);
		~GfxLogListener();

		virtual void onNewEntry(const LogEntry &entry);

	protected:

		TextList *mTextList;

	};

}
}
