#pragma once

#include "gfx_renderable.h"

#include "../transform.h"
#include "texture_window.h"

#include "../handle.h"

#include <vector>

using namespace std;

namespace am {
namespace gfx {

	class GfxEngine;
	class Asset;

	class Sprite : public Renderable {
	public:
		Sprite(GfxEngine *engine);
		Sprite(GfxEngine *engine, Asset *asset);
		Sprite(GfxEngine *engine, const char *assetName);
		~Sprite();

		Asset *getAsset();
		void setAsset(Asset *asset);
		void setAsset(const char *assetName);
		
		void setFrameRate(float rate);
		float getFrameRate() const;

		void setCurrentFrame(int frame);
		int getCurrentFrame() const;

		void setFrameTime(float time);
		float getFrameTime() const;

		// Renderable methods
		virtual void render(float dt);

		virtual float getWidth();
		virtual float getHeight();
		
	protected:

		am::util::Handle<Asset> mAsset;

		int mCurrentFrame;
		float mFrameRate;
		float mCurrentTime;

		virtual void renderTexture(const TextureWindow &win, const float &width, const float &height);
	};

}
}
