#pragma once

#include <base/imanaged.h>
using namespace am::base;

#include <string>
using namespace std;

namespace am {
namespace game {

	class Character;

	class IAction : public IManaged {
	public:
		
		IAction();

		virtual const char *getName() const = 0;

		virtual bool update(Character *obj, float dt) = 0;
		virtual void stop();

		virtual bool hasCompleted() const;

	protected:

		bool mCompleted;

	};

}
}
