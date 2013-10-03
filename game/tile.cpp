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

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	Handle<Tile> Tile::sDefaultTile;

	const int Tile::LUA_ID = LUA_ID_TILE;
	const char *Tile::LUA_TABLENAME = LUA_TABLE_TILE;

	Tile::Tile(const char *name) :
		mName(name),
		mFullName("No full name"),
		mDescription("describable"),
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

	string Tile::getNameWithSet() const
	{
		if (mTileSet)
		{
			return mTileSet->getName() + ":" + mName;
		}
		return mName;
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
	bool Tile::hasTransitionalAsset(const char *assetName, Tile *overlapTile) const
	{
		if (!assetName || assetName[0] == '\0')
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
			if (strcmp(iter->get()->getLoadedName(), assetName) == 0)
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

	void Tile::addTileType(TileType *tileType)
	{
		if (tileType != nullptr && !hasTileType(tileType))
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
	Tile::TileTypeSet &Tile::getTileTypes()
	{
		return mTileTypes;
	}
	const Tile::TileTypeSet &Tile::getTileTypes() const
	{
		return mTileTypes;
	}

	void Tile::setDefaultTile(Tile *tile)
	{
		sDefaultTile = tile;
	}
	Tile *Tile::getDefaultTile()
	{
		return sDefaultTile;
	}

}
}
