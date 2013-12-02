#pragma once

#include <gfx/gfx_renderable.h>
#include <gfx/gfx_sprite.h>

#include <base/handle.h>

#include <lua/lua_state.h>

namespace am {
namespace game {
	class Map;
	class Tile;
}

namespace gfx {
	class Asset;

	class TileRenderer : public Renderable 
	{
	public:
		TileRenderer(game::Map *map);
		TileRenderer(const TileRenderer &rhs);
		~TileRenderer();

		virtual Renderable *clone() const;

		virtual void setMap(game::Map *map);
		virtual game::Map *getMap() const;

		virtual float getWidth();
		virtual float getHeight();

		void updateAssetSprites();
		void updateAssetSprite(game::Tile *tile);
		virtual void render(float dt);

		virtual std::string getName() const;

	protected:

		game::Map *mMap;
		bool mEnabledMapCulling;

		// To keep memory at a sane level for larger maps, there is currently
		// only one sprite per asset. This should also keep tiles of the same animated
		// graphic in sync, and as well as not doubling up on computing the current frame.
		typedef std::map<Asset *, base::Handle<Sprite> > AssetSpriteMap;
		AssetSpriteMap mAssetSprites;

		void addAssetForUpdate(Asset *asset, float grid);

	};

}
}
