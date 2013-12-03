#include "ui_asset_renderer.h"

namespace am {
namespace ui {

	AssetRenderer::AssetRenderer()
	{
		
	}
	AssetRenderer::AssetRenderer(gfx::Asset *asset)
	{
		setAsset(asset);
	}
	AssetRenderer::~AssetRenderer()
	{

	}

	void AssetRenderer::setAsset(gfx::Asset *asset)
	{
		mAsset = asset;
	}
	gfx::Asset *AssetRenderer::getAsset()
	{
		return mAsset;
	}

}
}
