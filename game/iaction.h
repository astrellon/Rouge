#pragma once

#include <base/imanaged.h>

#include <string>

namespace am {
namespace game {

	class Character;

	class IAction : public base::IManaged {
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
