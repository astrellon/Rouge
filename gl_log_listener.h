#pragma once

#include "ilog_listener.h"

using namespace am::log;

namespace am {
namespace gfx {

	class GlTextList;

	class GlLogListener : public ILogListener {
	public:
		GlLogListener(GlTextList *output);
		~GlLogListener();

		virtual void onNewEntry(const LogEntry &entry);

	protected:

		GlTextList *mTextList;

	};

}
}
