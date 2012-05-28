#pragma once

#include"ievent_manager.h"
#include "event.h"

#include <string>
#include <map>
#include <vector>

using namespace std;

namespace am {
namespace ui {

	class EventManager : public IEventManager {
	public:
		EventManager();
		~EventManager();

		// IEventListener methods
		virtual void addEventListener(const char *type, IEventListener *content);
		virtual void addEventListener(const string &type, IEventListener *content);
		virtual void removeEventListener(const char *type, IEventListener *content);
		virtual void removeEventListener(const string &type, IEventListener *content);
		virtual bool hasEventListener(const char *type);
		virtual bool hasEventListener(const string &type);

		virtual void fireEvent(Event &e);

	protected:

		typedef vector<IEventListener *> ListenerList;
		typedef map<string, ListenerList> Listeners;
		
		Listeners mListeners;

		ListenerList::iterator findListener(const string &, IEventListener *context);
	};

}
}
