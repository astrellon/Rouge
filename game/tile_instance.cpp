#include "tile_instance.h"

#include "tile.h"

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
