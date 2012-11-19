#include "event.h"

#include "event_manager.h"

namespace am {
namespace ui {

	Event::Event() :
		mType(""),
		mEventTarget(NULL),
		mPropagating(true)
	{
	}
	Event::Event(const char *type) :
		mType(type),
		mEventTarget(NULL),
		mPropagating(true)
	{

	}
	Event::Event(const char *type, IEventManager *eventTarget) :
		mType(type),
		mEventTarget(eventTarget),
		mPropagating(true)
	{

	}

	Event::~Event()
	{

	}

	string Event::getType() const
	{
		return mType;
	}
	void Event::setType(const char *type)
	{
		mType = type;
	}

	IEventManager *Event::getEventTarget() const
	{
		return mEventTarget;
	}
	void Event::setEventTarget(IEventManager *manager)
	{
		mEventTarget = manager;
	}

	void Event::stopPropagation()
	{
		mPropagating = false;
	}
	bool Event::isPropagating() const
	{
		return mPropagating;
	}

}
}
