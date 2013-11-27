#include "event_interface.h"

namespace am {
namespace ui {

	EventInterface::EventInterface() :
		mManager(nullptr)
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
		if (mManager == nullptr)
		{
			mManager = new EventManager();
		}
		if (mManager->addEventListener(type, context))
		{
			EventInterface *asInterface = dynamic_cast<EventInterface *>(context);
			if (asInterface)
			{
				context->addListeningTo(asInterface);
			}
			return true;
		}
		return false;
	}
	bool EventInterface::addEventListener(am::ui::Mouse::EventType type, IEventListener *context)
	{
		return addEventListener(am::ui::Mouse::EventTypeNames[type], context);
	}
	bool EventInterface::addEventListener(am::ui::Keyboard::EventType type, IEventListener *context)
	{
		return addEventListener(am::ui::Keyboard::EventTypeNames[type], context);
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
				EventInterface *asInterface = dynamic_cast<EventInterface *>(context);
				if (asInterface)
				{
					context->removeListeningTo(asInterface);
				}
				//context->removeListeningTo(this);
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
	bool EventInterface::removeEventListener(am::ui::Mouse::EventType type, IEventListener *context)
	{
		return removeEventListener(am::ui::Mouse::EventTypeNames[type], context);
	}
	bool EventInterface::removeEventListener(am::ui::Keyboard::EventType type, IEventListener *context)
	{
		return removeEventListener(am::ui::Keyboard::EventTypeNames[type], context);
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
		return mManager != nullptr;
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
			mManager = nullptr;
			delete manager;
		}
	}

}
}
