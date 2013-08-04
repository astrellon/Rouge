#include "map.h"

#include <log/logger.h>

#include <algorithm>

#include <gfx/gfx_engine.h>

#include <util/utils.h>
using namespace am::util;

#include <sstream>
using namespace std;

#include "tile.h"
#include "tile_instance.h"
#include "tile_type.h"
#include "engine.h"
#include "game.h"
#include "pathfinder.h"

namespace am {
namespace game {

	const int Map::LUA_ID = 0x0C;
	const char *Map::LUA_TABLENAME = "am_game_Map";

	Map::Map(const char *name) :
		mName(name),
		mTiles(nullptr),
		mMapData(nullptr),
		mBackground(new Layer()),
		mForeground(new Layer()),
		mMapWidth(0),
		mMapHeight(0),
		mWidth(0.0f),
		mHeight(0.0f)
	{

		mTileRenderer = new TileRenderer(this);
		mTileRenderer->setInteractive(true);

		mBackground->addChild(mTileRenderer);

		//setInteractive(true);
		mBackground->setInteractive(true);
		}
	Map::Map(const char *name, int width, int height) : 
		mName(name),
		mTiles(nullptr),
		mMapData(nullptr),
		mBackground(new Layer()),
		mForeground(new Layer()),
		mMapWidth(0),
		mMapHeight(0),
		mWidth(0.0f),
		mHeight(0.0f)
	{
		setMapSize(width, height);
		mTileRenderer = new TileRenderer(this);
		mTileRenderer->setInteractive(true);

		mBackground->addChild(mTileRenderer);

		//setInteractive(true);
		mBackground->setInteractive(true);
	}
	Map::Map(const Map &rhs) :
		mName(rhs.mName),
		mTiles(nullptr),
		mMapData(nullptr),
		mBackground(rhs.mBackground),
		mForeground(rhs.mForeground),
		mMapWidth(0),
		mMapHeight(0),
		mWidth(0.0f),
		mHeight(0.0f)
	{
		for (int i = 0; i < mBackground->getNumChildren(); i++)
		{
			if (typeid(mBackground->getChildAt(i)) == typeid(TileRenderer))
			{
				mTileRenderer = dynamic_cast<TileRenderer *>(mBackground->getChildAt(i));
				mTileRenderer->setMap(this);
			}
		}
		if (!mTileRenderer)
		{
			mTileRenderer = new TileRenderer(this);
		}
	}
	Map::~Map()
	{
		clear();
	}

	void Map::deinit()
	{
		clear();

		mTileRenderer->setMap(nullptr);
		if (mBackground)
		{
			//mBackground->removeChild(this);
			mBackground->deinit();
			mBackground = nullptr;
		}
		if (mForeground)
		{
			//mForeground->removeChild(this);
			mForeground->deinit();
			mForeground = nullptr;
		}
		mObjects.clear();
	}

	void Map::setName(const char *name)
	{
		mName = name;
	}
	string Map::getName() const
	{
		return mName;
	}

	void Map::setFullName(const char *name)
	{
		mFullName = name;
	}
	string Map::getFullName() const
	{
		return mFullName;
	}
	
	Tile *Map::getTile(int x, int y)
	{
		if (x < 0 || y < 0 || x >= mMapWidth || y >= mMapHeight)
		{
			return nullptr;
		}
		return mTiles[y * mMapWidth + x].getTile();
	}
	TileInstance *Map::getTileInstance(int x, int y)
	{
		if (x < 0 || y < 0 || x >= mMapWidth || y >= mMapHeight)
		{
			return nullptr;
		}
		return &mTiles[y * mMapWidth + x];
	}
	TileInstance *Map::getTiles()
	{
		return mTiles;
	}
	const TileInstance *Map::getTiles() const
	{
		return mTiles;
	}
	
	void Map::setMapSize(int width, int height)
	{
		if (width < 1 || height < 1)
		{
			printf("Map size cannot be %d, %d\n", width, height);
			return;
		}
		clear();
		mTiles = new TileInstance[width * height];
		
		mMapWidth = width;
		mMapHeight = height;

		float gridSize = Engine::getEngine()->getGridSize();

		mWidth = static_cast<float>(width) * gridSize;
		mHeight = static_cast<float>(height) * gridSize;

		mMapData = new AStarNode*[width];
		for(int x = 0; x < width; x++)
		{
			mMapData[x] = new AStarNode[height];
			for(int y = 0; y < height; y++)
			{
				AStarNode &node = mMapData[x][y];
				node.position.x = static_cast<float>(x * gridSize);
				node.position.y = static_cast<float>(y * gridSize);
				node.gridPosition.x = x;
				node.gridPosition.y = y;
			}
		}
	}

	int Map::getMapWidth() const
	{
		return mMapWidth;
	}
	int Map::getMapHeight() const
	{
		return mMapHeight;
	}

	float Map::getWidth() const
	{
		return mWidth;
	}
	float Map::getHeight() const
	{
		return mHeight;
	}

	void Map::clear()
	{
		if (mTiles)
		{
			delete [] mTiles;
			mTiles = nullptr;
		}

		if (mMapData)
		{
			for(int x = 0; x < mMapWidth; x++)
			{
				delete [] mMapData[x];
			}
			delete [] mMapData;
			mMapData = nullptr;
		}
	}

	ObjectList *Map::getObjects()
	{
		return &mObjects;
	}
	bool Map::addGameObject(GameObject *object)
	{
		if (object == nullptr)
		{
			return false;
		}
		ObjectList::const_iterator iter = findGameObject(object);
		if (iter == mObjects.end())
		{
			mObjects.push_back(object);
			Game *game = Engine::getGame();
			if (game && game->getCurrentMap() == this)
			{
				ISource *source = object->getSource(false);
				if (source)
				{
					source->play();
				}
			}
			object->setMap(this);
			return true;
		}
		return false;
	}
	bool Map::removeGameObject(GameObject *object)
	{
		if (object == nullptr)
		{
			return false;
		}
		ObjectList::const_iterator iter = findGameObject(object);
		if (iter != mObjects.end())
		{
			mObjects.erase(iter);
			object->setMap(nullptr);
			return true;
		}
		return false;
	}
	bool Map::hasGameObject(GameObject *object) const
	{
		if (object == nullptr)
		{
			return false;
		}
		ObjectList::const_iterator iter = findGameObject(object);
		return iter != mObjects.end();
	}
	ObjectList::const_iterator Map::findGameObject(GameObject *object) const
	{
		ObjectList::const_iterator iter;
		for (iter = mObjects.begin(); iter != mObjects.end(); ++iter)
		{
			if (iter->get() == object)
			{
				break;
			}
		}
		return iter;
	}

	bool Map::isValidLocation(float x, float y, GameObject *forObject) const
	{
		if (x < 0 || y < 0 || x >= mWidth || y >= mHeight || forObject == nullptr)
		{
			return false;
		}

		float resp = Engine::getEngine()->getGridSizeResp();
		
		int gridXStart = static_cast<int>(x * resp);
		// There is a small amount deducted so that objects which are exactly the size of a tile will have
		// the same start and end grid location and objects that are not the exact size will start and end
		// in the tiles they are supposed to.
		int gridXEnd = static_cast<int>((x + forObject->getWidth()) * resp - 0.0001f);
		if (gridXStart >= mMapWidth || gridXEnd >= mMapWidth)
		{
			return false;
		}

		int gridYStart = static_cast<int>(y * resp);
		// See above for explaination.
		int gridYEnd = static_cast<int>((y + forObject->getHeight()) * resp - 0.0001f);
		if (gridYStart >= mMapHeight || gridYEnd >= mMapHeight)
		{
			return false;
		}

		for (int x = gridXStart; x <= gridXEnd; x++)
		{
			for (int y = gridYStart; y <= gridYEnd; y++)
			{
				if (!_isValidGridLocation(x, y, forObject))
				{
					return false;
				}
			}
		}
		return true;
	}

	bool Map::isValidGridLocation(int gridX, int gridY, const GameObject *forObject) const
	{
		if (gridX < 0 || gridY < 0 || gridX >= mMapWidth || gridY >= mMapHeight || forObject == nullptr)
		{
			return false;
		}
		return _isValidGridLocation(gridX, gridY, forObject);
	}
	bool Map::_isValidGridLocation(int gridX, int gridY, const GameObject *forObject) const
	{
		const GameObject::PassibleTypeList &objectsPassible = forObject->getPassibleTypes();
		const Tile::TileTypeList &tileTypes = mTiles[gridY * mMapWidth + gridX].getTile()->getTileTypes();

		for (size_t t = 0; t < tileTypes.size(); t++)
		{
			const TileType *tileType = tileTypes[t];
			for (size_t o = 0; o < objectsPassible.size(); o++)
			{
				if (objectsPassible[o] == tileType)
				{
					return true;
				}
			}
		}
		return false;
	}

	bool Map::isValidGridLocation(int gridX, int gridY, const vector< Handle<TileType> > &passibles) const
	{
		if (gridX < 0 || gridY < 0 || gridX >= mMapWidth || gridY >= mMapHeight)
		{
			return false;
		}
		const Tile::TileTypeList &tileTypes = mTiles[gridY * mMapWidth + gridX].getTile()->getTileTypes();

		for (size_t t = 0; t < tileTypes.size(); t++)
		{
			const TileType *tileType = tileTypes[t];
			for (size_t o = 0; o < passibles.size(); o++)
			{
				if (passibles[o] == tileType)
				{
					return true;
				}
			}
		}
		return false;
	}
	bool Map::isValidGridLocation(int gridX, int gridY, const TileType *forTileType) const
	{
		if (gridX < 0 || gridY < 0 || gridX >= mMapWidth || gridY >= mMapHeight || forTileType == nullptr)
		{
			return false;
		}

		const Tile::TileTypeList &tileTypes = mTiles[gridY * mMapWidth + gridX].getTile()->getTileTypes();

		for (size_t t = 0; t < tileTypes.size(); t++)
		{
			const TileType *tileType = tileTypes[t];
			if (forTileType == tileType)
			{
				return true;
			}
		}
		return false;
	}

	Layer *Map::getBackground() const
	{
		return mBackground;
	}
	Layer *Map::getForeground() const
	{
		return mForeground;
	}
	TileRenderer *Map::getTileRenderer() const
	{
		return mTileRenderer;
	}

	void Map::setFilename(const char *filename)
	{
		mFilename = filename;
	}
	const char *Map::getFilename() const
	{
		return mFilename.c_str();
	}
	
	bool Map::search(const Vector2i &start, Vector2i end, NodePath &path, const GameObject *forObj)
	{
		Pathfinder *pathfinder = Pathfinder::getPathfinder();
		return pathfinder->search(start, end, path, this, forObj);
	}

}
}
