#pragma once

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_sprite.h>
#include <gfx/gfx_text_field.h>
#include <gfx/tile_renderer.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

#include "game_object.h"
#include "astar_node.h"

namespace am {
namespace gfx {
	class Asset;
}
using namespace am::gfx;

namespace game {

	class TileInstance;
	class Tile;

	class Map : public IManaged {
	public:
		Map(const char *name);
		Map(const char *name, int width, int height);
		Map(const Map &rhs);
		~Map();

		//virtual Renderable *clone() const;

		virtual void deinit();
	
		void setName(const char *name);
		string getName() const;

		void setFullName(const char *name);
		string getFullName() const;

		Tile *getTile(int x, int y);
		TileInstance *getTileInstance(int x, int y);
		TileInstance *getTiles();
		const TileInstance *getTiles() const;

		Layer *getBackground() const;
		Layer *getForeground() const;
		TileRenderer *getTileRenderer() const;

		void setMapSize(int width, int height);
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
		bool isValidGridLocation(int gridX, int gridY, const vector< Handle<TileType> > &passibles) const;
		bool isValidGridLocation(int gridX, int gridY, const TileType *forTileType) const;

		//void loadDef(LuaState &lua);

		//void updateAssetSprites();

		void setFilename(const char *filename);
		const char *getFilename() const;

		//virtual void render(float dt);

		bool search(const Vector2i &start, Vector2i end, NodePath &path, const GameObject *forObj);

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

		string mName;
		string mFullName;
		string mFilename;

		Handle<Layer> mBackground;
		Handle<Layer> mForeground;
		Handle<TileRenderer> mTileRenderer;

		//bool mEnabledMapCulling;

		// To keep memory at a sane level for larger maps, there is currently
		// only one sprite per asset. This should also keep tiles of the same animated
		// graphic in sync, and as well as not doubling up on computing the current frame.
		//typedef map<Asset *, Handle<Sprite> > AssetSpriteMap;
		//AssetSpriteMap mAssetSprites;

		void clear();
		ObjectList::const_iterator findGameObject(GameObject *object) const;

		bool _isValidGridLocation(int gridX, int gridY, const GameObject *forObject) const;
	};

}
}
