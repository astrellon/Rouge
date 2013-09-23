#pragma once

#include <string>
#include <map>
using namespace std;

#include <gfx/gfx_asset.h>
using namespace am::gfx;

#include <base/handle.h>
#include <base/imanaged.h>
using namespace am::base;

#include <util/idefinition.h>
using namespace am::util;

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace game {

	class TileSet;
	class TileType;
	
	class Tile : public IManaged, public IDefinition {
	public:

		typedef vector< Handle<TileType> > TileTypeSet;
		typedef vector< Handle<Asset> > TileAssetList;
		typedef map< Tile *, TileAssetList > TileAssetMap;

		Tile(const char *name);
		Tile(const char *name, const char *fullName);
		~Tile();

		string getName() const;
		void setName(const char *name);
		bool setNameWithCheck(const char *name);

		string getFullName() const;
		void setFullName(const char *name);

		string getDescription() const;
		void setDescription(const char *description);

		string getNameWithSet() const;

		Asset *getGraphicAsset();
		void setGraphicAsset(Asset *asset);

		void setTileSet(TileSet *tileSet);
		TileSet *getTileSet() const;

		void setPrecedence(int precedence);
		int getPrecedence() const;

		void addTileType(TileType *tileType);
		void removeTileType(TileType *tileType);
		void removeAllTileTypes();
		bool hasTileType(TileType *tileType) const;
		TileTypeSet &getTileTypes();
		const TileTypeSet &getTileTypes() const;

		void addTransitionalAsset(Asset *asset, Tile *overlapTile = nullptr);
		void removeTransitionalAsset(Asset *asset, Tile *overlapTile = nullptr);
		void removeTransitionalAsset(const char *assetName, Tile *overlapTile = nullptr);
		bool hasTransitionalAsset(Asset *asset, Tile *overlapTile = nullptr) const;
		bool hasTransitionalAsset(const char *assetName, Tile *overlapTile = nullptr) const;
		bool hasTransitionalAsset(Tile *overlapTile) const;
		const TileAssetList *getTransitionalAsset(Tile *overlapTile) const;
		const TileAssetMap &getAllTransitionalAssets() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

		static void setDefaultTile(Tile *tile);
		static Tile *getDefaultTile();
	
	protected:
	
		string mName;
		string mFullName;
		
		TileSet *mTileSet;
		Handle<Asset> mGraphic;
		TileAssetMap mTransitionalGraphics;
		string mDescription;
		int mPrecedence;

		TileTypeSet mTileTypes;

		static Handle<Tile> sDefaultTile;

	};

}
}
