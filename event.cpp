#include "event.h"

namespace am {
namespace ui {

	Event::Event(const char *type) :
		mType(type),
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
