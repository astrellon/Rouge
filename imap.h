#pragma once

class IMap {
public:
	ITileInstance *getTile(int x, int y);
	ITileInstance [8]getNearbyTiles(int x, int y);
protected:
};
