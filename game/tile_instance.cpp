#include "tile_instance.h"

#include <util/utils.h>
using am::util::Utils;

#include "tile.h"
#include "engine.h"

namespace am {
namespace game {

	const int TileInstance::LUA_ID = 0x10;
	const char *TileInstance::LUA_TABLENAME = "am_game_TileInstance";

	TileInstance::TileInstance() : 
		mTile(nullptr),
		mTileFrame(0)
	{
		for (int i = 0; i < 8; i++)
		{
			mTileEdgeValues[i] = 0u;
		}
	}
	TileInstance::TileInstance(Tile *tile) : 
		mTile(tile),
		mTileFrame(0)
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
		return mTile;
	}
	void TileInstance::setTile(Tile *tile) 
	{
		mTile = tile;
	}
	bool TileInstance::setTileName(const char *tileName)
	{
		return setTileName(string(tileName));
	}
	bool TileInstance::setTileName(const string &tileName)
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
		setTileFrame(frameValue);
		return true;
	}

	void TileInstance::setTileFrame(int frame)
	{
		mTileFrame = frame;
	}
	int TileInstance::getTileFrame() const
	{
		return mTileFrame;
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

}
}
