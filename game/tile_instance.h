#pragma once

namespace am {
namespace game {

	class Tile;

	class TileInstance {
	public:
		TileInstance();
		TileInstance(Tile *tile);
		~TileInstance();
	
		Tile *getTile();
		void setTile(Tile *tile);

		void setTileFrame(int frame);
		int getTileFrame() const;
	
	protected:
		Tile *mTile;
		int mTileFrame;
	};

}
}
