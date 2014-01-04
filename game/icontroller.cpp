#include "icontroller.h"

namespace am {
namespace game {

	IController::IController() :
		mActive(false),
		mEnabled(true)
	{
	}

	void IController::setActive(bool active)
	{
		mActive = active;
	}
	bool IController::isActive() const
	{
		return mActive;
	}

	void IController::setEnabled(bool enabled)
	{
		mEnabled = enabled;
	}
	bool IController::isEnabled() const
	{
		return mEnabled;
	}

}
}
