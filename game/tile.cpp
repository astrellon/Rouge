#include "tile.h"

#include <gfx/gfx_engine.h>
using namespace am::gfx;

#include <log/logger.h>

#include <util/utils.h>
using namespace am::util;

#include <sstream>
using namespace std;

#include "tile_set.h"
#include "tile_type.h"
#include "engine.h"

namespace am {
namespace game {

	const int Tile::LUA_ID = 0x0F;
	const char *Tile::LUA_TABLENAME = "am_game_Tile";

	Tile::Tile(const char *name) :
		mName(name),
		mFullName("No full name"),
		mDescription("Undescribable"),
		mGraphic(nullptr),
		mTileSet(nullptr),
		mPrecedence(0)
	{
		//printf("Creating tile '%s'\n", name);
	}
	Tile::Tile(const char *name, const char *fullName) :
		mName(name),
		mFullName(fullName),
		mDescription("Undescribable"),
		mGraphic(nullptr),
		mTileSet(nullptr),
		mPrecedence(0)
	{
	}
	Tile::~Tile()
	{
	}

	void Tile::setName(const char *name)
	{
		if (!name)
		{
			return;
		}
		if (mTileSet)
		{
			mTileSet->removeTile(this);
		}
		mName = name;
		if (mTileSet)
		{
			mTileSet->addTile(this);
		}
	}
	bool Tile::setNameWithCheck(const char *name)
	{
		if (!name)
		{
			return false;
		}
		if (mTileSet && mTileSet->hasTile(name))
		{
			return false;
		}
		setName(name);
		return true;
	}
	string Tile::getName() const
	{
		return mName;
	}

	string Tile::getFullName() const
	{
		return mFullName;
	}
	void Tile::setFullName(const char *name)
	{
		mFullName = name;
	}

	string Tile::getDescription() const
	{
		return mDescription;
	}
	void Tile::setDescription(const char *description)
	{
		mDescription = description;
	}

	Asset *Tile::getGraphicAsset()
	{
		return mGraphic.get();
	}
	void Tile::setGraphicAsset(Asset *asset)
	{
		mGraphic = asset;
	}

	void Tile::setTileSet(TileSet *tileSet)
	{
		if (mTileSet)
		{
			mTileSet->release();
		}
		mTileSet = tileSet;
		if (mTileSet)
		{
			mTileSet->retain();
		}
	}
	TileSet *Tile::getTileSet() const
	{
		return mTileSet;
	}

	void Tile::setPrecedence(int precedence)
	{
		mPrecedence = precedence;
	}
	int Tile::getPrecedence() const
	{
		return mPrecedence;
	}

	void Tile::addTransitionalAsset(Asset *asset, Tile *overlapTile)
	{
		mTransitionalGraphics[overlapTile].push_back(asset);
	}
	void Tile::removeTransitionalAsset(Asset *asset, Tile *overlapTile)
	{
		if (!asset)
		{
			return;
		}
		auto find = mTransitionalGraphics.find(overlapTile);
		if (find == mTransitionalGraphics.end())
		{
			return;
		}
		for (auto iter = find->second.begin(); iter != find->second.end(); ++iter)
		{
			if (iter->get() == asset)
			{
				find->second.erase(iter);
				break;
			}
		}
	}
	void Tile::removeTransitionalAsset(const char *assetName, Tile *overlapTile)
	{
		if (!assetName || assetName[0] == '\0')
		{
			return;
		}
		auto find = mTransitionalGraphics.find(overlapTile);
		if (find == mTransitionalGraphics.end())
		{
			return;
		}
		for (auto iter = find->second.begin(); iter != find->second.end(); ++iter)
		{
			if (strcmp(iter->get()->getName(), assetName) == 0)
			{
				find->second.erase(iter);
				break;
			}
		}
	}
	bool Tile::hasTransitionalAsset(Asset *asset, Tile *overlapTile) const
	{
		if (!asset)
		{
			return false;
		}
		auto find = mTransitionalGraphics.find(overlapTile);
		if (find == mTransitionalGraphics.end())
		{
			return false;
		}
		for (auto iter = find->second.begin(); iter != find->second.end(); ++iter)
		{
			if (iter->get() == asset)
			{
				return true;
			}
		}
		return false;
	}
	bool Tile::hasTransitionalAsset(Tile *overlapTile) const
	{
		return mTransitionalGraphics.find(overlapTile) != mTransitionalGraphics.end();
	}
	const Tile::TileAssetList *Tile::getTransitionalAsset(Tile *overlapTile) const
	{
		auto find = mTransitionalGraphics.find(overlapTile);
		if (find == mTransitionalGraphics.end())
		{
			return nullptr;
		}
		return &find->second;
	}
	const Tile::TileAssetMap &Tile::getAllTransitionalAssets() const
	{
		return mTransitionalGraphics;
	}

	void Tile::loadDef(LuaState &lua)
	{
		if (!lua_istable(lua, -1))
		{
			return;
		}
		if (lua.isTableString("assetName"))
		{
			mGraphic = GfxEngine::getEngine()->getAsset(lua_tostring(lua, -1));
			lua.pop(1);
		}
		else
		{
			stringstream ss;
			ss << "Tile '" << mName << "' did not have a graphic defined";
			am_log("TILE", ss);
		}

		if (lua.isTableString("fullName"))
		{
			mFullName = lua_tostring(lua, -1);
			lua.pop(1);
		}
		if (lua.isTableString("description"))
		{
			mDescription = lua_tostring(lua, -1);
			lua.pop(1);
		}
		if (lua.isTableNumber("precedence"))
		{
			mPrecedence = lua_tointeger(lua, -1);
			lua.pop(1);
		}
		/*if (lua.isTableTable("transitional_graphics"))
		{
			lua_pushnil(lua);
			while (lua_next(lua, -2) != 0)
			{
				if (lua_isstr(lua, -1))
				{

				}
				else
				{
					// Sad times.
				}
				lua.pop(1);
			}
			lua.pop(1);
		}*/
		if (lua.isTableTable("tileTypes"))
		{
			lua_pushnil(lua);
			while (lua_next(lua, -2) != 0)
			{
				if (!lua_isstr(lua, -1))
				{
					stringstream errss;
					errss << "Tile type must be a string and not a '" << lua_typename(lua, -1) << "'";
					am_log("TILE", errss);
				}
				else
				{
					string tileName = Utils::toLowerCase(lua_tostring(lua, -1));
					Handle<TileType> type(Engine::getEngine()->getTileType(tileName.c_str()));
					if (type == nullptr)
					{
						stringstream errss;
						errss << "Unable to find tile type '" << tileName << "' for tile '" << getName() << "'";
						am_log("TILE", errss);
					}
					else
					{
						addTileType(type);
					}
				}
				lua.pop(1);
			}
			lua.pop(1);
		}
	}

	void Tile::addTileType(TileType *tileType)
	{
		if (tileType != nullptr)
		{
			mTileTypes.push_back(tileType);
		}
	}
	void Tile::removeTileType(TileType *tileType)
	{
		if (tileType == nullptr)
		{
			return;
		}
		for (size_t i = 0; i < mTileTypes.size(); i++)
		{
			if (mTileTypes[i].get() == tileType)
			{
				mTileTypes.erase(mTileTypes.begin() + i);
			}
		}
	}
	void Tile::removeAllTileTypes()
	{
		mTileTypes.clear();
	}
	bool Tile::hasTileType(TileType *tileType) const
	{
		if (tileType == nullptr)
		{
			return false;
		}
		for (size_t i = 0; i < mTileTypes.size(); i++)
		{
			if (mTileTypes[i].get() == tileType)
			{
				return true;
			}
		}
		return false;
	}
	Tile::TileTypeList &Tile::getTileTypes()
	{
		return mTileTypes;
	}
	const Tile::TileTypeList &Tile::getTileTypes() const
	{
		return mTileTypes;
	}

}
}
