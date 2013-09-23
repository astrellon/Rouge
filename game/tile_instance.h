#pragma once

#include <base/handle.h>
using am::base::Handle;

#include <string>
#include <vector>
using std::string;
using std::vector;

#include <game/tile_type.h>

#include <stdint.h>

namespace am {
namespace game {

	class Tile;

	class TileInstance {
	public:

		typedef vector< Handle<TileType> > TileTypeList;

		TileInstance();
		TileInstance(Tile *tile);
		~TileInstance();
	
		Tile *getTile();
		const Tile *getTile() const;
		void setTile(Tile *tile);
		bool setTileName(const char *tileName);
		bool setTileName(const string &tileName);

		void setBaseVariation(int variation);
		int getBaseVariation() const;
		void randomiseVaritation();

		void setTransitionVariation(int variation);
		int getTransitionVariation() const;

		bool hasEdgeValue() const;

		void clearTileEdges();
		void setTileEdgeValue(uint32_t index, uint8_t value);
		uint8_t getTileEdgeValue(uint32_t index) const;

		void addTileType(TileType *tileType);
		void removeTileType(TileType *tileType);
		void removeAllTileTypes();
		bool hasTileType(TileType *tileType) const;
		TileTypeList &getTileTypes();
		const TileTypeList &getTileTypes() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		Tile *mTile;
		uint16_t mBaseVariation;
		uint16_t mTransitionVariation;
		uint8_t mTileEdgeValues[8];
		TileTypeList mTileTypes;
	};

}
}
