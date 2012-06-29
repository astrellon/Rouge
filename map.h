#pragma once

#include "gfx/gfx_renderable.h"
#include "gfx/gfx_sprite.h"
#include "gfx/gfx_text_field.h"

#include "handle.h"
#include "json_value.h"
using namespace am::util;

#include "game_object.h"

namespace am {
namespace gfx {
	class Asset;
}
using namespace am::gfx;

namespace base {

	class TileInstance;

	class Map : public Renderable {
	public:
		Map(const char *name);
		Map(const char *name, int width, int height);
		~Map();
	
		void setName(const char *name);
		string getName() const;

		void setFullName(const char *name);
		string getFullName() const;

		TileInstance *getTile(int x, int y);
		TileInstance *getTiles();

		Layer *getBackground();
		Layer *getForeground();

		void setSize(int width, int height);
		int getWidth() const;
		int getHeight() const;

		ObjectList *getObjects();
		bool addGameObject(GameObject *object);
		bool removeGameObject(GameObject *object);
		bool hasGameObject(GameObject *object) const;

		void loadDef(JsonValue loaded);

		void updateAssetSprites();
		virtual void render(float dt);

	protected:

		TileInstance *mTiles;
		int mWidth;
		int mHeight;
		
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
