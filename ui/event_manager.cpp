#include "event_manager.h"

#include "mouse_event.h"
#include "ievent_listener.h"
#include "event_interface.h"

namespace am {
namespace ui {

	EventManager::EventManager() :
		mFiring(false),
		mDeleteResponse(nullptr)
	{

	}
	EventManager::~EventManager()
	{
		
	}

	bool EventManager::addEventListener(const char *type, IEventListener *context)
	{
		return addEventListener(std::string(type), context);
	}
	bool EventManager::addEventListener(const std::string &type, IEventListener *context)
	{
		if (findListener(type, context) == mListeners[type].end())
		{
			mListeners[type].push_back(context);
			return true;
		}
		return false;
	}
	bool EventManager::addEventListener(ui::Mouse::EventType type, IEventListener *context)
	{
		return addEventListener(ui::Mouse::EventTypeNames[type], context);
	}
	bool EventManager::addEventListener(ui::Keyboard::EventType type, IEventListener *context)
	{
		return addEventListener(ui::Keyboard::EventTypeNames[type], context);
	}

	bool EventManager::removeEventListener(const char *type, IEventListener *context)
	{
		return removeEventListener(std::string(type), context);
	}
	bool EventManager::removeEventListener(const std::string &type, IEventListener *context)
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
	bool EventManager::removeEventListener(ui::Mouse::EventType type, IEventListener *context)
	{
		return removeEventListener(ui::Mouse::EventTypeNames[type], context);
	}
	bool EventManager::removeEventListener(ui::Keyboard::EventType type, IEventListener *context)
	{
		return removeEventListener(ui::Keyboard::EventTypeNames[type], context);
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
				if (list[i]->compareListeners(context))
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
		return hasEventListener(std::string(type));
	}
	bool EventManager::hasEventListener(const std::string &type)
	{
		return mListeners.find(type) != mListeners.end();
	}

	EventManager::ListenerList::iterator EventManager::findListener(const std::string &type, IEventListener *context)
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
