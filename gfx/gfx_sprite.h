#pragma once

#include "gfx_renderable.h"

#include "../transform.h"
#include "texture_window.h"

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

		void setNumFramesX(int num);
		int getNumFramesX() const;
		void setNumFramesY(int num);
		int getNumFramesY() const;

		void setNumTotalFrames(int frames);
		int getNumTotalFrames() const;

		void setCurrentFrame(int frame);
		int getCurrentFrame() const;

		void setFrameRate(float rate);
		float getFrameRate() const;

		void setFrameTime(float time);
		float getFrameTime() const;

		// Renderable methods
		virtual void render(float dt);
		
	protected:

		Asset *mAsset;

		int mNumFramesX;
		int mNumFramesY;
		int mMaxFrames;
		int mCurrentFrame;

		float mFrameRate;
		float mCurrentTime;

		bool mAnimationDirty;
		vector<TextureWindow> mAnimationWindows;

		void processAnimation();
	};

}
}
