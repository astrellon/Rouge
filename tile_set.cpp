#include "tile_set.h"

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
	string TileSet::getName()
	{
		return mName;
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

	}

}
}
