#pragma once

#include "gfx/gfx_renderable.h"
#include "gfx/gfx_sprite.h"
#include "gfx/gfx_text_field.h"

#include <base/handle.h>
using namespace am::base;

#include <util/json_value.h>
using namespace am::util;

#include "game_object.h"

namespace am {
namespace gfx {
	class Asset;
}
using namespace am::gfx;

namespace game {

	class TileInstance;
	class Tile;

	class Map : public Renderable {
	public:
		Map(const char *name);
		Map(const char *name, int width, int height);
		~Map();
	
		void setName(const char *name);
		string getName() const;

		void setFullName(const char *name);
		string getFullName() const;

		Tile *getTile(int x, int y);
		TileInstance *getTileInstance(int x, int y);
		TileInstance *getTiles();

		Layer *getBackground();
		Layer *getForeground();

		void setMapSize(int width, int height);
		int getMapWidth() const;
		int getMapHeight() const;

		ObjectList *getObjects();
		bool addGameObject(GameObject *object);
		bool removeGameObject(GameObject *object);
		bool hasGameObject(GameObject *object) const;

		bool isValidGridLocation(int gridX, int gridY, const GameObject *forObject) const;
		bool isValidGridLocation(int gridX, int gridY, const vector<TileType *> &passibles) const;
		bool isValidGridLocation(int gridX, int gridY, const TileType *forTileType) const;

		void loadDef(JsonValue loaded);

		void updateAssetSprites();

		virtual void render(float dt);

	protected:

		TileInstance *mTiles;
		int mMapWidth;
		int mMapHeight;
		
		ObjectList mObjects;

		string mName;
		string mFullName;

		Handle<Layer> mBackground;
		Handle<Layer> mForeground;

		bool mEnabledMapCulling;

		// To keep memory at a sane level for larger maps, there is currently
		// only one sprite per asset. This should also keep tiles of the same animated
		// graphic in sync, and as well as not doubling up on computing the current frame.
		typedef map<Asset *, Handle<Sprite> > AssetSpriteMap;
		AssetSpriteMap mAssetSprites;
	
		void clear();
		ObjectList::const_iterator findGameObject(GameObject *object) const;
	};

}
}
