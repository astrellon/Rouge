#include "gfx_text_field.h"

#include <util/text_tokeniser.h>
#include <util/utils.h>
using namespace am::util;

#include <log/logger.h>

#include <gl.h>

#include "gfx_engine.h"
#include "gfx_texture.h"
#include "gfx_font.h"
#include "gfx_asset.h"
#include "gfx_layer.h"

namespace am {
namespace gfx {

	TextField::TextField() :
		Renderable(),
		mText(""),
		mRawText(""),
		mMeasuredWidth(0),
		mMeasuredHeight(0),
		mRenderedHeight(0),
		mDirty(true),
		mTextDirty(true),
		mAlignment(ALIGN_LEFT)
	{
		//mTransform.setUpDirection(am::math::Transform::REF_FORWARD);
		mFont = GfxEngine::getEngine()->getFont("basic");
	}

	TextField::~TextField()
	{

	}

	float TextField::getRenderedHeight() const
	{
		return mRenderedHeight;
	}

	Font *TextField::getBaseFont()
	{
		return mFont;
	}
	void TextField::setBaseFont(Font *font)
	{
		mFont = font;
	}
	void TextField::setBaseFont(const char *fontName)
	{
		mFont = GfxEngine::getEngine()->getFont(fontName);
	}

	float TextField::getMeasuredWidth()
	{
		if (mDirty)
		{
			calcSize();
		}
		return mMeasuredWidth;
	}
	float TextField::getMeasuredHeight()
	{
		if (mDirty)
		{
			calcSize();
		}
		return mMeasuredHeight;
	}

	void TextField::setText(const char *str)
	{
		mRawText = str;
		mTextDirty = true;
		mDirty = true;
	}
	void TextField::setText(const string &str)
	{
		mRawText = str;
		mDirty = true;
		mTextDirty= true;
	}
	void TextField::appendText(const char *str)
	{
		mRawText.append(str);
		mDirty = true;
		mTextDirty = true;
	}
	void TextField::appendText(const string &str)
	{
		mRawText.append(str);
		mDirty = true;
		mTextDirty = true;
	}
	const char *TextField::getText()
	{
		if (mTextDirty)
		{
			parseRawText();
		}
		return mText.c_str();
	}
	const char *TextField::getRawText() const
	{
		return mRawText.c_str();
	}

	void TextField::setAlignment(TextField::TextAlignment align)
	{
		mAlignment = align;
	}
	TextField::TextAlignment TextField::getAlignment() const
	{
		return mAlignment;
	}

	void TextField::render(float dt)
	{
		if (!mVisible || mFont == NULL || !mFont->isLoaded())
		{
			return;
		}

		preRender(dt);
		renderText(mText);
		postRender(dt);
	}

	void TextField::calcSize()
	{
		if (mTextDirty)
		{
			parseRawText();
		}
		mFont->measureText(mText.c_str(), mWidth, mMeasuredWidth, mMeasuredHeight);
		mDirty = false;
	}

	void TextField::preRender(float dt)
	{
		Renderable::preRender(dt);

		glBindTexture(GL_TEXTURE_2D, mFont->getAsset()->getTexture()->getTextureId());

		glBegin(GL_QUADS);

		mColourStack.clear();
		mColourStack.push_back(mColour);

		mStylePosition = 0;
		mTextPosition = 0;

		if (mTextDirty)
		{
			parseRawText();
		}

		mCurrXpos = 0.0f;
		mCurrYpos = 0.0f;
		checkAlignment(mText.c_str());
		mInWord = false;
	}

	void TextField::postRender(float dt)
	{
		glEnd();

		mRenderedHeight = mCurrYpos + mFont->getCharHeight();
		glBindTexture(GL_TEXTURE_2D, 0);
		
		Renderable::postRender(dt);
	}

	void TextField::newLine()
	{
		mCurrXpos = 0.0f;
		mCurrYpos += mFont->getCharHeight() + mFont->getLeading();
	}
	void TextField::renderText(const string &text)
	{
		int len = static_cast<int>(text.size());
		for (int i = 0; i < len; i++, mTextPosition++)
		{
			while (mStylePosition < mStyles.size())
			{
				const TextStyle &style = mStyles[mStylePosition];
				if (style.getPosition() <= mTextPosition)
				{
					if (style.getTextStylePop() == TextStyle::NONE)
					{
						if (style.hasColour())
						{
							GfxEngine *engine = GfxEngine::getEngine();
							engine->popColourStack();
							engine->pushColourStack(style.getColour());
							engine->applyColourStack();
							mColourStack.push_back(style.getColour());
						}
					}
					else if (style.getTextStylePop() == TextStyle::COLOUR)
					{
						if (mColourStack.size() > 1)
						{
							mColourStack.pop_back();
						}
						GfxEngine *engine = GfxEngine::getEngine();
						engine->popColourStack();
						engine->pushColourStack(mColourStack.back());
						engine->applyColourStack();
					}
					mStylePosition++;
				}
				else
				{
					break;
				}
			}
			char ch = text[i];
			if (ch <= ' ' && mInWord)
			{
				mInWord = false;
			}

			if (ch == ' ')
			{
				mCurrXpos += mFont->getSpaceWidth();
				continue;
			}
			else if(ch == '\t')
			{
				mCurrXpos = mFont->getVariableTabPosition(mCurrXpos);
				continue;
			}
			else if(ch == '\n')
			{
				newLine();
				checkAlignment(text.c_str() + i + 1);
				continue;
			}
			else if (mWidth > 0.0f && ch > ' ' && !mInWord)
			{
				mInWord = true;
				float wordWidth = 0.0f;
				float wordHeight = 0.0f;
				mFont->measureWord(text.c_str() + i, wordWidth, wordHeight);
				if (mCurrXpos + wordWidth > mWidth)
				{
					newLine();
					checkAlignment(text.c_str() + i);
				}
			}
				
			mFont->getTextureWindow(ch, mCharRender);

			glTexCoord2f(mCharRender.getLeftX(), mCharRender.getTopY());
			glVertex2f(mCurrXpos, mCurrYpos);
				
			glTexCoord2f(mCharRender.getRightX(), mCharRender.getTopY());
			glVertex2f(mCurrXpos + mCharRender.getWidth(), mCurrYpos);

			glTexCoord2f(mCharRender.getRightX(), mCharRender.getBottomY());
			glVertex2f(mCurrXpos + mCharRender.getWidth(), mCurrYpos + mCharRender.getHeight());

			glTexCoord2f(mCharRender.getLeftX(), mCharRender.getBottomY());
			glVertex2f(mCurrXpos, mCurrYpos + mCharRender.getHeight());

			mCurrXpos += mCharRender.getWidth();
			if (text[i + 1] > ' ')
			{
				mCurrXpos += mFont->getKerning();
			}
		}
	}

	void TextField::setWidth(float width)
	{
		if (width != mWidth)
		{
			mDirty = true;
			Renderable::setWidth(width);
		}
	}
	void TextField::setHeight(float height)
	{
		if (height != mHeight)
		{
			mDirty = true;
			Renderable::setHeight(height);
		}
	}

	void TextField::checkAlignment(const char *line)
	{
		if (mAlignment == ALIGN_RIGHT)
		{
			float width = 0.0f;
			float height = 0.0f;
			mFont->measureLine(line, mWidth, width, height);
			mCurrXpos = mWidth - width;
		}
		else if (mAlignment == ALIGN_CENTER)
		{
			float width = 0.0f;
			float height = 0.0f;
			mFont->measureLine(line, mWidth, width, height);
			mCurrXpos = (mWidth - width) / 2;
		}
	}

	void TextField::clearAllStyles()
	{
		mStyles.clear();
	}
	void TextField::addTextStyle(const TextStyle &style)
	{
		mStyles.push_back(style);
	}
	void TextField::removeStyleAt(int index)
	{
		if (index < mStyles.size())
		{
			mStyles.erase(mStyles.begin() + index);
		}
	}
	int TextField::getNumStyles() const
	{
		return static_cast<int>(mStyles.size());
	}
	bool TextField::getStyleAt(int index, TextStyle &style)
	{
		if (index < mStyles.size() && index >= 0)
		{
			style = mStyles[index];
			return true;
		}
		return false;
	}

	void TextField::parseRawText()
	{
		mText = "";
		TextTokeniser tokeniser(mRawText.c_str());
		const char *token = tokeniser.nextToken();

		enum ParseState {
			BASE, START_FORMAT, END_FORMAT, IN_FORMAT, POP_FORMAT, IN_COLOUR
		};
		ParseState state = BASE;
		mStyles.clear();

		while (token != NULL)
		{
			if (state == BASE && token[0] == '<')
			{
				state = IN_FORMAT;
			}
			else if (state != BASE && token[0] == '>')
			{
				state = BASE;
			}
			else if (state == IN_FORMAT)
			{
				if (token[0] == '/')
				{
					state = POP_FORMAT;
				}
				else 
				{
					string lower = Utils::toLowerCase(token);
					if (lower.compare("colour") == 0)
					{
						state = IN_COLOUR;
						continue;
					}
					else
					{
						stringstream ss;
						ss << "Unknown text node '" << token << "'";
						am_log("TEXT", ss);
						state = BASE;
					}
				}
			}
			else if (state == IN_COLOUR)
			{
				Colour c;
				c.parseFromTokeniser(tokeniser);
				
				token = tokeniser.currentToken();
				while (token && token[0] != '>')
				{
					token = tokeniser.nextToken();
				}
				mStyles.push_back(TextStyle(c, static_cast<int>(mText.size())));
				state = BASE;
			}
			else if (state == POP_FORMAT)
			{
				string lower = Utils::toLowerCase(token);
				if (lower.compare("colour") == 0)
				{
					mStyles.push_back(TextStyle(TextStyle::COLOUR, static_cast<int>(mText.size())));
				}
				else
				{
					stringstream ss;
					ss << "Unable to pop tag of unknown node '" << token << "'";
					am_log("TEXT", ss);
				}
				state = END_FORMAT;
			}
			else if (state == BASE)
			{
				mText += token;
			}
			token = tokeniser.nextToken();
		}

		mTextDirty = false;
	}

}
}
