#include "tile_instance.h"

#include <util/utils.h>
using am::util::Utils;

#include "tile.h"
#include "engine.h"

namespace am {
namespace game {

	TileInstance::TileInstance() : 
		mTile(NULL),
		mTileFrame(0)
	{
	}
	TileInstance::TileInstance(Tile *tile) : 
		mTile(tile),
		mTileFrame(0)
	{
	}
	TileInstance::~TileInstance()
	{
	}
	
	Tile *TileInstance::getTile() {
		return mTile;
	}
	void TileInstance::setTile(Tile *tile) {
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
		if (tile == NULL)
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

}
}
