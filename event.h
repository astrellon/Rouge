#pragma once

#include <string>

using namespace std;

namespace am {
namespace ui {

	class Event {
	public:
		Event(const char *type);
		virtual ~Event();

		string getType() const;
		
		void stopPropagation();
		bool isPropagating() const;

	protected:
		string mType;
		bool mPropagating;
	};

}
}
