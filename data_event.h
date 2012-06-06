#pragma once

#include "event.h"

#include "json_value.h"

namespace am {
namespace ui {

	class DataEvent : public Event {
	public:
		DataEvent(const char *type, am::util::JsonValue data);
		DataEvent(const char *type, EventManager *target, am::util::JsonValue data);
		~DataEvent();

		am::util::JsonValue getData() const;

	protected:

		am::util::JsonValue mData;
	};

}
}
