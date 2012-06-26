#include "game_object.h"

#include "screen.h"

#include "engine.h"

namespace am {
namespace base {

	GameObject::GameObject() :
		Layer(),
		mLocationX(0.0f),
		mLocationY(0.0f),
		mCameraOffsetX(0.0f),
		mCameraOffsetY(0.0f),
		mFixedToGrid(false),
		mOnlyOnPassable(false),
		mScreen(NULL)
	{
		setName("GameObject");
	}
	GameObject::~GameObject()
	{

	}

	void GameObject::setLocation(float x, float y, bool setDraw)
	{
		mLocationX = x;
		mLocationY = y;
		if (setDraw)
		{
			mTransform.setXY(x, y);
		}
		
	}
	float GameObject::getLocationX() const
	{
		return mLocationX;
	}
	float GameObject::getLocationY() const
	{
		return mLocationY;
	}

	void GameObject::setGridLocation(int x, int y, bool setDraw)
	{
		mLocationX = Engine::getMainEngine()->getGridXSize() * static_cast<float>(x);
		mLocationY = Engine::getMainEngine()->getGridYSize() * static_cast<float>(y);
	}
	int GameObject::getGridLocationX() const
	{
		return static_cast<int>(mLocationX * Engine::getMainEngine()->getGridXSize());
	}
	int GameObject::getGridLocationY() const
	{
		return static_cast<int>(mLocationY * Engine::getMainEngine()->getGridYSize());
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

	void GameObject::setFixedToGrid(bool fixed)
	{
		mFixedToGrid = fixed;
	}
	bool GameObject::isFixedToGrid() const
	{
		return mFixedToGrid;
	}

	void GameObject::setOnlyOnPassable(bool only)
	{
		mOnlyOnPassable = only;
	}
	bool GameObject::isOnlyOnPassable() const
	{
		return mOnlyOnPassable;
	}

}
}
