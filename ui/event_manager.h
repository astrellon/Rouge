#pragma once

#include <base/handle.h>
using namespace am::base;

#include "mouse_common.h"
#include "keyboard_common.h"

#include "ievent_listener.h"
#include "event.h"

#include <string>
#include <map>
#include <vector>
using namespace std;

namespace am {
namespace ui {

	class EventInterface;

	class EventManager  {
	public:

		friend class EventInterface;

		typedef vector< Handle<IEventListener> > ListenerList;
		typedef map<string, ListenerList> Listeners;

		EventManager();
		~EventManager();

		// IEventListener methods
		bool addEventListener(const char *type, IEventListener *context);
		bool addEventListener(const string &type, IEventListener *context);
		bool addEventListener(MouseEventType type, IEventListener *context);
		bool addEventListener(KeyboardEventType type, IEventListener *context);
		
		bool removeEventListener(const char *type, IEventListener *context);
		bool removeEventListener(const string &type, IEventListener *context);
		bool removeEventListener(MouseEventType type, IEventListener *context);
		bool removeEventListener(KeyboardEventType type, IEventListener *context);
		void removeEventListener(IEventListener *context);

		bool hasEventListener(const char *type);
		bool hasEventListener(const string &type);

		bool isEmpty() const;
		bool isFiring() const;

		void flagDeletion(EventInterface *response);

		template <class T>
		void fireEvent(T *e)
		{
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

			checkDeletion();
		}

	protected:

		Listeners mListeners;
		Listeners mToRemove;
		bool mFiring;
		EventInterface *mDeleteResponse;

		ListenerList::iterator findListener(const string &type, IEventListener *context);

		bool removeToRemove();
		void checkDeletion();
	};

}
}
