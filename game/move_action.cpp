#include "move_action.h"

#include <game/character.h>
#include <game/engine.h>

namespace am {
namespace game {

	MoveAction::MoveAction(float destX, float destY) :
		IAction(),
		mDestination(destX, destY)
	{
	}

	MoveAction::MoveAction(Vector2f dest) :
		IAction(),
		mDestination(dest)
	{
	}

	void MoveAction::setDestination(float destX, float destY)
	{
		mDestination.x = destX;
		mDestination.y = destY;
	}
	void MoveAction::setDestination(const Vector2f &dest)
	{
		mDestination = dest;
	}
	Vector2f MoveAction::getDestination() const
	{
		return mDestination;
	}

	const char *MoveAction::getName() const
	{
		return "move";
	}

	bool MoveAction::update(Character *obj, float dt)
	{
		if (!obj || dt == 0.0f)
		{
			mCompleted = true;
			return false;
		}

		Vector2f curr(obj->getLocationX(), obj->getLocationY());
		if (curr.equals(mDestination, 0.0001f))
		{
			// Done!
			mCompleted = true;
			return true;
		}
		Vector2f subbed = curr.sub(mDestination);
		subbed.normalise();
		subbed.x *= Engine::getEngine()->getGridXSize() * 0.5f;
		subbed.y *= Engine::getEngine()->getGridYSize() * 0.5f;
		if (!obj->move(subbed.x, subbed.y))
		{
			mCompleted = true;
			// Unable to move
			return false;
		}
		return true;
	}
	
}
}
