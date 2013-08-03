#pragma once

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_sprite.h>

#include <base/handle.h>
using namespace am::base;

#include <lua/lua_state.h>
using namespace am::lua;

namespace am {
namespace game {
	class Map;
}
using namespace am::game;

namespace gfx {
	class Asset;

	class TileRenderer : public Renderable {
	public:
		TileRenderer(Map *map);
		TileRenderer(const TileRenderer &rhs);
		~TileRenderer();

		virtual Renderable *clone() const;

		virtual void setMap(Map *map);
		virtual Map *getMap() const;

		virtual float getWidth();
		virtual float getHeight();

		void updateAssetSprites();
		virtual void render(float dt);

	protected:

		Map *mMap;
		bool mEnabledMapCulling;

		// To keep memory at a sane level for larger maps, there is currently
		// only one sprite per asset. This should also keep tiles of the same animated
		// graphic in sync, and as well as not doubling up on computing the current frame.
		typedef map<Asset *, Handle<Sprite> > AssetSpriteMap;
		AssetSpriteMap mAssetSprites;

	};

}
}
