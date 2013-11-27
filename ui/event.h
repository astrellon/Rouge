#pragma once

#include <string>

#include <base/imanaged.h>
#include <base/handle.h>

namespace am {
namespace ui {

	class EventInterface;

	class Event : public am::base::IManaged {
	public:
		Event(const char *type);
		Event(const char *type, EventInterface *target);
		virtual ~Event();

		std::string getType() const;
		EventInterface *getEventTarget() const;
		void setEventTarget(EventInterface *manager);
		
		void stopPropagation();
		bool isPropagating() const;

	protected:

		Event();

		std::string mType;
		bool mPropagating;

		void setType(const char *type);

		am::base::Handle<EventInterface> mEventTarget;
	};

}
}
