#include "event_manager.h"

#include "mouse_event.h"
#include "ievent_listener.h"

namespace am {
namespace ui {

	EventManager::EventManager()
	{

	}
	EventManager::~EventManager()
	{

	}

	void EventManager::addEventListener(const char *type, IEventListener *context)
	{
		addEventListener(string(type), context);
	}
	void EventManager::addEventListener(const string &type, IEventListener *context)
	{
		if (findListener(type, context) == mListeners[type].end())
		{
			mListeners[type].push_back(context);
		}
	}
	void EventManager::removeEventListener(const char *type, IEventListener *context)
	{
		removeEventListener(string(type), context);
	}
	void EventManager::removeEventListener(const string &type, IEventListener *context)
	{
		ListenerList::iterator iter = findListener(type, context);
		if (iter != mListeners[type].end())
		{
			mListeners[type].end();
		}
		mListeners[type].erase(iter);
	}
	bool EventManager::hasEventListener(const char *type)
	{
		return hasEventListener(string(type));
	}
	bool EventManager::hasEventListener(const string &type)
	{
		return mListeners.find(type) != mListeners.end();
	}

	EventManager::ListenerList::iterator EventManager::findListener(const string &type, IEventListener *context)
	{
		ListenerList::iterator iter;
		ListenerList &listeners = mListeners[type];
		for (iter = listeners.begin(); iter != listeners.end(); ++iter)
		{
			if (*iter == context)
			{
				break;
			}
		}
		return iter;
	}

}
}
