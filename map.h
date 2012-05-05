#pragma once

class NearbyTiles {
public:
	NearbyTiles() {
		for (int i = 0; i < 8; i++) {
			tiles = NULL;
		}
	}
	TileInstance *[8] tiles;
};

class Map {
public:
	Map(int width, int height) : mTiles(NULL) {
		setSize(width, height);
	}
	~Map() {
		clear();
	}
	
	TileInstance *getTile(int x, int y) {
		if (x < 0 || y < 0 || x >= mWidth || y >= mHeight) {
			return NULL;
		}
		return mTiles[y * mWidth + x];
	}
	TileInstance *getTiles() {
		return mTiles;
	}
	
	void setSize(int width, int height) {
		if (width < 1 || height < 1) {
			printf("Map size cannot be %d, %d\n", width, height);
			return;
		}
		clear();
		mTiles = new TileInstance[width * height];
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				Tile &tile = mTiles[y * width + y]
				tile.setPos(x, y);
				tile.setTile(NULL);
			}
		}
		mWidth = width;
		mHeight = height;
	}
protected:

	TileInstance *mTiles;
	int mWidth;
	int mHeight;
	
	void clear() {
		if (mTiles) {
			delete [] mTiles;
		}
	}
};
