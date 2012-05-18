#include "gl_font.h"

#include "gl_texture.h"
#include "gl_gfx_engine.h"

namespace am {
namespace gfx {

	GlFont::GlFont(GlGfxEngine *engine, const char *name) :
		mGfxEngine(engine),
		mTexture(NULL),
		mName(name)
	{

	}
	GlFont::~GlFont()
	{

	}

	ITexture *GlFont::getTexture()
	{
		return mTexture;
	}
	GlTexture *GlFont::getGlTexture()
	{
		return mTexture;
	}
	void GlFont::setTexture(ITexture *texture)
	{
		GlTexture *glTexture = dynamic_cast<GlTexture *>(texture);
		if (glTexture == NULL)
		{
			return;
		}
		mTexture = glTexture;
	}

	bool GlFont::isLoaded() const
	{
		return mTexture != NULL && mTexture->isLoaded() && !mCharRenders.empty();
	}

	int GlFont::loadDef(JsonValue value)
	{
		if (value.has("texture", JV_STR))
		{
			/*am::base::Texture *texture = 
				new am::base::Texture(value["texture"].getCStr());
			setTexture(texture);*/
			mTexture = mGfxEngine->loadGlTexture(value["texture"].getCStr());
		}
		if (value.has("fixedWidth", JV_BOOL))
		{
			mFixedWidth = value["fixedWidth"].getBool();
		}
		if (value.has("utf", JV_BOOL))
		{
			mUtfSupport = value["utf"].getBool();
		}

		postLoad();

		return 0;
	}

	string GlFont::getName() const
	{
		return mName;
	}

	bool GlFont::isFixedWidth() const
	{
		return mFixedWidth;
	}

	float GlFont::getCharHeight() const
	{
		return mCharHeight;
	}
	float GlFont::getFixedCharWidth() const
	{
		return mFixedCharWidth;
	}

	IGfxEngine *GlFont::getGfxEngine()
	{
		return mGfxEngine;
	}
	GlGfxEngine *GlFont::getGlGfxEngine()
	{
		return mGfxEngine;
	}

	void GlFont::getCharRender(char ch, CharRender &render)
	{
		render = mCharRenders[ch];
	}

	void GlFont::postLoad()
	{
		if (mTexture == NULL || !mTexture->isLoaded())
		{
			return;
		}
		CharRender render;

		if (!mUtfSupport)
		{
			mCharHeight = mTexture->getHeight() / 16.0f;
			mFixedCharWidth = mTexture->getWidth() / 16.0f;
			if (mFixedWidth)
			{
				for (int y = 0; y < 16; y++)
				{
					for (int x = 0; x < 16; x++)
					{
						render.mWidth = mFixedCharWidth;
						render.mHeight = mCharHeight;
						float uvWidth = render.mWidth / mTexture->getWidth();
						float uvHeight = render.mHeight / mTexture->getHeight();
						render.mLeftX = static_cast<float>(x) * uvWidth;
						render.mTopY =  static_cast<float>(y) * uvHeight;
						render.mRightX = render.mLeftX + uvWidth;
						render.mBottomY = render.mTopY + uvHeight;
						mCharRenders.push_back(render);
					}
				}
			}
			else
			{
				int *tempData = new int[mTexture->getWidth() * mTexture->getHeight()];
				glBindTexture(GL_TEXTURE_2D, mTexture->getTextureId());
				glGetTexImage(GL_TEXTURE_2D, 0, mTexture->getGlFormat(), GL_UNSIGNED_BYTE, tempData);
				for (int y = 0; y < 16; y++)
				{
					for (int x = 0; x < 16; x++)
					{
						pair<float, float> pos = processChar(tempData, x, y);

						render.mWidth = (pos.second - pos.first);
						render.mHeight = mCharHeight;
						float uvWidth = render.mWidth / mTexture->getWidth();
						float uvHeight = render.mHeight / mTexture->getHeight();
						render.mLeftX = pos.first / mTexture->getWidth();
						render.mTopY = static_cast<float>(y) * uvHeight;
						render.mRightX = render.mLeftX + uvWidth;
						render.mBottomY = render.mTopY + uvHeight;

						mCharRenders.push_back(render);
					}
				}

				delete tempData;
			}
		}
	}

	pair<float, float> GlFont::processChar(int *data, int xPos, int yPos)
	{
		float left = -1;
		float right = -1;

		int width = static_cast<int>(mTexture->getWidth());

		int xStart = xPos * mFixedCharWidth;
		int xEnd = xStart + mFixedCharWidth;

		int yStart = yPos * mCharHeight;
		int yEnd = yStart + mCharHeight;

		for (int x = xStart; x < xEnd; x++)
		{
			for (int y = yStart; y < yEnd; y++)
			{
				unsigned int pixel = data[y * width + x];
				unsigned int alpha = pixel & 0xFF000000;
				if (alpha > 0)
				{
					if (left < 0)
					{
						left = static_cast<float>(x);
					}
					right = static_cast<float>(x);
					break;
				}
			}
		}

		if (left > xStart)
		{
			left--;
		}

		if (right < xEnd)
		{
			right++;
		}

		if (left == -1)
		{
			left = xStart;
			right = xEnd - 1;
		}

		return pair<float, float>(left, right);
	}

}
}
