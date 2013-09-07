#pragma once

#include <string>
using namespace std;

#include <base/imanaged.h>
#include <base/handle.h>
using namespace am::base;

namespace am {
namespace ui {

	class EventInterface;

	class Event : public IManaged {
	public:
		Event(const char *type);
		Event(const char *type, EventInterface *target);
		virtual ~Event();

		string getType() const;
		EventInterface *getEventTarget() const;
		void setEventTarget(EventInterface *manager);
		
		void stopPropagation();
		bool isPropagating() const;

	protected:

		Event();

		string mType;
		bool mPropagating;

		void setType(const char *type);

		Handle<EventInterface> mEventTarget;
	};

}
}
