#pragma once

#include <base/handle.h>

#include <gfx/gfx_sprite.h>
#include <gfx/gfx_asset.h>

#include <ui/ui_component.h>

namespace am {
namespace ui {

	class AssetRenderer : public UIComponent 
	{
	public:
		AssetRenderer();
		AssetRenderer(gfx::Asset *asset);
		~AssetRenderer();

		void setAsset(gfx::Asset *asset);
		gfx::Asset *getAsset();

	protected:

		base::Handle<gfx::Asset> mAsset;

	};

}
}
