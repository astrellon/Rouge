#pragma once

namespace am {
namespace base {

	class Tile;

	class TileInstance {
	public:
		TileInstance();
		TileInstance(Tile *tile);
		TileInstance(Tile *tile, int x, int y);
		~TileInstance();
	
		int getX() const;
		int getY() const;
	
		void setPos(int x, int y);
	
		Tile *getTile();
		void setTile(Tile *tile);
	
	protected:
		Tile *mTile;

		int mXpos;
		int mYpos;
	};

}
}

#ifdef _ANDROID
#	include "tile_instance.cpp"
#endif
