#pragma once

#include <string>
#include <map>
using namespace std;

#include <gfx/gfx_asset.h>
using namespace am::gfx;

#include <base/handle.h>
#include <base/imanaged.h>
using namespace am::base;

//#include <util/json_value.h>
//using namespace am::util;

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace game {

	class TileSet;
	class TileType;
	
	class Tile : public IManaged {
	public:

		typedef vector< Handle<TileType> > TileTypeList;
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
		TileTypeList &getTileTypes();
		const TileTypeList &getTileTypes() const;

		void addTransitionalAsset(Tile *overlapTile, Asset *asset);
		void removeTransitionalAsset(Tile *overlapTile, Asset *asset);
		bool hasTransitionalAsset(Tile *overlapTile, Asset *asset) const;
		bool hasTransitionalAssets(Tile *overlapTile) const;
		const TileAssetList *getTransitionalAsset(Tile *overlapTile) const;
		const TileAssetMap &getAllTransitionalAssets() const;

		//void loadDef(JsonValue value);
		void loadDef(LuaState &lua);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;
	
	protected:
	
		string mName;
		string mFullName;
		TileSet *mTileSet;
		Handle<Asset> mGraphic;
		TileAssetMap mTransitionalGraphics;
		string mDescription;
		int mPrecedence;

		TileTypeList mTileTypes;

	};

}
}
