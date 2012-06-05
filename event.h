#pragma once

#include <string>

using namespace std;

namespace am {
namespace ui {

	class EventManager;

	class Event {
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
