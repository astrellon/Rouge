#include "data_event.h"

namespace am {
namespace ui {

	DataEvent::DataEvent(const char *type, am::util::JsonValue data) :
		Event(type),
		mData(data)
	{
	}
	DataEvent::DataEvent(const char *type, EventManager *target, am::util::JsonValue data) :
		Event(type, target),
		mData(data)
	{
	}
	DataEvent::~DataEvent()
	{
	}

	am::util::JsonValue DataEvent::getData() const
	{
		return mData;
	}
}
}
