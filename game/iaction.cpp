#include "iaction.h"

namespace am {
namespace game {

	IAction::IAction() :
		mCompleted(false)
	{
	}

	void IAction::stop()
	{
		mCompleted = true;
	}
	bool IAction::hasCompleted() const
	{
		return mCompleted;
	}
	
}
}
