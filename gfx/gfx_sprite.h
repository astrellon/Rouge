#pragma once

#include <base\handle.h>
using namespace am::base;

#include <vector>
using namespace std;

#include "gfx_renderable.h"
#include "texture_window.h"
#include "scale_nine.h"

namespace am {
namespace gfx {

	class Asset;

	class Sprite : public Renderable {
	public:
		Sprite();
		Sprite(Asset *asset);
		Sprite(const char *assetName);
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

		void setScaleNineState(ScaleNineState state);
		ScaleNineState getScaleNineState() const;

		// Renderable methods
		virtual void render(float dt);

		virtual float getWidth();
		virtual float getHeight();

		void updateSprite(float dt);
		void renderSprite();

		static const int LUA_ID;
		static const char *LUA_TABLENAME;
		
	protected:

		Asset *mAsset;

		int mCurrentFrame;
		float mFrameRate;
		float mCurrentTime;

		ScaleNineState mScaleNineState;

		virtual void renderTexture(const TextureWindow &win, const float &width, const float &height);
	};

}
}
