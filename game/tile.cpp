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

	Tile::Tile(const char *name) :
		mName(name),
		mFullName("No full name"),
		mDescription("Undescribable"),
		mGraphic(NULL),
		mTileSet(NULL)
	{
		//printf("Creating tile '%s'\n", name);
	}
	Tile::Tile(const char *name, const char *fullName) :
		mName(name),
		mFullName(fullName),
		mDescription("Undescribable"),
		mGraphic(NULL),
		mTileSet(NULL)
	{
		//printf("Creating tile '%s', '%s'\n", name, fullName);
	}
	Tile::~Tile()
	{
		//printf("Deleting tile '%s'\n", mFullName.c_str());
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

	/*void Tile::loadDef(JsonValue value) 
	{
		if (value.has("assetName", JV_STR))
		{
			mGraphic = GfxEngine::getEngine()->getAsset(value["assetName"].getCStr());
		}
		else
		{
			stringstream ss;
			ss << "Tile '" << mName << "' did not have a graphic defined";
			am_log("TILE", ss);
		}
		if (value.has("fullName", JV_STR))
		{
			mFullName = value["fullName"].getCStr();
		}
		if (value.has("description", JV_STR))
		{
			mDescription = value["description"].getCStr();
		}
		if (value.has("tileTypes", JV_ARR))
		{
			JsonArray *types = value["tileTypes"].getArr();
			JsonArray::iterator iter;
			for (iter = types->begin(); iter != types->end(); ++iter)
			{
				if (iter->getType() != JV_STR)
				{
					stringstream errss;
					errss << "Tile type must be a string and not a '" << iter->getTypeName() << "'";
					am_log("TILE", errss);
					continue;
				}
				string tileName = Utils::toLowerCase(iter->getCStr());
				TileType *type = TileType::getTileType(tileName.c_str());
				if (type == NULL)
				{
					stringstream errss;
					errss << "Unable to find tile type '" << tileName << "' for tile '" << getName() << "'";
					am_log("TILE", errss);
					continue;
				}
				addTileType(type);
			}
		}
	}*/
	void Tile::loadDef(LuaState &lua)
	{
		if (!lua_istable(lua, -1))
		{
			return;
		}
		if (lua.isTableString("assetName"))
		{
			mGraphic = GfxEngine::getEngine()->getAssetLua(lua_tostring(lua, -1));
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
		if (lua.isTableTable("tileTypes"))
		{
			lua_pushnil(lua);
			while (lua_next(lua, -2) != 0)
			{
				if (lua_type(lua, -1) != LUA_TSTRING)
				{
					stringstream errss;
					errss << "Tile type must be a string and not a '" << lua_typename(lua, -1) << "'";
					am_log("TILE", errss);
				}
				else
				{
					string tileName = Utils::toLowerCase(lua_tostring(lua, -1));
					TileType *type = Engine::getEngine()->getTileType(tileName.c_str());
					if (type == NULL)
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
		if (tileType != NULL)
		{
			mTileTypes.push_back(tileType);
		}
	}
	void Tile::removeTileType(TileType *tileType)
	{
		if (tileType == NULL)
		{
			return;
		}
		for (int i = 0; i < mTileTypes.size(); i++)
		{
			if (mTileTypes[i] == tileType)
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
		if (tileType == NULL)
		{
			return false;
		}
		for (int i = 0; i < mTileTypes.size(); i++)
		{
			if (mTileTypes[i] == tileType)
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
