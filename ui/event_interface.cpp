#include "event_interface.h"

namespace am {
namespace ui {

	EventInterface::EventInterface() :
		mManager(NULL)
	{

	}
	EventInterface::~EventInterface()
	{
		managerDeleted();
	}

	bool EventInterface::addEventListener(const char *type, IEventListener *context)
	{
		return addEventListener(string(type), context);
	}
	bool EventInterface::addEventListener(const string &type, IEventListener *context)
	{
		if (mManager == NULL)
		{
			mManager = new EventManager();
		}
		if (mManager->addEventListener(type, context))
		{
			context->addListeningTo(this);
			return true;
		}
		return false;
	}
	bool EventInterface::addEventListener(MouseEventType type, IEventListener *context)
	{
		return addEventListener(MouseEventTypeNames[type], context);
	}
	bool EventInterface::addEventListener(KeyboardEventType type, IEventListener *context)
	{
		return addEventListener(KeyboardEventTypeNames[type], context);
	}

	bool EventInterface::removeEventListener(const char *type, IEventListener *context)
	{
		return removeEventListener(string(type), context);
	}
	bool EventInterface::removeEventListener(const string &type, IEventListener *context)
	{
		if (mManager)
		{
			bool result = mManager->removeEventListener(type, context);
			if (result)
			{
				context->removeListeningTo(this);
			}
			if (mManager->isEmpty())
			{
				if (mManager->isFiring())
				{
					mManager->flagDeletion(this);
				}
				else
				{
					managerDeleted();
				}
			}
			return result;
		}
		return false;
	}
	bool EventInterface::removeEventListener(MouseEventType type, IEventListener *context)
	{
		return removeEventListener(MouseEventTypeNames[type], context);
	}
	bool EventInterface::removeEventListener(KeyboardEventType type, IEventListener *context)
	{
		return removeEventListener(KeyboardEventTypeNames[type], context);
	}
	void EventInterface::removeEventListener(IEventListener *context)
	{
		if (mManager)
		{
			mManager->removeEventListener(context);
			if (mManager->isEmpty())
			{
				if (mManager->isFiring())
				{
					mManager->flagDeletion(this);
				}
				else
				{                   
					managerDeleted();
				}
			}
		}
	}

	bool EventInterface::hasEventListener(const char *type)
	{
		if (mManager)
		{
			return mManager->hasEventListener(string(type));
		}
		return false;
	}
	bool EventInterface::hasEventListener(const string &type)
	{
		if (mManager)
		{
			return mManager->hasEventListener(type);
		}
		return false;
	}

	bool EventInterface::hasActiveManager() const
	{
		return mManager != NULL;
	}

	void EventInterface::managerDeleted()
	{
		if (mManager)
		{
			EventManager::Listeners::iterator iter;
			for (iter = mManager->mListeners.begin(); iter != mManager->mListeners.end(); ++iter)
			{
				EventManager::ListenerList::iterator listIter;
				EventManager::ListenerList &list = iter->second;
				for (listIter = list.begin(); listIter != list.end(); ++listIter)
				{
					(*listIter)->removeListeningTo(this);
				}
			}

			EventManager *manager = mManager;
			mManager = NULL;
			delete manager;
		}
	}

}
}
