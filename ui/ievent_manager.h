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
			if (e->getEventTarget() == NULL)
			{
				e->setEventTarget(this);
			}
			Listeners::iterator iter = mListeners.find(e->getType());
			if (iter == mListeners.end())
			{
				return;
			}

			ListenerList::iterator listIter;
			ListenerList &listeners = iter->second;
			for (int i = 0; i < static_cast<int>(listeners.size()); i++)
			{
				mFiring = true;
				listeners[i]->onEvent(e);
				mFiring = false;

				if (!e->isPropagating())
				{
					break;
				}
			}

			removeToRemove();
		}

	protected:

		Listeners mListeners;
		bool mFiring;
		Listeners mToRemove;

		bool removeToRemove()
		{
			bool didRemove = false;
			Listeners::iterator typeIter;
			for (typeIter = mToRemove.begin(); typeIter != mToRemove.end(); ++typeIter)
			{
				ListenerList::iterator listIter;
				for (listIter = typeIter->second.begin(); listIter != typeIter->second.end(); ++listIter)
				{
					didRemove = true;
					removeEventListener(typeIter->first, *listIter);
				}
			}
			mToRemove.clear();
			return didRemove;
		}
	};

}
}
