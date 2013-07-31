#include "map.h"

#include <log/logger.h>

#include <algorithm>

#include <gl.h>
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
		mEnabledMapCulling(true),
		mMapData(nullptr)
	{
		mBackground = new Layer();
		mBackground->addChild(this);
		mForeground = new Layer();

		setInteractive(true);
		mBackground->setInteractive(true);
		}
	Map::Map(const char *name, int width, int height) : 
		mName(name),
		mTiles(nullptr),
		mEnabledMapCulling(true),
		mMapData(nullptr)
	{
		setMapSize(width, height);
		mBackground = new Layer();
		mBackground->addChild(this);
		mForeground = new Layer();

		setInteractive(true);
		mBackground->setInteractive(true);
	}
	Map::~Map()
	{
		clear();
	}

	void Map::deinit()
	{
		clear();
		if (mBackground.get())
		{
			mBackground->removeChild(this);
			mBackground->deinit();
			mBackground = nullptr;
		}
		if (mForeground.get())
		{
			mForeground->removeChild(this);
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

	Layer *Map::getBackground()
	{
		return mBackground.get();
	}
	Layer *Map::getForeground()
	{
		return mForeground.get();
	}

	void Map::setFilename(const char *filename)
	{
		mFilename = filename;
	}
	const char *Map::getFilename() const
	{
		return mFilename.c_str();
	}
	/*
	void Map::loadDef(LuaState &lua)
	{
		if (!lua_istable(lua, -1))
		{
			stringstream errss;
			errss << "Unable to load map def as top element is: " << lua_typename(lua, -1);
			am_log("MAP", errss);
			return;
		}
		if (lua.isTableString("fullName"))
		{
			setFullName(lua_tostring(lua, -1));
			lua.pop(1);
		}
		Engine *engine = Engine::getEngine();
		engine->clearUsingTileSet();

		if (lua.isTableTable("usingTileSets"))
		{
			lua_pushnil(lua);
			while (lua_next(lua, -2) != 0)
			{
				if (lua_type(lua, -1) == LUA_TSTRING)
				{
					engine->usingTileSet(lua_tostring(lua, -1));
				}
				lua.pop(1);
			}
			lua.pop(1);
		}
		else if (lua.isTableString("usingTileSets"))
		{
			engine->usingTileSet(lua_tostring(lua, -1));
			lua.pop(1);
		}

		if (lua.isTableTable("tiles"))
		{
			lua_len(lua, -1);
			int height = lua_tointeger(lua, -1);
			lua.pop(1);
			int width = 0;
			bool error = false;

			int x = 0, y = 0;

			lua_pushnil(lua);
			while (lua_next(lua, -2) != 0)
			{
				if (lua_istable(lua, -1))
				{
					lua_len(lua, -1);
					if (width == 0)
					{
						width = lua_tointeger(lua, -1);
						setMapSize(width, height);
					}
					lua.pop(1);
					lua_pushnil(lua);
					x = 0;
					while (lua_next(lua, -2) != 0)
					{
						if (lua_isstring(lua, -1))
						{
							string tileNameStr = lua_tostring(lua, -1);
							//string tileNameStr(lua_tostring(lua, -1));
							int framePos = static_cast<int>(tileNameStr.find_last_of(":"));
							string tileNameUse;
							int frameValue = 0;
							if (framePos >= 0)
							{
								string name = tileNameStr.substr(0, framePos);
								string frame = tileNameStr.substr(framePos + 1);
								tileNameUse = name;
								bool parseResult = Utils::fromString<int>(frameValue, frame);
								if (!parseResult)
								{
									frameValue = 0;
								}
							}
							else
							{
								tileNameUse = tileNameStr;
							}
							Tile *tile = engine->getTile(tileNameUse.c_str());
							if (tile == nullptr)
							{
								stringstream ss;
								ss << "Unable to find tile '" << tileNameUse.c_str();
								ss << "' for map '" << mName << "' tile " << x << ", " << y;
								am_log("MAP", ss);
								error =	true;
								break;
							}
							mTiles[y * width + x].setTile(tile);
							mTiles[y * width + x].setTileFrame(frameValue);
						}
						x++;
						lua.pop(1);
					}
				}
				y++;
				lua.pop(1);
			}
			if (!error)
			{
				updateAssetSprites();
			}

			lua.pop(1);
		}
	}*/

	void Map::updateAssetSprites()
	{
		mAssetSprites.clear();

		int numTiles = mMapWidth * mMapHeight;
		float grid = Engine::getEngine()->getGridSize();

		for (int i = 0; i < numTiles; i++)
		{
			Asset *asset = mTiles[i].getTile()->getGraphicAsset();
			if (asset)
			{
				AssetSpriteMap::iterator iter = mAssetSprites.find(asset);
				if (iter == mAssetSprites.end())
				{
					Sprite *assetSprite = new Sprite(asset);
					mAssetSprites[asset] = assetSprite;
					assetSprite->setWidth(grid);
					assetSprite->setHeight(grid);
				}
			}
		}
	}

	void Map::render(float dt)
	{
		AssetSpriteMap::iterator iter;
		for (iter = mAssetSprites.begin(); iter != mAssetSprites.end(); ++iter)
		{
			iter->second->updateSprite(dt);
		}

		float grid = Engine::getEngine()->getGridSize();
		float gridResp = Engine::getEngine()->getGridSizeResp();

		GfxEngine *gfxEngine = GfxEngine::getEngine();
		float cameraX = gfxEngine->getCameraX();
		float cameraY = gfxEngine->getCameraY();

		float screenWidth = static_cast<float>(gfxEngine->getScreenWidth());
		float screenHeight = static_cast<float>(gfxEngine->getScreenHeight());

		int minX = 0;
		int maxX = mMapWidth;
		int minY = 0;
		int maxY = mMapHeight;

		if (mEnabledMapCulling)
		{
			int camMinX = static_cast<int>((cameraX - screenWidth * 0.5f) * gridResp);
			minX = max(0, camMinX);
			int camMaxX = static_cast<int>((cameraX + screenWidth * 0.5f) * gridResp) + 1;
			maxX = min(mMapWidth, camMaxX);

			int camMinY = static_cast<int>((cameraY - screenHeight * 0.5f) * gridResp);
			minY = max(0, camMinY);
			int camMaxY = static_cast<int>((cameraY + screenHeight * 0.5f) * gridResp) + 1;
			maxY = min(mMapHeight, camMaxY);
		}
		glPushMatrix();
		int t = minY * mMapWidth + minX;
		int tStep = mMapWidth - (maxX - minX);

		float resetX = -(maxX - minX) * grid;
		glTranslatef(minX * grid, minY * grid, 0.0f);
		for (int y = minY; y < maxY; y++)
		{
			t = y * mMapWidth + minX;
			for (int x = minX; x < maxX; x++)
			{
				Asset *asset = mTiles[t].getTile()->getGraphicAsset();
				Sprite *sprite = mAssetSprites[asset].get();
				if (asset->getFrameRate() <= 0.0f)
				{
					sprite->setCurrentFrame(mTiles[t].getTileFrame());
				}
				sprite->renderSprite();
				t++;

				glTranslatef(grid, 0.0f, 0.0f);
			}
			//t += tStep;
			glTranslatef(resetX, grid, 0.0f);
		}
		glPopMatrix();
	}

	bool Map::search(const Vector2i &start, Vector2i end, NodePath &path, const GameObject *forObj)
	{
		Pathfinder *pathfinder = Pathfinder::getPathfinder();
		return pathfinder->search(start, end, path, this, forObj);
	}

}
}
