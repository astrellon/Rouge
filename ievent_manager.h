#pragma once

#include "event.h"
#include "ievent_listener.h"

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
		virtual void removeEventListener(const char *type, IEventListener *content) = 0;
		virtual void removeEventListener(const string &type, IEventListener *content) = 0;
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
