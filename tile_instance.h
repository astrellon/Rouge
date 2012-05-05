#pragma once

#include "tile.h"

class TileInstance {
public:
	TileInstance(Tile *tile) : mTile(tile), mXpos(0), mYpos(0) {
	}
	TileInstance(Tile *tile, int x, int y) : mTile(tile), mXpos(x), mYpos(y) {
	}
	~TileInstance() {
	}
	
	int getX() const {
		return mXpos;
	}
	int getY() const {
		return mYpos;
	}
	
	void setPos(int x, int y) {
		mXpos = x;
		mYpos = y;
	}
	
	Tile *getTile() {
		return mTile;
	}
	void setTile(Tile *tile) {
		mTile = tile;
	}
	
protected:
	Tile *mTile;

	int mXpos;
	int mYpos;
}
