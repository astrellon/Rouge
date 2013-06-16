#include "icontroller.h"

namespace am {
namespace game {

	IController::IController() :
		mActive(false)
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

}
}
