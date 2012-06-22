#include "game_object.h"

#include "screen.h"

namespace am {
namespace base {

	GameObject::GameObject() :
		Layer(),
		mGameLocationX(0.0f),
		mGameLocationY(0.0f),
		mCameraOffsetX(0.0f),
		mCameraOffsetY(0.0f),
		mScreen(NULL)
	{
		setName("GameObject");
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

	void GameObject::setCameraOffset(float x, float y)
	{
		mCameraOffsetX = x;
		mCameraOffsetY = y;
	}
	float GameObject::getCameraOffsetX() const
	{
		return mCameraOffsetX;
	}
	float GameObject::getCameraOffsetY() const
	{
		return mCameraOffsetY;
	}

	void GameObject::setScreen(Screen *screen)
	{
		if (mScreen)
		{
			mScreen->release();
		}
		mScreen = screen;
		if (mScreen)
		{
			mScreen->retain();
		}
	}
	Screen *GameObject::getScreen()
	{
		return mScreen;
	}

}
}
