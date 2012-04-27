#pragma once

#include "itile.h"

class ITileInstance {
public:
	ITileInstance(ITile *tile) : mTile(tile) {
	}
	~ITileInstance() {
	}
	
protected:
	ITile *mTile;
}