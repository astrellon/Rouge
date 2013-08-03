#include "tile_renderer.h"

#include <gl.h>
#include <gfx/gfx_engine.h>

#include <game/engine.h>
#include <game/map.h>
#include <game/tile.h>
#include <game/tile_instance.h>

#include <util/utils.h>
using namespace am::util;

#include <sstream>
using namespace std;

namespace am {
namespace gfx {

	TileRenderer::TileRenderer(Map *map) :
		mMap(map),
		mEnabledMapCulling(true)
	{
		if (map)
		{
			map->retain();
		}
	}
	TileRenderer::TileRenderer(const TileRenderer &rhs) :
		mMap(rhs.mMap),
		mEnabledMapCulling(rhs.mEnabledMapCulling)
	{
		if (mMap)
		{
			mMap->retain();
		}
	}
	TileRenderer::~TileRenderer()
	{
		if (mMap)
		{
			Map *temp = mMap;
			mMap = NULL;
			temp->release();
		}
	}

	Renderable *TileRenderer::clone() const
	{
		return new TileRenderer(*this);
	}

	void TileRenderer::setMap(Map *map)
	{
		Handle<Map> oldMap(mMap);
		if (mMap)
		{
			mMap->release();
		}
		mMap = map;
		if (map)
		{
			map->retain();
		}
	}
	Map *TileRenderer::getMap() const
	{
		return mMap;
	}

	float TileRenderer::getWidth()
	{
		if (mMap)
		{
			return mMap->getWidth();
		}
		return 0.0f;
	}
	float TileRenderer::getHeight()
	{
		if (mMap)
		{
			return mMap->getHeight();
		}
		return 0.0f;
	}

	void TileRenderer::updateAssetSprites()
	{
		mAssetSprites.clear();
		if (!mMap)
		{
			return;
		}

		int numTiles = mMap->getMapWidth() * mMap->getMapHeight();
		float grid = Engine::getEngine()->getGridSize();

		TileInstance *tiles = mMap->getTiles();

		for (int i = 0; i < numTiles; i++)
		{
			Asset *asset = tiles[i].getTile()->getGraphicAsset();
			if (asset)
			{
				AssetSpriteMap::iterator iter = mAssetSprites.find(asset);
				if (iter == mAssetSprites.end())
				{
					Sprite *assetSprite = new Sprite(asset);
					mAssetSprites[asset] = assetSprite;
					assetSprite->setWidth(grid);
					assetSprite->setHeight(grid);
				}
			}
		}
	}
	
	void TileRenderer::render(float dt)
	{
		if (!mMap)
		{
			return;
		}

		AssetSpriteMap::iterator iter;
		for (iter = mAssetSprites.begin(); iter != mAssetSprites.end(); ++iter)
		{
			iter->second->updateSprite(dt);
		}

		float grid = Engine::getEngine()->getGridSize();
		float gridResp = Engine::getEngine()->getGridSizeResp();

		GfxEngine *gfxEngine = GfxEngine::getEngine();
		float cameraX = gfxEngine->getCameraX();
		float cameraY = gfxEngine->getCameraY();

		float screenWidth = static_cast<float>(gfxEngine->getScreenWidth());
		float screenHeight = static_cast<float>(gfxEngine->getScreenHeight());

		int mapWidth = mMap->getMapWidth();
		int mapHeight = mMap->getMapHeight();

		int minX = 0;
		int maxX = mapWidth;
		int minY = 0;
		int maxY = mapHeight;

		TileInstance *tiles = mMap->getTiles();

		if (mEnabledMapCulling)
		{
			int camMinX = static_cast<int>((cameraX - screenWidth * 0.5f) * gridResp);
			minX = max(0, camMinX);
			int camMaxX = static_cast<int>((cameraX + screenWidth * 0.5f) * gridResp) + 1;
			maxX = min(mapWidth, camMaxX);

			int camMinY = static_cast<int>((cameraY - screenHeight * 0.5f) * gridResp);
			minY = max(0, camMinY);
			int camMaxY = static_cast<int>((cameraY + screenHeight * 0.5f) * gridResp) + 1;
			maxY = min(mapHeight, camMaxY);
		}
		glPushMatrix();
		int t = minY * mapWidth + minX;
		int tStep = mapWidth - (maxX - minX);

		float resetX = -(maxX - minX) * grid;
		glTranslatef(minX * grid, minY * grid, 0.0f);
		for (int y = minY; y < maxY; y++)
		{
			t = y * mapWidth + minX;
			for (int x = minX; x < maxX; x++)
			{
				Asset *asset = tiles[t].getTile()->getGraphicAsset();
				Sprite *sprite = mAssetSprites[asset].get();
				if (asset->getFrameRate() <= 0.0f)
				{
					sprite->setCurrentFrame(tiles[t].getTileFrame());
				}
				sprite->renderSprite();
				t++;

				glTranslatef(grid, 0.0f, 0.0f);
			}
			//t += tStep;
			glTranslatef(resetX, grid, 0.0f);
		}
		glPopMatrix();
	}

}
}
