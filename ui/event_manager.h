#pragma once

#include <base/handle.h>

#include "mouse_common.h"
#include "keyboard_common.h"

#include "ievent_listener.h"
#include "event.h"

#include <string>
#include <map>
#include <vector>

namespace am {
namespace ui {

	class EventInterface;

	class EventManager  {
	public:

		friend class EventInterface;

		typedef std::vector< base::Handle<IEventListener> > ListenerList;
		typedef std::map<std::string, ListenerList> Listeners;

		EventManager();
		~EventManager();

		// IEventListener methods
		bool addEventListener(const char *type, IEventListener *context);
		bool addEventListener(const std::string &type, IEventListener *context);
		bool addEventListener(Mouse::EventType type, IEventListener *context);
		bool addEventListener(Keyboard::EventType type, IEventListener *context);
		
		bool removeEventListener(const char *type, IEventListener *context);
		bool removeEventListener(const std::string &type, IEventListener *context);
		bool removeEventListener(Mouse::EventType type, IEventListener *context);
		bool removeEventListener(Keyboard::EventType type, IEventListener *context);
		void removeEventListener(IEventListener *context);

		bool hasEventListener(const char *type);
		bool hasEventListener(const std::string &type);

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
		base::Handle<EventInterface> mDeleteResponse;

		ListenerList::iterator findListener(const std::string &type, IEventListener *context);

		bool removeToRemove();
		void checkDeletion();
	};

}
}
