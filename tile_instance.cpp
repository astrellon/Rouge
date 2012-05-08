#ifndef _ANDROID
#	include "tile_instance.h"
#endif

#include "tile.h"

namespace am {
namespace base {

TileInstance::TileInstance() : 
	mTile(NULL), 
	mXpos(0), 
	mYpos(0) 
{
}
TileInstance::TileInstance(Tile *tile) : 
	mTile(tile), 
	mXpos(0), 
	mYpos(0) 
{
}
TileInstance::TileInstance(Tile *tile, int x, int y) : 
	mTile(tile), 
	mXpos(x), 
	mYpos(y) 
{
}
TileInstance::~TileInstance() {
}
	
int TileInstance::getX() const {
	return mXpos;
}
int TileInstance::getY() const {
	return mYpos;
}
	
void TileInstance::setPos(int x, int y) {
	mXpos = x;
	mYpos = y;
}
	
Tile *TileInstance::getTile() {
	return mTile;
}
void TileInstance::setTile(Tile *tile) {
	mTile = tile;
}

}
}
