#include "event_interface.h"

namespace am {
namespace ui {

	EventInterface::EventInterface() :
		mManager(NULL)
	{

	}
	EventInterface::~EventInterface()
	{
		if (mManager)
		{
			delete mManager;
			mManager = NULL;
		}
	}

	void EventInterface::addEventListener(const char *type, IEventListener *context)
	{
		addEventListener(string(type), context);
	}
	void EventInterface::addEventListener(const string &type, IEventListener *context)
	{
		if (mManager == NULL)
		{
			mManager = new EventManager();
		}
		mManager->addEventListener(type, context);
	}
	void EventInterface::addEventListener(MouseEventType type, IEventListener *content)
	{
		addEventListener(MouseEventTypeNames[type], content);
	}
	void EventInterface::addEventListener(KeyboardEventType type, IEventListener *content)
	{
		addEventListener(KeyboardEventTypeNames[type], content);
	}

	void EventInterface::removeEventListener(const char *type, IEventListener *context)
	{
		removeEventListener(string(type), context);
	}
	void EventInterface::removeEventListener(const string &type, IEventListener *context)
	{
		if (mManager)
		{
			mManager->removeEventListener(type, context);
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
	void EventInterface::removeEventListener(MouseEventType type, IEventListener *content)
	{
		removeEventListener(MouseEventTypeNames[type], content);
	}
	void EventInterface::removeEventListener(KeyboardEventType type, IEventListener *content)
	{
		removeEventListener(KeyboardEventTypeNames[type], content);
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
		EventManager *manager = mManager;
		mManager = NULL;
		delete manager;
	}

}
}
