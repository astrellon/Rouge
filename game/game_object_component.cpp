#include "game_object_component.h"

#include <game/engine.h>

namespace am {
namespace game {

	GameObjectComponent::GameObjectComponent(const char *id)
	{
		if (id != NULL && id[0] != '\0')
		{
			mId = id;
		}
		else
		{
			// Create random id.
		}
	}
	GameObjectComponent::~GameObjectComponent()
	{

	}

	void GameObjectComponent::setLocation(float x, float y, bool setDraw)
	{
		mLocationX = x;
		mLocationY = y;
		if (!mMap || (mMap && mMap->isValidLocation(x, y, this)))
		{

		}
		if (setDraw)
		{
			mTransform.setXY(x, y);
		}
		
	}
	float GameObjectComponent::getLocationX() const
	{
		return mLocationX;
	}
	float GameObjectComponent::getLocationY() const
	{
		return mLocationY;
	}

	bool GameObjectComponent::setGridLocation(int x, int y, bool setDraw)
	{
		float flocX = Engine::getEngine()->getGridXSize() * static_cast<float>(x);
		float flocY = Engine::getEngine()->getGridYSize() * static_cast<float>(y);
		int locX = static_cast<int>(flocX);
		int locY = static_cast<int>(flocY);
		if (!mMap || (mMap && mMap->isValidGridLocation(x, y, this)))
		{
			mLocationX = flocX;
			mLocationY = flocY;
			if (setDraw)
			{
				mTransform.setXY(mLocationX, mLocationY);
			}
			return true;
		}
		return false;
	}
	int GameObjectComponent::getGridLocationX() const
	{
		return static_cast<int>(mLocationX * Engine::getEngine()->getGridXSizeResp());
	}
	int GameObjectComponent::getGridLocationY() const
	{
		return static_cast<int>(mLocationY * Engine::getEngine()->getGridYSizeResp());
	}

	void GameObjectComponent::move(float x, float y)
	{
		const Engine *engine = Engine::getEngine();
		int gridX = static_cast<int>(mLocationX * engine->getGridXSizeResp());
		int gridY = static_cast<int>(mLocationY * engine->getGridYSizeResp());
		int newGridX = static_cast<int>((mLocationX + x) * engine->getGridXSizeResp());
		int newGridY = static_cast<int>((mLocationY + y) * engine->getGridYSizeResp());
		float dx = 0.0f;
		float dy = 0.0f;
		if (mMap->isValidGridLocation(gridX, newGridY, this))
		{
			dy = y;
		}
		if (mMap->isValidGridLocation(newGridX, gridY, this))
		{
			dx = x;
		}
		setLocation(mLocationX + dx, mLocationY + dy);
	}

	void GameObjectComponent::moveGrid(int x, int y)
	{
		const Engine *engine = Engine::getEngine();
		int gridX = static_cast<int>(mLocationX * engine->getGridXSizeResp());
		int gridY = static_cast<int>(mLocationY * engine->getGridYSizeResp());
		int newGridX = gridX + x;
		int newGridY = gridY + y;
		float dx = 0.0f;
		float dy = 0.0f;
		if (mMap->isValidGridLocation(gridX, newGridY, this))
		{
			dy = static_cast<float>(y) * engine->getGridYSize();
		}
		if (mMap->isValidGridLocation(newGridX, gridY, this))
		{
			dx = static_cast<float>(x) * engine->getGridXSize();
		}
		if (mMap->isValidGridLocation(newGridX, newGridY, this))
		{
			dy = static_cast<float>(y) * engine->getGridYSize();
			dx = static_cast<float>(x) * engine->getGridXSize();
		}
		else
		{
			dy = 0.0f;
			dx = 0.0f;
		}
		setLocation(mLocationX + dx, mLocationY + dy);
	}

	const char *GameObjectComponent::getId() const
	{
		return mId.c_str();
	}
	void GameObjectComponent::setId(const char *id)
	{
		mId = id;
	}

	const GameObjectComponent::ComponentArray &GameObjectComponent::getComponents() const
	{
		return mComponents;
	}
	Component *GameObjectComponent::getComponent(Component::ComponentKey key) const
	{
		for (size_t i = 0; i < mComponents.size(); ++i)
		{
			if (mComponents[i]->getCompKey() == key)
			{
				return mComponents[i];
			}
		}
		return NULL;
	}
	
}
}
