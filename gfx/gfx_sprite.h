#pragma once

#include <base/handle.h>
using namespace am::base;

#include <vector>
using namespace std;

#include <util/idata.h>
using namespace am::util;

#include "gfx_renderable.h"
#include "texture_window.h"
#include "scale_nine.h"

namespace am {
namespace game {
	class LoadingState;
}

namespace gfx {

	class Asset;

	class Sprite : public Renderable {
	public:
		Sprite();
		Sprite(const Sprite &copy);
		Sprite(Asset *asset);
		Sprite(const char *assetName);
		~Sprite();

		virtual Renderable *clone() const;

		Asset *getAsset();
		void setAsset(Asset *asset);
		void setAsset(const char *assetName);
		
		void setFrameRate(float rate);
		float getFrameRate() const;

		void setCurrentFrame(int frame);
		int getCurrentFrame() const;

		void setTextureFrame(int frame);
		int getTextureFrame() const;

		void setFrameTime(float time);
		float getFrameTime() const;

		void setScaleNineState(ScaleNineState state);
		ScaleNineState getScaleNineState() const;

		// Renderable methods
		virtual void render(float dt);

		virtual float getWidth();
		virtual float getHeight();

		void updateSprite(float dt);
		virtual void renderSprite();

		virtual string getName() const;

		data::IData *serialise();
		void deserialise(am::game::LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;
		
	protected:

		Handle<Asset> mAsset;

		int mCurrentFrame;
		int mTextureFrame;
		float mFrameRate;
		float mCurrentTime;

		ScaleNineState mScaleNineState;

		virtual int getTotalFrames() const;

		virtual void renderTexture(const TextureWindow &win, const float &width, const float &height);
	};

}
}
