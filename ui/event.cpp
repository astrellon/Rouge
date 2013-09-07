#include "event.h"

#include "event_interface.h"

namespace am {
namespace ui {

	Event::Event() :
		mType(""),
		mEventTarget(nullptr),
		mPropagating(true)
	{
	}
	Event::Event(const char *type) :
		mType(type),
		mEventTarget(nullptr),
		mPropagating(true)
	{

	}
	Event::Event(const char *type, EventInterface *eventTarget) :
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

	EventInterface *Event::getEventTarget() const
	{
		return mEventTarget;
	}
	void Event::setEventTarget(EventInterface *manager)
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
