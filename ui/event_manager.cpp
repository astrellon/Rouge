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

	bool EventManager::addEventListener(const char *type, IEventListener *context)
	{
		return addEventListener(string(type), context);
	}
	bool EventManager::addEventListener(const string &type, IEventListener *context)
	{
		if (findListener(type, context) == mListeners[type].end())
		{
			mListeners[type].push_back(context);
			return true;
		}
		return false;
	}
	bool EventManager::addEventListener(MouseEventType type, IEventListener *context)
	{
		return addEventListener(MouseEventTypeNames[type], context);
	}
	bool EventManager::addEventListener(KeyboardEventType type, IEventListener *context)
	{
		return addEventListener(KeyboardEventTypeNames[type], context);
	}

	bool EventManager::removeEventListener(const char *type, IEventListener *context)
	{
		return removeEventListener(string(type), context);
	}
	bool EventManager::removeEventListener(const string &type, IEventListener *context)
	{
		ListenerList::const_iterator iter = findListener(type, context);
		if (iter != mListeners[type].end())
		{
			mListeners[type].erase(iter);
			if (mListeners[type].empty())
			{
				mListeners.erase(mListeners.find(type));
				return true;
			}
		}
		return false;
	}
	bool EventManager::removeEventListener(MouseEventType type, IEventListener *context)
	{
		return removeEventListener(MouseEventTypeNames[type], context);
	}
	bool EventManager::removeEventListener(KeyboardEventType type, IEventListener *context)
	{
		return removeEventListener(KeyboardEventTypeNames[type], context);
	}
	void EventManager::removeEventListener(IEventListener *context)
	{
		Listeners::iterator typeIter;
		for (typeIter = mListeners.begin(); typeIter != mListeners.end(); ++typeIter)
		{
			ListenerList &list = typeIter->second;
			size_t size = list.size();
			for (size_t i = 0; i < size; i++)
			{
				if (list[i] == context)
				{
					list.erase(list.begin() + i);
					i--;
					size--;
				}
			}
		}
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
