#pragma once

#include "gfx/gfx_renderable.h"
#include "gfx/gfx_sprite.h"

#include "handle.h"
using namespace am::util;

namespace am {
namespace gfx {
	class Asset;
}
using namespace am::gfx;

namespace base {

	class TileInstance;

	class Map : public Renderable {
	public:
		Map(int width, int height);
		~Map();
	
		TileInstance *getTile(int x, int y);
		TileInstance *getTiles();

		void setSize(int width, int height);
		int getWidth() const;
		int getHeight() const;

		//virtual void render(float dt);

	protected:

		TileInstance *mTiles;
		int mWidth;
		int mHeight;

		// To keep memory at a sane level for larger maps, there is currently
		// only one sprite per asset. This should also keep tiles of the same animated
		// graphic in sync, and as well as not doubling up on computing the current frame.
		typedef map<Asset *, Handle<Sprite> > AssetSpriteMap;
		AssetSpriteMap mAssetSprites;
	
		void clear();
	};

}
}
