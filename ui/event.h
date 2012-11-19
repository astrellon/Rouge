#pragma once

#include <string>
using namespace std;

#include "../base/imanaged.h"
using namespace am::base;

namespace am {
namespace ui {

	class IEventManager;

	class Event : public IManaged {
	public:
		Event(const char *type);
		Event(const char *type, IEventManager *target);
		virtual ~Event();

		string getType() const;
		IEventManager *getEventTarget() const;
		void setEventTarget(IEventManager * manager);
		
		void stopPropagation();
		bool isPropagating() const;

	protected:

		Event();

		string mType;
		bool mPropagating;

		void setType(const char *type);

		IEventManager *mEventTarget;
	};

}
}
