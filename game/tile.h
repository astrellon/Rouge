#pragma once

#include <string>
#include <map>

#include <gfx/gfx_asset.h>

#include <base/handle.h>
#include <base/imanaged.h>

#include <util/idefinition.h>

#include <lua/lua_state.h>

namespace am {
namespace game {

	class TileSet;
	class TileType;
	
	class Tile : public base::IManaged, public util::IDefinition 
	{
	public:

		typedef std::vector< base::Handle<TileType> > TileTypeSet;
		typedef std::vector< base::Handle<gfx::Asset> > TileAssetList;
		typedef std::map< Tile *, TileAssetList > TileAssetMap;

		Tile(const char *name);
		Tile(const char *name, const char *fullName);
		~Tile();

		std::string getName() const;
		void setName(const char *name);
		bool setNameWithCheck(const char *name);

		std::string getFullName() const;
		void setFullName(const char *name);

		std::string getDescription() const;
		void setDescription(const char *description);

		std::string getNameWithSet() const;

		gfx::Asset *getGraphicAsset();
		void setGraphicAsset(gfx::Asset *asset);

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

		void addTransitionalAsset(gfx::Asset *asset, Tile *overlapTile = nullptr);
		void removeTransitionalAsset(gfx::Asset *asset, Tile *overlapTile = nullptr);
		void removeTransitionalAsset(const char *assetName, Tile *overlapTile = nullptr);
		bool hasTransitionalAsset(gfx::Asset *asset, Tile *overlapTile = nullptr) const;
		bool hasTransitionalAsset(const char *assetName, Tile *overlapTile = nullptr) const;
		bool hasTransitionalAsset(Tile *overlapTile) const;
		const TileAssetList *getTransitionalAsset(Tile *overlapTile) const;
		const TileAssetMap &getAllTransitionalAssets() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

		static void setDefaultTile(Tile *tile);
		static Tile *getDefaultTile();
	
	protected:
	
		std::string mName;
		std::string mFullName;
		
		TileSet *mTileSet;
		base::Handle<gfx::Asset> mGraphic;
		TileAssetMap mTransitionalGraphics;
		std::string mDescription;
		int mPrecedence;

		TileTypeSet mTileTypes;

		static base::Handle<Tile> sDefaultTile;

	};

}
}
