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
		//for (auto iter
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
		am::base::Handle<Map> oldMap(mMap);
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
			Tile *tile = tiles[i].getTile();
			if (!tile)
			{
				continue;
			}
			Asset *asset = tile->getGraphicAsset();
			addAssetForUpdate(asset, grid);
			auto transitionalTiles = tile->getAllTransitionalAssets();
			for (auto iter = transitionalTiles.begin(); iter != transitionalTiles.end(); ++iter)
			{
				for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
				{
					addAssetForUpdate(*iter2, grid);
				}
			}
		}
	}
	void TileRenderer::updateAssetSprite(Tile *tile)
	{
		if (!mMap || !tile || !tile->getGraphicAsset())
		{
			return;
		}
		Asset *asset = tile->getGraphicAsset();
		if (mAssetSprites.find(asset) != mAssetSprites.end())
		{
			return;
		}

		float grid = Engine::getEngine()->getGridSize();
		addAssetForUpdate(tile->getGraphicAsset(), grid);
		auto transitionalTiles = tile->getAllTransitionalAssets();
		for (auto iter = transitionalTiles.begin(); iter != transitionalTiles.end(); ++iter)
		{
			for (auto iter2 = iter->second.begin(); iter2 != iter->second.end(); ++iter2)
			{
				addAssetForUpdate(*iter2, grid);
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

		int offsets[8] = {-1 - mapWidth, -mapWidth, -mapWidth + 1, -1, 1, -1 + mapWidth, mapWidth, 1 + mapWidth};

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
				TileInstance &instance = tiles[t];
				Asset *asset = instance.getTile()->getGraphicAsset();
				if (asset == nullptr)
				{
					// Render something else.
					continue;
				}
				Sprite *sprite = mAssetSprites[asset];
				sprite->setFrameRate(asset->getFrameRate());
				// Only 1 sprite per asset.
				// So to get each variation for non-animated tiles then
				// we need to set the frame from each tile instance onto the
				// sprite just before we render that sprite.
				if (asset->getFrameRate() <= 0.0f)
				{
					sprite->setSubWindowFrame(instance.getBaseVariation());
					//sprite->setSubWindowFrame(instance.getTileSubWindowFrame());
				}
				else if (asset->isSubWindowAnimation())
				{
					sprite->setTextureFrame(instance.getBaseVariation());
				}
				sprite->renderSprite();
				if (instance.hasEdgeValue())
				{
					for (int i = 0; i < 8; i++)
					{
						uint8_t value = instance.getTileEdgeValue(i);
						if (value != 0)
						{
							Tile *overlapTile = tiles[t + offsets[i]].getTile();
							const Tile::TileAssetList *assets = overlapTile->getTransitionalAsset(instance.getTile());
							if (!assets)
							{
								assets = overlapTile->getTransitionalAsset(nullptr);
								if (!assets)
								{
									continue;
								}
							}
							int variation = instance.getTransitionVariation();
							if (variation >= assets->size())
							{
								variation = assets->size() - 1;
							}
							asset = assets->at(variation);
							{
								sprite = mAssetSprites[asset];
								if (!sprite)
								{
									continue;
								}
								if (!asset->isSubWindowAnimation() || asset->getTotalTextures() == 1)
								{
									sprite->setSubWindowFrame(value);
								}
								else
								{
									sprite->setTextureFrame(value);
								}
								sprite->renderSprite();
								sprite->setTextureFrame(0);
							}
						}
					}
				}
				t++;

				glTranslatef(grid, 0.0f, 0.0f);
			}
			glTranslatef(resetX, grid, 0.0f);
		}
		glPopMatrix();
	}

	void TileRenderer::addAssetForUpdate(Asset *asset, float grid)
	{
		if (asset)
		{
			AssetSpriteMap::iterator iter = mAssetSprites.find(asset);
			if (iter == mAssetSprites.end())
			{
				Sprite *assetSprite = new Sprite(asset);
				mAssetSprites[asset] = assetSprite;
				//asset->retain();
				assetSprite->setWidth(grid);
				assetSprite->setHeight(grid);
			}
		}
	}

	string TileRenderer::getName() const
	{
		return "TileRenderer";
	}

}
}
