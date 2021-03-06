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
		return addEventListener(std::string(type), context);
	}
	bool EventInterface::addEventListener(const std::string &type, IEventListener *context)
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
	bool EventInterface::addEventListener(ui::Mouse::EventType type, IEventListener *context)
	{
		return addEventListener(ui::Mouse::EventTypeNames[type], context);
	}
	bool EventInterface::addEventListener(ui::Keyboard::EventType type, IEventListener *context)
	{
		return addEventListener(ui::Keyboard::EventTypeNames[type], context);
	}

	bool EventInterface::removeEventListener(const char *type, IEventListener *context)
	{
		return removeEventListener(std::string(type), context);
	}
	bool EventInterface::removeEventListener(const std::string &type, IEventListener *context)
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
	bool EventInterface::removeEventListener(ui::Mouse::EventType type, IEventListener *context)
	{
		return removeEventListener(ui::Mouse::EventTypeNames[type], context);
	}
	bool EventInterface::removeEventListener(ui::Keyboard::EventType type, IEventListener *context)
	{
		return removeEventListener(ui::Keyboard::EventTypeNames[type], context);
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
			return mManager->hasEventListener(std::string(type));
		}
		return false;
	}
	bool EventInterface::hasEventListener(const std::string &type)
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
