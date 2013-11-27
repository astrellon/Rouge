#pragma once

#include <base/handle.h>

#include <gfx/gfx_sprite.h>
#include <gfx/gfx_asset.h>
using namespace am::gfx;

#include <ui/ui_component.h>

namespace am {
namespace ui {

	class AssetRenderer : public UIComponent {
	public:
		AssetRenderer();
		AssetRenderer(Asset *asset);
		~AssetRenderer();

		void setAsset(Asset *asset);
		Asset *getAsset();

	protected:

		am::base::Handle<Asset> mAsset;

	};

}
}
