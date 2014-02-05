#pragma once

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_text_field.h>
#include <gfx/tile_renderer.h>

#include <base/handle.h>
#include <base/return_codes.h>

#include <lua/lua_state.h>

#include "game_object.h"
#include "astar_node.h"
#include "map_region.h"
#include "unique_id.h"

namespace am {
namespace game {

	class TileInstance;
	class Tile;
	class Game;

	class Map : public base::IManaged, public UniqueId
	{
	public:

		enum TileFlags 
		{
			FLAG_L  = 0x01,
			FLAG_R  = 0x02,
			FLAG_T  = 0x04,
			FLAG_B  = 0x08,
			FLAG_TL = 0x11,
			FLAG_TR = 0x12,
			FLAG_BL = 0x14,
			FLAG_BR = 0x18
		};

		Map(const char *name);
		Map(const char *name, int width, int height);
		Map(const Map &rhs);
		~Map();

		virtual void deinit();
	
		void setName(const char *name);
		std::string getName() const;

		void setFullName(const char *name);
		std::string getFullName() const;

		Tile *getTile(int x, int y);
		TileInstance *getTileInstance(int x, int y);
		TileInstance *getTiles();
		const TileInstance *getTiles() const;
		void calcAllTileEdgeValues() const;
		void calcTileEdgeValuesAround(int x, int y) const;
		void calcTileEdgeValues(int x, int y) const;

		gfx::Layer *getBackground() const;
		gfx::Layer *getForeground() const;
		gfx::TileRenderer *getTileRenderer() const;

		void setMapSize(int width, int height, Tile *defaultTile = nullptr);
		int getMapWidth() const;
		int getMapHeight() const;

		float getWidth() const;
		float getHeight() const;

		ObjectList *getObjects();
		bool addGameObject(GameObject *object);
		bool removeGameObject(GameObject *object);
		bool hasGameObject(GameObject *object) const;
		
		bool isValidLocation(float x, float y, GameObject *forObject) const;
		
		bool isValidGridLocation(int gridX, int gridY, const GameObject *forObject) const;
		bool isValidGridLocation(int gridX, int gridY, const std::vector< base::Handle<TileType> > &passibles) const;
		bool isValidGridLocation(int gridX, int gridY, const TileType *forTileType) const;

		void setFilename(const char *filename);
		const char *getFilename() const;

		void setGamePartof(Game *game);
		Game *getGamePartof() const;

		bool addMapRegion(MapRegion *region);
		bool removeMapRegion(MapRegion *region);
		bool hasMapRegion(MapRegion *region);
		const MapRegion::MapRegionList &getMapRegions() const;

		void checkMapRegion(GameObject *obj);
		bool getGameObjectsAt(float worldX, float worldY, ObjectList &result) const;
		bool getGameObjectsAt(int gridX, int gridY, ObjectList &result) const;

		bool search(const math::Vector2i &start, math::Vector2i end, NodePath &path, const GameObject *forObj);

		base::ReturnCode saveMap(const char *filename) const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		AStarNode **mMapData;
		friend class Pathfinder;

		TileInstance *mTiles;
		int mMapWidth;
		int mMapHeight;
		
		ObjectList mObjects;

		float mWidth;
		float mHeight;

		std::string mName;
		std::string mFullName;
		std::string mFilename;

		base::Handle<gfx::Layer> mBackground;
		base::Handle<gfx::Layer> mForeground;
		base::Handle<gfx::TileRenderer> mTileRenderer;

		Game *mGamePartof;

		MapRegion::MapRegionList mMapRegions;

		bool canOverlap(int x, int y, Tile *overlapTile) const;
		bool canOverlap(Tile *tile, Tile *overlapTile) const;

		void clear();
		size_t findGameObject(GameObject *object) const;

		typedef struct _TileOrder
		{
			int precedence;
			int index;

			_TileOrder();
			_TileOrder(int precedence, int index);
		} TileOrder;

		bool _isValidGridLocation(int gridX, int gridY, const GameObject *forObject) const;
	};

}
}
