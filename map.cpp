#ifndef _ANDROID
#	include "map.h"
#endif

#include "tile.h"
#include "tile_instance.h"

namespace am {
namespace base {

Map::Map(int width, int height) : 
	mTiles(NULL) 
{
	setSize(width, height);
}
Map::~Map() {
	clear();
}
	
TileInstance *Map::getTile(int x, int y) {
	if (x < 0 || y < 0 || x >= mWidth || y >= mHeight) {
		return NULL;
	}
	return &mTiles[y * mWidth + x];
}
TileInstance *Map::getTiles() {
	return mTiles;
}
	
void Map::setSize(int width, int height) {
	if (width < 1 || height < 1) {
		printf("Map size cannot be %d, %d\n", width, height);
		return;
	}
	clear();
	mTiles = new TileInstance[width * height];
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			TileInstance &tile = mTiles[y * width + y];
			tile.setPos(x, y);
		}
	}
	mWidth = width;
	mHeight = height;
}

void Map::clear() {
	if (mTiles) {
		delete [] mTiles;
	}
}

}
}
