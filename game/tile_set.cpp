#include "tile_set.h"

#include <log/logger.h>
#include <sstream>

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int TileSet::LUA_ID = LUA_ID_TILESET;
	const char *TileSet::LUA_TABLENAME = LUA_TABLE_TILESET;

	TileSet::TileSet()
	{
	}
	TileSet::TileSet(const char *name) :
		mName(name)
	{
	}
	TileSet::~TileSet()
	{
	}

	void TileSet::setName(const char *name)
	{
		mName = name;
	}
	string TileSet::getName() const
	{
		return mName;
	}

	void TileSet::setFullName(const char *name)
	{
		mFullName = name;
	}
	string TileSet::getFullName() const
	{
		return mFullName;
	}

	void TileSet::addTile(Tile *tile)
	{
		if (tile)
		{
			mTiles[tile->getName()] = tile;
		}
	}
	bool TileSet::addTileWithCheck(Tile *tile)
	{
		if (tile && mTiles.find(tile->getName()) == mTiles.end())
		{
			mTiles[tile->getName()] = tile;
			return true;
		}
		return false;
	}
	void TileSet::removeTile(Tile *tile)
	{
		if (tile)
		{
			removeTile(tile->getName().c_str());
		}
	}
	void TileSet::removeTile(const char *tileName)
	{
		if (tileName)
		{
			TileMap::iterator iter = mTiles.find(string(tileName));
			if (iter != mTiles.end())
			{
				mTiles.erase(iter);
			}
		}
	}
	bool TileSet::hasTile(Tile *tile)
	{
		if (tile)
		{
			return mTiles.find(string(tile->getName())) != mTiles.end();
		}
		return false;
	}
	bool TileSet::hasTile(const char *tileName)
	{
		if (tileName)
		{
			return mTiles.find(string(tileName)) != mTiles.end();
		}
		return false;
	}
	Tile *TileSet::getTile(const char *tileName)
	{
		if (tileName)
		{
			TileMap::iterator iter = mTiles.find(string(tileName));
			if (iter == mTiles.end())
			{
				return nullptr;
			}
			return iter->second.get();
		}
		return nullptr;
	}
	const TileSet::TileMap &TileSet::getTiles() const
	{
		return mTiles;
	}

}
}
