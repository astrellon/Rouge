#include "tile_set.h"

#include "logger.h"
#include <sstream>

namespace am {
namespace base {

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
				return NULL;
			}
			return iter->second.get();
		}
		return NULL;
	}

	void TileSet::loadDef(JsonValue value)
	{
		if (value.has("fullName", JV_STR))
		{
			mFullName = value["fullName"].getCStr();
		}
		if (value.has("tiles", JV_OBJ))
		{
			JsonObject *tiles = value["tiles"].getObj();
			JsonObject::iterator iter;
			for (iter = tiles->begin(); iter != tiles->end(); ++iter)
			{
				JsonValue tileDef = iter->second;
				if (tileDef.getType() == JV_OBJ)
				{
					Tile *tile = new Tile(iter->first.c_str());
					tile->loadDef(tileDef);
					addTile(tile);
				}
			}
		}
		else
		{
			stringstream ss;
			ss << "Tile set '" << mName << "' definition does not have a set of tiles";
			am_log("SET", ss);
		}
	}

}
}
