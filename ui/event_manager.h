#pragma once

#include <base/handle.h>
using am::base::Handle;

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

		typedef vector< Handle<IEventListener> > ListenerList;
		typedef map<string, ListenerList> Listeners;

		EventManager();
		~EventManager();

		// IEventListener methods
		void addEventListener(const char *type, IEventListener *content);
		void addEventListener(const string &type, IEventListener *content);
		void addEventListener(MouseEventType type, IEventListener *content);
		void addEventListener(KeyboardEventType type, IEventListener *content);
		
		void removeEventListener(const char *type, IEventListener *content);
		void removeEventListener(const string &type, IEventListener *content);
		void removeEventListener(MouseEventType type, IEventListener *content);
		void removeEventListener(KeyboardEventType type, IEventListener *content);

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
