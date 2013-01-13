#pragma once

#include <string>
using std::string;

namespace am {
namespace game {

	class Tile;

	class TileInstance {
	public:
		TileInstance();
		TileInstance(Tile *tile);
		~TileInstance();
	
		Tile *getTile();
		const Tile *getTile() const;
		void setTile(Tile *tile);
		bool setTileName(const char *tileName);
		bool setTileName(const string &tileName);

		void setTileFrame(int frame);
		int getTileFrame() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		Tile *mTile;
		int mTileFrame;
	};

}
}
