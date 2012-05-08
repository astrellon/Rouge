#pragma once

namespace am {
namespace base {

class TileInstance;

class Map {
public:
	Map(int width, int height);
	~Map();
	
	TileInstance *getTile(int x, int y);
	TileInstance *getTiles();
	
	void setSize(int width, int height);
protected:

	TileInstance *mTiles;
	int mWidth;
	int mHeight;
	
	void clear();
};

}
}

#ifdef _ANDROID
#	include "map.cpp"
#endif
