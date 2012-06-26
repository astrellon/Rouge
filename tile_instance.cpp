#include "tile_instance.h"

#include "tile.h"

namespace am {
namespace base {

	TileInstance::TileInstance() : 
		mTile(NULL)
	{
	}
	TileInstance::TileInstance(Tile *tile) : 
		mTile(tile)
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

}
}
