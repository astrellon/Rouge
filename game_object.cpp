#include "game_object.h"

namespace am {
namespace base {

	GameObject::GameObject() :
		Layer(),
		mGameLocationX(0.0f),
		mGameLocationY(0.0f)
	{

	}
	GameObject::~GameObject()
	{

	}

	void GameObject::setGameLocation(float x, float y, bool setDraw)
	{
		mGameLocationX = x;
		mGameLocationY = y;
		if (setDraw)
		{
			mTransform.setXY(x, y);
		}
		
	}
	float GameObject::getGameLocationX() const
	{
		return mGameLocationX;
	}
	float GameObject::getGameLocationY() const
	{
		return mGameLocationY;
	}

}
}
