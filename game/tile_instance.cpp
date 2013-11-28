#include "tile_instance.h"

#include <util/utils.h>
using am::util::Utils;

#include "tile.h"
#include "engine.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int TileInstance::LUA_ID = LUA_ID_TILEINSTANCE;
	const char *TileInstance::LUA_TABLENAME = LUA_TABLE_TILEINSTANCE;

	TileInstance::TileInstance() : 
		mTile(nullptr),
		mBaseVariation(0),
		mTransitionVariation(0)
	{
		for (int i = 0; i < 8; i++)
		{
			mTileEdgeValues[i] = 0u;
		}
	}
	TileInstance::TileInstance(Tile *tile) : 
		mTile(tile),
		mBaseVariation(0),
		mTransitionVariation(0)
	{
		for (int i = 0; i < 8; i++)
		{
			mTileEdgeValues[i] = 0u;
		}
	}
	TileInstance::~TileInstance()
	{
	}
	
	Tile *TileInstance::getTile() 
	{
		if (mTile)
		{
			return mTile;
		}
		return Tile::getDefaultTile();
	}
	const Tile *TileInstance::getTile() const
	{
		if (mTile)
		{
			return mTile;
		}
		return Tile::getDefaultTile();
	}
	void TileInstance::setTile(Tile *tile) 
	{
		mTile = tile;
	}
	bool TileInstance::setTileName(const char *tileName)
	{
		return setTileName(std::string(tileName));
	}
	bool TileInstance::setTileName(const std::string &tileName)
	{
		int framePos = static_cast<int>(tileName.find_last_of(":"));
		string tileNameUse;
		int frameValue = 0;
		if (framePos >= 0)
		{
			string name = tileName.substr(0, framePos);
			string frame = tileName.substr(framePos + 1);
			tileNameUse = name;
			bool parseResult = Utils::fromString<int>(frameValue, frame);
			if (!parseResult)
			{
				frameValue = 0;
			}
		}
		else
		{
			tileNameUse = tileName;
		}
		Tile *tile = Engine::getEngine()->getTile(tileNameUse.c_str());
		if (tile == nullptr)
		{
			return false;
		}
		setTile(tile);
		setBaseVariation(frameValue);
		return true;
	}

	void TileInstance::setBaseVariation(int frame)
	{
		mBaseVariation = frame;
	}
	int TileInstance::getBaseVariation() const
	{
		return mBaseVariation;
	}
	void TileInstance::randomiseVaritation()
	{
		Asset *asset = mTile ? mTile->getGraphicAsset() : nullptr;
		if (!asset)
		{
			return;
		}
		if (asset->getFrameRate() <= 0.0f)
		{
			if (asset->isSubWindowAnimation())
			{
				setBaseVariation(Utils::rand(0, asset->getTotalSubWindows()));
			}
			else
			{
				setBaseVariation(Utils::rand(0, asset->getTotalTextures()));
			}
		}
		else
		{
			setBaseVariation(0);
		}
	}

	void TileInstance::setTransitionVariation(int frame)
	{
		mTransitionVariation = frame;
	}
	int TileInstance::getTransitionVariation() const
	{
		return mTransitionVariation;
	}

	bool TileInstance::hasEdgeValue() const
	{
		for (int i = 0; i < 8; i++)
		{
			if (mTileEdgeValues[i] != 0)
			{
				return true;
			}
		}
		return false;
	}

	void TileInstance::clearTileEdges()
	{
		for (int i = 0; i < 8; i++)
		{
			mTileEdgeValues[i] = 0u;
		}
	}

	void TileInstance::setTileEdgeValue(uint32_t index, uint8_t edgeValue)
	{
		if (index < 8)
		{
			mTileEdgeValues[index] = edgeValue;
		}
	}
	uint8_t TileInstance::getTileEdgeValue(uint32_t index) const
	{
		if (index < 8)
		{
			return mTileEdgeValues[index];
		}
		return 0u;
	}

	void TileInstance::addTileType(TileType *tileType)
	{
		if (tileType != nullptr)
		{
			mTileTypes.push_back(tileType);
		}
	}
	void TileInstance::removeTileType(TileType *tileType)
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
	void TileInstance::removeAllTileTypes()
	{
		mTileTypes.clear();
	}
	bool TileInstance::hasTileType(TileType *tileType) const
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
	TileInstance::TileTypeList &TileInstance::getTileTypes()
	{
		return mTileTypes;
	}
	const TileInstance::TileTypeList &TileInstance::getTileTypes() const
	{
		return mTileTypes;
	}

}
}
