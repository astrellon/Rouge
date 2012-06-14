#pragma once

#include <string>
using namespace std;

#include "imanaged.h"
using namespace am::util;

namespace am {
namespace ui {

	class EventManager;

	class Event : public IManaged {
	public:
		Event(const char *type);
		Event(const char *type, EventManager *target);
		virtual ~Event();

		string getType() const;
		EventManager *getEventTarget();
		
		void stopPropagation();
		bool isPropagating() const;

	protected:

		Event();

		string mType;
		bool mPropagating;

		void setType(const char *type);

		EventManager *mEventTarget;
	};

}
}
