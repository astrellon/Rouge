#include "gfx_font.h"

#include "gfx_texture.h"
#include "gfx_asset.h"
#include "gfx_engine.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace gfx {

	const int Font::LUA_ID = LUA_ID_FONT;
	const char *Font::LUA_TABLENAME = LUA_TABLE_FONT;

	Font::Font(const char *name) :
		am::base::IManaged(),
		mAsset(nullptr),
		mName(name),
		mKerning(1.0f),
		mLeading(1.0f),
		mSpaceWidth(6.0f),
		mTabWidth(24.0f),
		mCharsAcross(16),
		mCharsDown(16),
		mUtfSupport(false)
	{

	}
	Font::~Font()
	{

	}

	Asset *Font::getAsset()
	{
		return mAsset;
	}
	void Font::setAsset(Asset *asset)
	{
		if (asset != nullptr)
		{
			mAsset = asset;
		}
	}

	bool Font::isLoaded() const
	{
		return mAsset != nullptr && mAsset->getTexture() != nullptr && !mTextureWindows.empty();
	}

	string Font::getName() const
	{
		return mName;
	}

	void Font::setFixedWidth(bool fixed)
	{
		mFixedWidth = fixed;
	}
	bool Font::isFixedWidth() const
	{
		return mFixedWidth;
	}

	float Font::getCharHeight() const
	{
		return mCharHeight;
	}
	float Font::getFixedCharWidth() const
	{
		return mFixedCharWidth;
	}

	void Font::setKerning(float kerning)
	{
		mKerning = kerning;
	}
	float Font::getKerning() const
	{
		return mKerning;
	}

	void Font::setLeading(float leading)
	{
		mLeading = leading;
	}
	float Font::getLeading() const
	{
		return mLeading;
	}

	void Font::setSpaceWidth(float width)
	{
		mSpaceWidth = width;
	}
	float Font::getSpaceWidth() const
	{
		return mSpaceWidth;
	}

	void Font::setTabWidth(float width)
	{
		mTabWidth = width;
	}
	float Font::getTabWidth() const
	{
		return mTabWidth;
	}

	void Font::setUtf(bool utf)
	{
		mUtfSupport = utf;
	}
	bool Font::isUtf() const
	{
		return mUtfSupport;
	}

	float Font::getVariableTabPosition(float xPos) const
	{
		int xMult = static_cast<int>(xPos) / static_cast<int>(getTabWidth());
		float nextXpos = static_cast<float>(xMult + 1) * getTabWidth();
		if (nextXpos - xPos < getSpaceWidth())
		{
			nextXpos += getSpaceWidth();
		}
		return nextXpos;
	}

	int Font::measureText(const char *text, float containerWidth, float &width, float &height, char endChar) const
	{
		if (text == nullptr || text[0] <= ' ')
		{
			return 0;
		}
		int index = 0;
		char ch = text[index];
		float currWidth = 0.0f;

		width = 0.0f;
		height = mCharHeight;

		while (ch > '\0' && ch != endChar)
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
				currWidth = 0.0f;
			}
			else
			{
				if (containerWidth > 0)
				{
					float wordWidth;
					float wordHeight;
					index += measureWord(text + index, wordWidth, wordHeight) - 1;
					if (currWidth + wordWidth > containerWidth)
					{
						height += wordHeight + mLeading;
						width = max(width, currWidth);
						currWidth = 0.0f;
					}
					else
					{
						currWidth += wordWidth;
					}
				}
				else
				{
					currWidth += mTextureWindows[ch].getWidth() + mKerning;
				}
			}
			ch = text[++index];
		}
		width = max(width, currWidth);
		return index;
	}
	int Font::measureLine(const char *word, float containerWidth, float &width, float &height) const
	{
		if (word == nullptr || word[0] <= ' ')
		{
			return 0;
		}
		int index = 0;
		char ch = word[index];
		
		float currWidth = 0.0f;

		width = 0.0f;
		height = mCharHeight;

		while (ch > '\0' && ch <= ' ')
		{
			ch = word[++index];
		}
		if (ch == '\0')
		{
			return index;
		}

		bool useLetterWidth = false;
		float letterWidth = currWidth;
		char prevCh = '\0';
		while (ch > '\0')
		{
			if (ch == ' ')
			{
				if (prevCh > ' ')
				{
					letterWidth = currWidth;
					useLetterWidth = true;
				}
				currWidth += mSpaceWidth;
			}
			else if (ch == '\t')
			{
				if (prevCh > ' ')
				{
					letterWidth = currWidth;
					useLetterWidth = true;
				}
				currWidth += getVariableTabPosition(currWidth);
			}
			else if (ch == '\n')
			{
				break;
			}
			else
			{
				useLetterWidth = false;
				float wordWidth;
				float wordHeight;
				int indexChange = measureWord(word + index, wordWidth, wordHeight);
				if (currWidth + wordWidth > containerWidth)
				{
					break;
				}
				index += indexChange - 1;
				currWidth += wordWidth;
				
			}
			prevCh = word[index];
			ch = word[++index];
		}

		if (useLetterWidth)
		{
			width = letterWidth;
		}
		else
		{
			width = currWidth;
		}
		return index;
	}
	int Font::measureWord(const char *word, float &width, float &height) const
	{
		if (word == nullptr || word[0] <= ' ')
		{
			return 0;
		}
		int index = 0;
		char ch = word[index];
		width = -mKerning;
		height = mCharHeight;

		while (ch > ' ')
		{
			width += mTextureWindows[ch].getWidth() + mKerning;
			ch = word[++index];
		}
		return index;
	}

	void Font::setCharsAcross(int across)
	{
		mCharsAcross = across;
		mCharsDown = 256 / across;
	}
	int Font::getCharsAcross() const
	{
		return mCharsAcross;
	}
	int Font::getCharsDown() const
	{
		return mCharsDown;
	}

	void Font::getTextureWindow(unsigned char ch, TextureWindow &render)
	{
		render = mTextureWindows[ch];
	}

	void Font::process()
	{
		if (mAsset == nullptr || mAsset->getTexture() == nullptr)
		{
			return;
		}
		TextureWindow render;

		const Texture *texture = mAsset->getTexture();
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

	pair<float, float> Font::processChar(int *data, int xPos, int yPos)
	{
		float left = -1;
		float right = -1;

		int width = static_cast<int>(mAsset->getTexture()->getWidth());

		int xStart = xPos * static_cast<int>(mFixedCharWidth);
		xStart += static_cast<int>(mAsset->getTextureWindow().getLeftX() *
			mAsset->getTexture()->getWidth());
		int xEnd = xStart + static_cast<int>(mFixedCharWidth);

		int yStart = yPos * static_cast<int>(mCharHeight);
		yStart += static_cast<int>(mAsset->getTextureWindow().getTopY() *
			mAsset->getTexture()->getHeight());
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

	float Font::getCharWidth(unsigned char ch) const
	{
		return mTextureWindows[ch].getWidth();
	}

}
}
