#pragma once

#include "event.h"

namespace am {
namespace ui {

	class IEventListener;

	class IEventManager {
	public:
		virtual void addEventListener(const char *type, IEventListener *content) = 0;
		virtual void addEventListener(const string &type, IEventListener *content) = 0;
		virtual void removeEventListener(const char *type, IEventListener *content) = 0;
		virtual void removeEventListener(const string &type, IEventListener *content) = 0;
		virtual bool hasEventListener(const char *type) = 0;
		virtual bool hasEventListener(const string &type) = 0;

		virtual void fireEvent(Event &e) = 0;
	};

}
}
