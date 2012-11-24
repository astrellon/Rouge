#include "event_manager2.h"

#include "mouse_event.h"
#include "ievent_listener.h"

namespace am {
namespace ui {

	EventManager2::EventManager2()
	{

	}
	EventManager2::~EventManager2()
	{

	}

	void EventManager2::addEventListener(const char *type, IEventListener *context)
	{
		addEventListener(string(type), context);
	}
	void EventManager2::addEventListener(const string &type, IEventListener *context)
	{
		if (findListener(type, context) == mListeners[type].end())
		{
			mListeners[type].push_back(context);
		}
	}
	void EventManager2::addEventListener(MouseEventType type, IEventListener *content)
	{
		addEventListener(MouseEventTypeNames[type], content);
	}
	void EventManager2::addEventListener(KeyboardEventType type, IEventListener *content)
	{
		addEventListener(KeyboardEventTypeNames[type], content);
	}

	void EventManager2::removeEventListener(const char *type, IEventListener *context)
	{
		removeEventListener(string(type), context);
	}
	void EventManager2::removeEventListener(const string &type, IEventListener *context)
	{
		ListenerList::iterator iter = findListener(type, context);
		if (iter != mListeners[type].end())
		{
			mListeners[type].erase(iter);
			if (mListeners[type].empty())
			{
				mListeners.erase(mListeners.find(type));
			}
		}
	}
	void EventManager2::removeEventListener(MouseEventType type, IEventListener *content)
	{
		removeEventListener(MouseEventTypeNames[type], content);
	}
	void EventManager2::removeEventListener(KeyboardEventType type, IEventListener *content)
	{
		removeEventListener(KeyboardEventTypeNames[type], content);
	}

	bool EventManager2::hasEventListener(const char *type)
	{
		return hasEventListener(string(type));
	}
	bool EventManager2::hasEventListener(const string &type)
	{
		return mListeners.find(type) != mListeners.end();
	}

	EventManager2::ListenerList::iterator EventManager2::findListener(const string &type, IEventListener *context)
	{
		ListenerList::iterator iter;
		ListenerList &listeners = mListeners[type];
		for (iter = listeners.begin(); iter != listeners.end(); ++iter)
		{
			if (context->compareListeners(*iter))
			{
				break;
			}
		}
		return iter;
	}

	bool EventManager2::isEmpty() const
	{
		return mListeners.empty();
	}

}
}
