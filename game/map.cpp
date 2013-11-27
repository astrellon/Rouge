#include "map.h"

#include <log/logger.h>

#include <algorithm>

#include <gfx/gfx_engine.h>

#include <util/utils.h>
using namespace am::util;

#include <sstream>
#include <ostream>
#include <fstream>

#include "tile.h"
#include "tile_instance.h"
#include "tile_type.h"
#include "engine.h"
#include "game.h"
#include "pathfinder.h"
#include "door.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int Map::LUA_ID = LUA_ID_MAP;
	const char *Map::LUA_TABLENAME = LUA_TABLE_MAP;

	Map::Map(const char *name) :
		mName(name),
		mTiles(nullptr),
		mMapData(nullptr),
		mBackground(new Layer()),
		mForeground(new Layer()),
		mMapWidth(0),
		mMapHeight(0),
		mWidth(0.0f),
		mHeight(0.0f),
		mGamePartof(nullptr)
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
		mHeight(0.0f),
		mGamePartof(nullptr)
	{
		setMapSize(width, height);
		mTileRenderer = new TileRenderer(this);
		mTileRenderer->setInteractive(true);

		mBackground->addChild(mTileRenderer);
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
		mHeight(0.0f),
		mGamePartof(nullptr)
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
			mBackground->deinit();
			mBackground = nullptr;
		}
		if (mForeground)
		{
			mForeground->deinit();
			mForeground = nullptr;
		}
		mObjects.clear();
	}

	void Map::setName(const char *name)
	{
		mName = name;
	}
	std::string Map::getName() const
	{
		return mName;
	}

	void Map::setFullName(const char *name)
	{
		mFullName = name;
	}
	std::string Map::getFullName() const
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
	void Map::calcAllTileEdgeValues() const
	{
		for (int y = 0; y < mMapHeight; y++)
		{
			for (int x = 0; x < mMapWidth; x++)
			{
				calcTileEdgeValues(x, y);
			}
		}
	}
	void Map::calcTileEdgeValuesAround(int x, int y) const
	{
		for (int yy = y - 1; yy <= y + 1; yy++)
		{
			for (int xx = x - 1; xx <= x + 1; xx++)
			{
				calcTileEdgeValues(xx, yy);
			}
		}
	}
	
	void Map::calcTileEdgeValues(int x, int y) const
	{
		if (x < 0 || y < 0 || x >= mMapWidth || y >= mMapHeight)
		{
			return;
		}

		int offsets[8] = {-1 - mMapWidth, -mMapWidth, -mMapWidth + 1, -1, 1, -1 + mMapWidth, mMapWidth, 1 + mMapWidth};
		bool yMax = y >= mMapHeight - 1;
		bool xMax = x >= mMapWidth - 1;
		bool skip[8] = {
			y == 0 | x == 0, y == 0, y == 0 | xMax, 
			x == 0, xMax, 
			x == 0 | yMax, yMax, xMax | yMax};

		int index = y * mMapWidth + x;
		int total = mMapWidth * mMapHeight;
		TileInstance &instance = mTiles[index];
		Tile *tile = instance.getTile();

		Tile *tiles[8];
		for (int i = 0; i < 8; i++)
		{
			tiles[i] = !skip[i] ? mTiles[index + offsets[i]].getTile() : nullptr;
		}

		instance.clearTileEdges();

		// Check top
		if (!skip[1] && canOverlap(tile, tiles[1]))
		{
			int flag = Map::FLAG_T;
			if (!skip[3] && tiles[1] == tiles[3])
			{
				flag |= Map::FLAG_L;
				skip[3] = true;
			}
			if (!skip[4] && tiles[1] == tiles[4])
			{
				flag |= Map::FLAG_R;
				skip[4] = true;
			}
			if (!skip[6] && tiles[1] == tiles[6])
			{
				flag |= Map::FLAG_B;
				skip[6] = true;
			}
			instance.setTileEdgeValue(1, flag);
		}
		// Check left
		if (!skip[3] && canOverlap(tile, tiles[3]))
		{
			int flag = Map::FLAG_L;
			if (!skip[4] && tiles[3] == tiles[4])
			{
				flag |= Map::FLAG_R;
				skip[4] = true;
			}
			if (!skip[6] && tiles[3] == tiles[6])
			{
				flag |= Map::FLAG_B;
				skip[6] = true;
			}
			instance.setTileEdgeValue(3, flag);
		}
		// Check right
		if (!skip[4] && canOverlap(tile, tiles[4]))
		{
			int flag = Map::FLAG_R;
			if (!skip[6] && tiles[4] == tiles[6])
			{
				flag |= Map::FLAG_B;
				skip[6] = true;
			}
			instance.setTileEdgeValue(4, flag);
		}
		// Check bottom
		if (!skip[6] && canOverlap(tile, tiles[6]))
		{
			instance.setTileEdgeValue(6, Map::FLAG_B);
		}

		// Check top left
		if (!skip[0] && canOverlap(tile, tiles[0]))
		{
			int flag = 0;
			if (canOverlap(tiles[1], tiles[0]) && canOverlap(tiles[3], tiles[0]))
			{
				flag = Map::FLAG_TL;
			}
			if (!skip[2] && tiles[0] == tiles[2])
			{
				if (canOverlap(tiles[1], tiles[2]) && canOverlap(tiles[4], tiles[2]))
				{
					flag |= Map::FLAG_TR;
					skip[2] = true;
				}
			}
			if (!skip[5] && tiles[0] == tiles[5])
			{
				if (canOverlap(tiles[3], tiles[5]) && canOverlap(tiles[6], tiles[5]))
				{
					flag |= Map::FLAG_BL;
					skip[5] = true;
				}
			}
			if (!skip[7] && tiles[0] == tiles[7])
			{
				if (canOverlap(tiles[6], tiles[7]) && canOverlap(tiles[4], tiles[7]))
				{
					flag |= Map::FLAG_BR;
					skip[7] = true;
				}
			}
			instance.setTileEdgeValue(0, flag);
		}
		// Check top right
		if (!skip[2] && canOverlap(tile, tiles[2]))
		{
			int flag = 0;
			if (canOverlap(tiles[1], tiles[2]) && canOverlap(tiles[4], tiles[2]))
			{
				flag = Map::FLAG_TR;
			}
			if (!skip[5] && tiles[2] == tiles[5])
			{
				if (canOverlap(tiles[3], tiles[5]) && canOverlap(tiles[6], tiles[5]))
				{
					flag |= Map::FLAG_BL;
					skip[5] = true;
				}
			}
			if (!skip[7] && tiles[2] == tiles[7])
			{
				if (canOverlap(tiles[6], tiles[7]) && canOverlap(tiles[4], tiles[7]))
				{
					flag |= Map::FLAG_BR;
					skip[7] = true;
				}
			}
			instance.setTileEdgeValue(2, flag);
		}
		// Check bottom left
		if (!skip[5] && canOverlap(tile, tiles[5]))
		{
			int flag = 0;
			if (canOverlap(tiles[3], tiles[5]) && canOverlap(tiles[6], tiles[5]))
			{
				flag = Map::FLAG_BL;
			}
			if (!skip[7] && tiles[5] == tiles[7])
			{
				if (canOverlap(tiles[6], tiles[7]) && canOverlap(tiles[4], tiles[7]))
				{
					flag |= Map::FLAG_BR;
					skip[7] = true;
				}
			}
			instance.setTileEdgeValue(5, flag);
		}
		// Check bottom right
		if (!skip[7] && canOverlap(tile, tiles[7]))
		{
			int flag = 0;
			if (canOverlap(tiles[6], tiles[7]) && canOverlap(tiles[4], tiles[7]))
			{
				flag = Map::FLAG_BR;
			}
			instance.setTileEdgeValue(7, flag);
		}
	}
	
	bool Map::canOverlap(int x, int y, Tile *overlapTile) const
	{
		Tile *tile = mTiles[y * mMapWidth + x].getTile();
		if (!overlapTile || !tile)
		{
			return false;
		}
		if (tile == overlapTile)
		{
			return false;
		}

		// The overlapTile tile should overlap onto other tiles.
		// If it does not then return nullptr to indicate that it should
		// not overlap this tile.
		if (tile->getPrecedence() > overlapTile->getPrecedence())
		{
			return false;
		}
		// If the precedence is the same then we'll compare arbitarily
		// by the lexigraphical comparison of their names. This should at least
		// allow one to overlap the other and to do so consistenly.
		if (tile->getPrecedence() == overlapTile->getPrecedence())
		{
			if (tile->getName().compare(overlapTile->getName()) > 0)
			{
				return false;
			}
		}
		return true;
	}
	bool Map::canOverlap(Tile *tile, Tile *overlapTile) const
	{
		if (!overlapTile || !tile || tile == overlapTile)
		{
			return false;
		}

		// The overlapTile tile should overlap onto other tiles.
		// If it does not then return nullptr to indicate that it should
		// not overlap this tile.
		if (tile->getPrecedence() < overlapTile->getPrecedence())
		{
			return true;
		}
		// If the precedence is the same then we'll compare arbitarily
		// by the lexigraphical comparison of their names. This should at least
		// allow one to overlap the other and to do so consistenly.
		if (tile->getPrecedence() == overlapTile->getPrecedence())
		{
			if (tile->getName().compare(overlapTile->getName()) > 0)
			{
				return true;
			}
		}
		return false;
	}
	
	void Map::setMapSize(int width, int height, Tile *defaultTile)
	{
		if (width < 1 || height < 1)
		{
			printf("Map size cannot be %d, %d\n", width, height);
			return;
		}

		int total = width * height;
		TileInstance *tiles = new TileInstance[width * height];
		for (int i = 0; i < total; i++)
		{
			tiles[i].setTile(defaultTile);
			tiles[i].randomiseVaritation();
		}
		
		float gridSize = Engine::getEngine()->getGridSize();

		mWidth = static_cast<float>(width) * gridSize;
		mHeight = static_cast<float>(height) * gridSize;

		AStarNode **nodes = new AStarNode*[width];
		for(int x = 0; x < width; x++)
		{
			nodes[x] = new AStarNode[height];
			for(int y = 0; y < height; y++)
			{
				AStarNode &node = nodes[x][y];
				node.position.x = static_cast<float>(x * gridSize);
				node.position.y = static_cast<float>(y * gridSize);
				node.gridPosition.x = x;
				node.gridPosition.y = y;
			}
		}

		int maxWidth = min(width, mMapWidth);
		int maxHeight = min(height, mMapHeight);
		for (int y = 0; y < maxHeight; y++)
		{
			for (int x = 0; x < maxWidth; x++)
			{
				tiles[y * width + x].setTile(mTiles[y * mMapWidth + x].getTile());
				tiles[y * width + x].setBaseVariation(mTiles[y * mMapWidth + x].getBaseVariation());
			}
		}

		clear();
		mTiles = tiles;
		mMapData = nodes;

		mMapWidth = width;
		mMapHeight = height;

		calcAllTileEdgeValues();
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
			if (mGamePartof && mGamePartof->getCurrentMap() == this)
			{
				mGamePartof->addGameObject(object);
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
		const TileInstance &instance = mTiles[gridY * mMapWidth + gridX];

		// Check any tile types that are set on the specific tile instance.
		const TileInstance::TileTypeList &instanceTypes = instance.getTileTypes();
		if (instanceTypes.size() > 0)
		{
			for (size_t t = 0; t < instanceTypes.size(); t++)
			{
				const TileType *tileType = instanceTypes[t];
				for (size_t o = 0; o < objectsPassible.size(); o++)
				{
					if (objectsPassible[o] == tileType)
					{
						return true;
					}
				}
			}

			// Cannot move because of an instanced effect, possibly a door, lets find out.
			ObjectList result;
			if (forObject->getGameObjectType() == GameObject::CHARACTER && getGameObjectsAt(gridX, gridY, result))
			{
				for (auto iter = result.begin(); iter != result.end(); ++iter)
				{
					if (iter->get()->getGameObjectType() == GameObject::DOOR)
					{
						Door *door = dynamic_cast<Door *>(iter->get());
						if (door && door->canOpenBy(forObject))
						{
							return true;
						}
					}
				}
			}
			return false;
		}
		// Check the tile types set onto the tile itself.
		const Tile *tile = instance.getTile();
		if (!tile)
		{
			return false;
		}
		const Tile::TileTypeSet &tileTypes = tile->getTileTypes();
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

	bool Map::isValidGridLocation(int gridX, int gridY, const std::vector< base::Handle<TileType> > &passibles) const
	{
		if (gridX < 0 || gridY < 0 || gridX >= mMapWidth || gridY >= mMapHeight)
		{
			return false;
		}
		
		const TileInstance &instance = mTiles[gridY * mMapWidth + gridX];
		
		// Check any tile types that are set on the specific tile instance.
		const TileInstance::TileTypeList &instanceTypes = instance.getTileTypes();
		if (instanceTypes.size() > 0)
		{
			for (size_t t = 0; t < instanceTypes.size(); t++)
			{
				const TileType *tileType = instanceTypes[t];
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
		// Check the tile types set onto the tile itself.
		const Tile::TileTypeSet &tileTypes = instance.getTile()->getTileTypes();
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

		const TileInstance &instance = mTiles[gridY * mMapWidth + gridX];
		// Check any tile types that are set on the specific tile instance.
		const TileInstance::TileTypeList &instanceTypes = instance.getTileTypes();
		if (instanceTypes.size() > 0)
		{
			for (size_t t = 0; t < instanceTypes.size(); t++)
			{
				if (forTileType == instanceTypes[t])
				{
					return true;
				}
			}
			return false;
		}
		// Check the tile types set onto the tile itself.
		const Tile::TileTypeSet &tileTypes = instance.getTile()->getTileTypes();
		for (size_t t = 0; t < tileTypes.size(); t++)
		{
			if (forTileType == tileTypes[t])
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

	void Map::setGamePartof(Game *game)
	{
		mGamePartof = game;
	}
	Game *Map::getGamePartof() const
	{
		return mGamePartof;
	}

	bool Map::addMapRegion(MapRegion *region)
	{
		if (region == nullptr)
		{
			return false;
		}
		if (!hasMapRegion(region))
		{
			mMapRegions.push_back(region);
			return true;
		}
		return false;
	}
	bool Map::removeMapRegion(MapRegion *region)
	{
		if (region == nullptr)
		{
			return false;
		}
		for (auto iter = mMapRegions.begin(); iter != mMapRegions.end(); ++iter)
		{
			if (iter->get() == region)
			{
				mMapRegions.erase(iter);
				return true;
			}
		}
		return false;
	}
	bool Map::hasMapRegion(MapRegion *region)
	{
		if (region == nullptr)
		{
			return false;
		}
		for (auto iter = mMapRegions.begin(); iter != mMapRegions.end(); ++iter)
		{
			if (iter->get() == region)
			{
				return true;
			}
		}
		return false;
	}
	const MapRegion::MapRegionList &Map::getMapRegions() const
	{
		return mMapRegions;
	}

	void Map::checkMapRegion(GameObject *obj)
	{
		if (!obj)
		{
			return;
		}
		const MapRegion::MapRegionList &objRegions = obj->getMapRegions();
		for (int index = 0; index < objRegions.size(); index++)
		{
			// Check if no longer in region.
			if (!objRegions[index]->intersectsWith(obj))
			{
				obj->removeFromMapRegion(objRegions[index]);
				index--;
			}
		}
		for (auto iter = mMapRegions.begin(); iter != mMapRegions.end(); ++iter)
		{
			if (iter->get()->intersectsWith(obj))
			{
				obj->addToMapRegion(iter->get());
			}
		}
	}

	bool Map::getGameObjectsAt(float worldX, float worldY, ObjectList &result) const
	{
		float size = Engine::getEngine()->getGridSize();
		if (worldX < 0 || worldY < 0 || worldX >= (mMapWidth * size) || worldY >= (mMapHeight * size))
		{
			return false;
		}

		for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
		{
			GameObject *obj = iter->get();
			int x = obj->getLocationX();
			int y = obj->getLocationY();
			float left = x + obj->getWidth();
			float bottom = y + obj->getHeight();
			if (worldX >= x && worldY >= y && worldX < left && worldY < bottom)
			{
				result.push_back(obj);
			}
		}

		return true;
	}
	bool Map::getGameObjectsAt(int gridX, int gridY, ObjectList &result) const
	{
		if (gridX < 0 || gridY < 0 || gridX >= mMapWidth || gridY >= mMapHeight)
		{
			return false;
		}

		for (auto iter = mObjects.begin(); iter != mObjects.end(); ++iter)
		{
			GameObject *obj = iter->get();
			if (obj->getGridLocationX() == gridX && obj->getGridLocationY() == gridY)
			{
				result.push_back(obj);
			}
		}

		return true;
	}
	
	bool Map::search(const Vector2i &start, Vector2i end, NodePath &path, const GameObject *forObj)
	{
		Pathfinder *pathfinder = Pathfinder::getPathfinder();
		return pathfinder->search(start, end, path, this, forObj);
	}

	base::ReturnCode Map::saveMap(const char *filename) const
	{
		if (!filename || filename[0] == '\0')
		{
			return base::NULL_PARAMETER;
		}

		ofstream ss(filename);
		if (!ss)
		{
			return base::FILE_NOT_FOUND;
		}
		ss << "-- Saved map file: " << mFullName << "\ndo"
			<< "\tlocal map = am.map.new(\"" << mName << "\", " << mMapWidth << ", " << mMapHeight << ")\n"
			<< "\t\t:full_name(\"" << mFullName << "\")\n"

			<< "\tam.engine.clear_using_tile_set()\n"
			<< "\tam.engine.using_tile_set(\"nature:nature\")\n"
			<< "\n\tmap:tiles({\n";

		int i = 0;
		int total = mMapWidth * mMapHeight;
		for (int y = 0; y < mMapHeight; y++)
		{
			ss << "\t\t";
			for (int x = 0; x < mMapWidth; x++)
			{
				ss << '"' << mTiles[i].getTile()->getName() << ":" << mTiles[i].getBaseVariation() << '"';
				i++;
				if (i < total)
				{
					ss << ", ";
				}
			}
			if (y == mMapHeight - 1)
			{
				ss << "})";
			}
			ss << "\n";
		}

		ss << '\n';
		ss << "\treturn map\nend\n";
			
		return base::SUCCESS;
	}

}
}
