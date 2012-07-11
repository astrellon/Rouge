#pragma once

#include "event.h"
#include "ievent_listener.h"

#include "mouse_common.h"
#include "keyboard_common.h"

#include <vector>
#include <map>
#include <string>

using namespace std;

namespace am {
namespace ui {

	class IEventManager {
	public:
		typedef vector<IEventListener *> ListenerList;
		typedef map<string, ListenerList> Listeners;

		virtual void addEventListener(const char *type, IEventListener *content) = 0;
		virtual void addEventListener(const string &type, IEventListener *content) = 0;
		void addEventListener(MouseEventType type, IEventListener *content)
		{
			addEventListener(MouseEventTypeNames[type], content);
		}
		void addEventListener(KeyboardEventType type, IEventListener *content)
		{
			addEventListener(KeyboardEventTypeNames[type], content);
		}
		virtual void removeEventListener(const char *type, IEventListener *content) = 0;
		virtual void removeEventListener(const string &type, IEventListener *content) = 0;
		void removeEventListener(MouseEventType type, IEventListener *content)
		{
			removeEventListener(MouseEventTypeNames[type], content);
		}
		void removeEventListener(KeyboardEventType type, IEventListener *content)
		{
			removeEventListener(KeyboardEventTypeNames[type], content);
		}
		virtual bool hasEventListener(const char *type) = 0;
		virtual bool hasEventListener(const string &type) = 0;

		template <class T>
		void fireEvent(T *e)
		{
			Listeners::iterator iter = mListeners.find(e->getType());
			if (iter == mListeners.end())
			{
				return;
			}

			ListenerList::iterator listIter;
			for (listIter = iter->second.begin(); listIter != iter->second.end(); ++listIter)
			{
				(*listIter)->onEvent(e);
				if (!e->isPropagating())
				{
					break;
				}
			}
		}

	protected:

		Listeners mListeners;
	};

}
}
