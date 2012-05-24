#include "gl_font.h"

#include "gl_texture.h"
#include "gl_asset.h"
#include "gl_gfx_engine.h"

namespace am {
namespace gfx {

	GlFont::GlFont(GlGfxEngine *engine, const char *name) :
		mGfxEngine(engine),
		mAsset(NULL),
		mName(name),
		mKerning(1.0f),
		mLeading(1.0f),
		mSpaceWidth(6.0f),
		mTabWidth(24.0f),
		mCharsAcross(16),
		mCharsDown(16)
	{

	}
	GlFont::~GlFont()
	{

	}

	IAsset *GlFont::getAsset()
	{
		return mAsset;
	}
	GlAsset *GlFont::getGlAsset()
	{
		return mAsset;
	}
	void GlFont::setAsset(IAsset *asset)
	{
		setGlAsset(dynamic_cast<GlAsset *>(asset));
	}
	void GlFont::setGlAsset(GlAsset *asset)
	{
		if (asset != NULL)
		{
			mAsset = asset;
		}
	}

	bool GlFont::isLoaded() const
	{
		return mAsset != NULL && mAsset->getGlTexture() != NULL && !mTextureWindows.empty();
	}

	int GlFont::loadDef(JsonValue value)
	{
		if (value.has("asset", JV_STR))
		{
			mAsset = mGfxEngine->getGlAsset(value["asset"].getCStr());
		}
		if (value.has("fixedWidth", JV_BOOL))
		{
			mFixedWidth = value["fixedWidth"].getBool();
		}
		if (value.has("utf", JV_BOOL))
		{
			mUtfSupport = value["utf"].getBool();
		}
		if (value.has("kerning", JV_INT) || value.has("kerning", JV_FLOAT))
		{
			mKerning = value["kerning"].getFloat();
		}
		if (value.has("leading", JV_INT) || value.has("leading", JV_FLOAT))
		{
			mLeading = value["leading"].getFloat();
		}
		if (value.has("spaceWidth", JV_INT) || value.has("spaceWidth", JV_FLOAT))
		{
			mSpaceWidth = value["spaceWidth"].getFloat();
		}
		if (value.has("tabWidth", JV_INT) || value.has("tabWidth", JV_FLOAT))
		{
			mTabWidth = value["tabWidth"].getFloat();
		}
		if (value.has("charsAcross", JV_INT))
		{
			setCharsAcross(value["charsAcross"].getInt());
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

	void GlFont::setKerning(float kerning)
	{
		mKerning = kerning;
	}
	float GlFont::getKerning() const
	{
		return mKerning;
	}

	void GlFont::setLeading(float leading)
	{
		mLeading = leading;
	}
	float GlFont::getLeading() const
	{
		return mLeading;
	}

	void GlFont::setSpaceWidth(float width)
	{
		mSpaceWidth = width;
	}
	float GlFont::getSpaceWidth() const
	{
		return mSpaceWidth;
	}

	void GlFont::setTabWidth(float width)
	{
		mTabWidth = width;
	}
	float GlFont::getTabWidth() const
	{
		return mTabWidth;
	}
	float GlFont::getVariableTabPosition(float xPos) const
	{
		int xMult = static_cast<int>(xPos) / static_cast<int>(getTabWidth());
		float nextXpos = static_cast<float>(xMult + 1) * getTabWidth();
		if (nextXpos - xPos < getSpaceWidth())
		{
			nextXpos += getSpaceWidth();
		}
		return nextXpos;
	}

	IGfxEngine *GlFont::getGfxEngine()
	{
		return mGfxEngine;
	}
	GlGfxEngine *GlFont::getGlGfxEngine()
	{
		return mGfxEngine;
	}

	void GlFont::measureText(const char *text, float &width, float &height) const
	{
		if (text == NULL || text[0] <= ' ')
		{
			return;
		}
		int index = 0;
		char ch = text[index];
		float currWidth = -mKerning;

		width = 0.0f;
		height = mCharHeight;

		while (ch > '\0')
		{
			if (ch == ' ')
			{
				currWidth += mSpaceWidth;
			}
			else if (ch == '\t')
			{
				currWidth += getVariableTabPosition(currWidth);
			}
			else if (ch == '\n')
			{
				width = max(width, currWidth);
				height += mCharHeight + mLeading;
			}
			else
			{
				currWidth += mTextureWindows[ch].getWidth() + mKerning;
			}
			ch = text[++index];
		}
		width = max(width, currWidth);
	}
	void GlFont::measureWord(const char *word, float &width, float &height) const
	{
		if (word == NULL || word[0] <= ' ')
		{
			return;
		}
		int index = 0;
		char ch = word[index];
		width = -mKerning;

		while (ch > ' ')
		{
			width += mTextureWindows[ch].getWidth() + mKerning;
			ch = word[++index];
		}
	}

	void GlFont::setCharsAcross(int across)
	{
		mCharsAcross = across;
		mCharsDown = 256 / across;
	}
	int GlFont::getCharsAcross() const
	{
		return mCharsAcross;
	}
	int GlFont::getCharsDown() const
	{
		return mCharsDown;
	}

	void GlFont::getTextureWindow(char ch, TextureWindow &render)
	{
		render = mTextureWindows[ch];
	}

	void GlFont::postLoad()
	{
		if (mAsset == NULL || mAsset->getGlTexture() == NULL)
		{
			return;
		}
		TextureWindow render;

		const GlTexture *texture = mAsset->getGlTexture();
		float assetWidth = mAsset->getTextureWindow().getWidth();
		float assetHeight = mAsset->getTextureWindow().getHeight();

		float assetLeft = mAsset->getTextureWindow().getLeftX();
		float assetTop = mAsset->getTextureWindow().getTopY();
		if (!mUtfSupport)
		{
			mCharHeight = assetHeight / static_cast<float>(mCharsDown);
			mFixedCharWidth = assetWidth / static_cast<float>(mCharsAcross);
			if (mFixedWidth)
			{
				for (int y = 0; y < mCharsDown; y++)
				{
					for (int x = 0; x < mCharsAcross; x++)
					{
						render.mWidth = mFixedCharWidth;
						render.mHeight = mCharHeight;
						float uvWidth = render.mWidth / texture->getWidth();
						float uvHeight = render.mHeight / texture->getHeight();
						render.mLeftX = static_cast<float>(x) * uvWidth + assetLeft;
						render.mTopY =  static_cast<float>(y) * uvHeight + assetTop;
						render.mRightX = render.mLeftX + uvWidth;
						render.mBottomY = render.mTopY + uvHeight;


						mTextureWindows.push_back(render);
					}
				}
			}
			else
			{
				int *tempData = new int[texture->getWidth() * texture->getHeight()];
				glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
				glGetTexImage(GL_TEXTURE_2D, 0, texture->getGlFormat(), GL_UNSIGNED_BYTE, tempData);
				for (int y = 0; y < mCharsDown; y++)
				{
					for (int x = 0; x < mCharsAcross; x++)
					{
						pair<float, float> pos = processChar(tempData, x, y);

						render.mWidth = (pos.second - pos.first);
						render.mHeight = mCharHeight;
						float uvWidth = render.mWidth / texture->getWidth();
						float uvHeight = render.mHeight / texture->getHeight();
						render.mLeftX = pos.first / texture->getWidth();
						render.mTopY = static_cast<float>(y) * uvHeight + assetTop;
						render.mRightX = render.mLeftX + uvWidth;
						render.mBottomY = render.mTopY + uvHeight;

						mTextureWindows.push_back(render);
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

		int width = static_cast<int>(mAsset->getGlTexture()->getWidth());

		int xStart = xPos * static_cast<int>(mFixedCharWidth);
		xStart += static_cast<int>(mAsset->getTextureWindow().getLeftX() *
			mAsset->getGlTexture()->getWidth());
		int xEnd = xStart + static_cast<int>(mFixedCharWidth);

		int yStart = yPos * static_cast<int>(mCharHeight);
		yStart += static_cast<int>(mAsset->getTextureWindow().getTopY() *
			mAsset->getGlTexture()->getHeight());
		int yEnd = yStart + static_cast<int>(mCharHeight);

		for (int x = xStart; x < xEnd; x++)
		{
			for (int y = yStart; y < yEnd; y++)
			{
				unsigned int pixel = data[y * width + x];
				unsigned int alpha = pixel & 0xFF000000;
				if (alpha > 0x05000000)
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
			left = static_cast<float>(xStart);
			right = static_cast<float>(xEnd) - 1;
		}

		return pair<float, float>(left, right);
	}

}
}
