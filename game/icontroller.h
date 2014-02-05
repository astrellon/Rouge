#pragma once

#include <base/imanaged.h>

#include "unique_id.h"

namespace am {
namespace game {

	class Character;

	class IController : public base::IManaged, public UniqueId
	{
	public:

		IController();

		// Happens on every game update.
		virtual void update(Character *character, float dt) = 0;
		// Happens on every game tick.
		virtual void onGameTick(Character *character, float dt) = 0;

		virtual void detach() = 0;

		// When true the controller is currently executing. This can happen over multiple
		// game updates and game ticks potentially.
		virtual void setActive(bool active);
		virtual bool isActive() const;

		// When true the controller is enabled, when not enabled it will be skipped completely.
		virtual void setEnabled(bool enabled);
		virtual bool isEnabled() const;

	protected:
		bool mActive;
		bool mEnabled;

	};

}
}
