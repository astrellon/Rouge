#include "ui_asset_renderer.h"

//#include <gfx/gfx_engine.h>

namespace am {
namespace ui {

	AssetRenderer::AssetRenderer()
	{
		
	}
	AssetRenderer::AssetRenderer(Asset *asset)
	{
		setAsset(asset);
	}
	AssetRenderer::~AssetRenderer()
	{

	}

	void AssetRenderer::setAsset(Asset *asset)
	{
		mAsset = asset;
	}
	Asset *AssetRenderer::getAsset()
	{
		return mAsset;
	}

}
}
