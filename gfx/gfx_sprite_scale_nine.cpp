#include "gfx_sprite_scale_nine.h"

#include "gfx_texture.h"
#include "gfx_asset.h"
#include "../gl.h"

namespace am {
namespace gfx {

	SpriteScaleNine::SpriteScaleNine(GfxEngine *engine) :
		Sprite(engine),
		mEnableScaleNine(false)
	{
	}
	SpriteScaleNine::SpriteScaleNine(GfxEngine *engine, Asset *asset) :
		Sprite(engine, asset),
		mEnableScaleNine(false)
	{
	}
	SpriteScaleNine::SpriteScaleNine(GfxEngine *engine, const char *assetName) :
		Sprite(engine, assetName),
		mEnableScaleNine(false)
	{
	}
	SpriteScaleNine::~SpriteScaleNine()
	{
	}

	void SpriteScaleNine::setScaleNine(const ScaleNine &scaleNine)
	{
		mScaleNine = scaleNine;
	}
	void SpriteScaleNine::setScaleNine(const ScaleNine &scaleNine, bool enable)
	{
		mScaleNine = scaleNine;
		mEnableScaleNine = enable;
	}
	ScaleNine &SpriteScaleNine::getScaleNine()
	{
		return mScaleNine;
	}

	void SpriteScaleNine::setScaleNineEnabled(bool enable)
	{
		mEnableScaleNine = enable;
	}
	bool SpriteScaleNine::isScaleNineEnabled() const
	{
		return mEnableScaleNine;
	}

	void SpriteScaleNine::renderTexture(const TextureWindow &win, const float &width, const float &height)
	{
		if (mEnableScaleNine)
		{
			float rightX = width - (win.getWidth() - mScaleNine.getRight());
			float bottomY = height - (win.getHeight() - mScaleNine.getBottom());

			float leftU = mScaleNine.getLeft() / mAsset->getTexture()->getWidth() + win.getLeftX();
			float rightU = mScaleNine.getRight() / mAsset->getTexture()->getWidth() + win.getLeftX();
			float topV = mScaleNine.getTop() / mAsset->getTexture()->getHeight() + win.getTopY();
			float bottomV = mScaleNine.getBottom() / mAsset->getTexture()->getHeight() + win.getTopY();

			glBegin(GL_TRIANGLE_STRIP);

				// Top left
				glTexCoord2f(win.getLeftX(), win.getTopY());
				glVertex2f(0.0f, 0.0f);

				glTexCoord2f(win.getLeftX(), topV);
				glVertex2f(0.0f, mScaleNine.getTop());

				glTexCoord2f(leftU, win.getTopY());
				glVertex2f(mScaleNine.getLeft(), 0.0f);

				glTexCoord2f(leftU, topV);
				glVertex2f(mScaleNine.getLeft(), mScaleNine.getTop());

				// Top middle
				glTexCoord2f(rightU, win.getTopY());
				glVertex2f(rightX, 0.0f);

				glTexCoord2f(rightU, topV);
				glVertex2f(rightX, mScaleNine.getTop());

				// Top right
				glTexCoord2f(win.getRightX(), win.getTopY());
				glVertex2f(width, 0.0f);

				glTexCoord2f(win.getRightX(), topV);
				glVertex2f(width, mScaleNine.getTop());

			glEnd();

			glBegin(GL_TRIANGLE_STRIP);
				
				// Middle left
				glTexCoord2f(win.getLeftX(), topV);
				glVertex2f(0.0f, mScaleNine.getTop());

				glTexCoord2f(win.getLeftX(), bottomV);
				glVertex2f(0.0f, bottomY);

				glTexCoord2f(leftU, topV);
				glVertex2f(mScaleNine.getLeft(), mScaleNine.getTop());

				glTexCoord2f(leftU, bottomV);
				glVertex2f(mScaleNine.getLeft(), bottomY);

				// Middle middle
				glTexCoord2f(rightU, topV);
				glVertex2f(rightX, mScaleNine.getTop());

				glTexCoord2f(rightU, bottomV);
				glVertex2f(rightX, bottomY);

				// Middle right
				glTexCoord2f(win.getRightX(), topV);
				glVertex2f(width, mScaleNine.getTop());

				glTexCoord2f(win.getRightX(), bottomV);
				glVertex2f(width, bottomY);

			glEnd();

			glBegin(GL_TRIANGLE_STRIP);

				// Bottom left
				glTexCoord2f(win.getLeftX(), bottomV);
				glVertex2f(0.0f, bottomY);

				glTexCoord2f(win.getLeftX(), win.getBottomY());
				glVertex2f(0.0f, height);

				glTexCoord2f(leftU, bottomV);
				glVertex2f(mScaleNine.getLeft(), bottomY);

				glTexCoord2f(leftU, win.getBottomY());
				glVertex2f(mScaleNine.getLeft(), height);

				// Bottom middle
				glTexCoord2f(rightU, bottomV);
				glVertex2f(rightX, bottomY);

				glTexCoord2f(rightU, win.getBottomY());
				glVertex2f(rightX, height);

				// Bottom right
				glTexCoord2f(win.getRightX(), bottomV);
				glVertex2f(width, bottomY);

				glTexCoord2f(win.getRightX(), win.getBottomY());
				glVertex2f(width, height);

			glEnd();
		}
		else
		{
			Sprite::renderTexture(win, width, height);
		}
	}

}
}
