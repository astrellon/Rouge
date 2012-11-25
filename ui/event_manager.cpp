#include "event_manager.h"

#include "mouse_event.h"
#include "ievent_listener.h"
#include "event_interface.h"

namespace am {
namespace ui {

	EventManager::EventManager() :
		mFiring(false),
		mDeleteResponse(NULL)
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
	void EventManager::addEventListener(MouseEventType type, IEventListener *content)
	{
		addEventListener(MouseEventTypeNames[type], content);
	}
	void EventManager::addEventListener(KeyboardEventType type, IEventListener *content)
	{
		addEventListener(KeyboardEventTypeNames[type], content);
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
			mListeners[type].erase(iter);
			if (mListeners[type].empty())
			{
				mListeners.erase(mListeners.find(type));
			}
		}
	}
	void EventManager::removeEventListener(MouseEventType type, IEventListener *content)
	{
		removeEventListener(MouseEventTypeNames[type], content);
	}
	void EventManager::removeEventListener(KeyboardEventType type, IEventListener *content)
	{
		removeEventListener(KeyboardEventTypeNames[type], content);
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
			if (context->compareListeners(*iter))
			{
				break;
			}
		}
		return iter;
	}

	bool EventManager::isEmpty() const
	{
		return mListeners.empty();
	}
	bool EventManager::isFiring() const
	{
		return mFiring;
	}

	void EventManager::flagDeletion(EventInterface *response)
	{
		mDeleteResponse = response;
	}

	bool EventManager::removeToRemove()
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
	void EventManager::checkDeletion()
	{
		if (mDeleteResponse)
		{
			mDeleteResponse->managerDeleted();
		}
	}

}
}
