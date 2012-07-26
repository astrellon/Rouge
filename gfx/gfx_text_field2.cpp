#include "gfx_text_field2.h"

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

	TextField2::TextField2() :
		Layer(),
		//mText(""),
		mRawText(""),
		mMeasuredWidth(0),
		mMeasuredHeight(0),
		mRenderedHeight(0),
		mDirty(true),
		mTextDirty(true),
		mAlignment(ALIGN_LEFT),
		mRootNode(new Node("body"))
	{
		mFont = GfxEngine::getEngine()->getFont("basic");
		addEventListener(MOUSE_DOWN, this);
	}

	TextField2::~TextField2()
	{

	}

	void TextField2::onEvent(MouseEvent *e)
	{
		stringstream ss;
		ss << "Click at text index " << getTextPosition(e->getLocalMouseX(), e->getLocalMouseY());
		am_log("TEXT", ss);
	}

	float TextField2::getRenderedHeight() const
	{
		return mRenderedHeight;
	}

	Font *TextField2::getBaseFont()
	{
		return mFont;
	}
	void TextField2::setBaseFont(Font *font)
	{
		mFont = font;
	}
	void TextField2::setBaseFont(const char *fontName)
	{
		mFont = GfxEngine::getEngine()->getFont(fontName);
	}

	float TextField2::getMeasuredWidth()
	{
		if (mDirty)
		{
			calcSize();
		}
		return mMeasuredWidth;
	}
	float TextField2::getMeasuredHeight()
	{
		if (mDirty)
		{
			calcSize();
		}
		return mMeasuredHeight;
	}

	void TextField2::setText(const char *str)
	{
		mRawText = str;
		mTextDirty = true;
		mDirty = true;
	}
	void TextField2::setText(const string &str)
	{
		mRawText = str;
		mDirty = true;
		mTextDirty= true;
	}
	void TextField2::appendText(const char *str)
	{
		mRawText.append(str);
		mDirty = true;
		mTextDirty = true;
	}
	void TextField2::appendText(const string &str)
	{
		mRawText.append(str);
		mDirty = true;
		mTextDirty = true;
	}
	const char *TextField2::getText()
	{
		if (mTextDirty)
		{
			parseRawText();
		}
		//return mText.c_str();
		return mRawText.c_str();
	}
	const char *TextField2::getRawText() const
	{
		return mRawText.c_str();
	}

	void TextField2::setAlignment(TextField2::TextAlignment align)
	{
		mAlignment = align;
	}
	TextField2::TextAlignment TextField2::getAlignment() const
	{
		return mAlignment;
	}

	void TextField2::render(float dt)
	{
		if (!mVisible || mFont == NULL || !mFont->isLoaded())
		{
			return;
		}

		preRender(dt);
		while (mCurrentNode.get() != NULL)
		{
			mCurrentStyle = mCurrentNode->getTextStyle();
			GfxEngine::getEngine()->popColourStack();
			if (mCurrentStyle.hasColour())
			{
				GfxEngine::getEngine()->pushColourStack(mCurrentStyle.getColour());
			}
			else
			{
				GfxEngine::getEngine()->pushColourStack(mColour);
			}
			GfxEngine::getEngine()->applyColourStack();
			renderText(mCurrentNode->getText());
			mCurrentNode = mCurrentNode->nextSibling();
		}
		//renderText(mText);
		postRender(dt);
	}

	void TextField2::calcSize()
	{
		if (mTextDirty)
		{
			parseRawText();
		}
		//mFont->measureText(mText.c_str(), mWidth, mMeasuredWidth, mMeasuredHeight);
		mDirty = false;
	}

	void TextField2::preRender(float dt)
	{
		Renderable::preRender(dt);

		glBindTexture(GL_TEXTURE_2D, mFont->getAsset()->getTexture()->getTextureId());

		glBegin(GL_QUADS);

		//mColourStack.clear();
		//mColourStack.push_back(mColour);

		//mStylePosition = 0;
		mTextPosition = 0;

		if (mTextDirty)
		{
			parseRawText();
		}

		mCurrXpos = 0.0f;
		mCurrYpos = 0.0f;
		//checkAlignment(mText.c_str());
		mInWord = false;

		mCurrentNode = mRootNode->firstChild();
	}

	void TextField2::postRender(float dt)
	{
		glEnd();

		mRenderedHeight = mCurrYpos + mFont->getCharHeight();
		glBindTexture(GL_TEXTURE_2D, 0);
		
		Renderable::postRender(dt);
	}

	int TextField2::getTextPosition(int localX, int localY) const
	{
		/*int line = localY / static_cast<int>(mFont->getCharHeight() + mFont->getLeading());
		if (line < 0)
		{
			return -1;
		}
		if (line >= mNewLinePositions.size())
		{
			return -2;
		}
		int index = mNewLinePositions[line];
		char ch = mText[index];
		float left = -mFont->getKerning();
		while (ch != '\0' && ch != '\n' && localX > left)
		{
			left += mFont->getCharWidth(ch) + mFont->getKerning();
			ch = mText[++index];
		}
		return index;*/
		return -1;
	}

	void TextField2::newLine()
	{
		mCurrXpos = 0.0f;
		mCurrYpos += mFont->getCharHeight() + mFont->getLeading();
	}
	void TextField2::renderText(const string &text)
	{
		int len = static_cast<int>(text.size());
		for (int i = 0; i < len; i++, mTextPosition++)
		{
			/*while (mStylePosition < mStyles.size())
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
						else if (style.getGameObjId() > 0)
						{
							
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
					else if (style.getGameObjId() > 0)
					{

					}
					mStylePosition++;
				}
				else
				{
					break;
				}
			}*/
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

	void TextField2::setWidth(float width)
	{
		if (width != mWidth)
		{
			mDirty = true;
			Renderable::setWidth(width);
		}
	}
	void TextField2::setHeight(float height)
	{
		if (height != mHeight)
		{
			mDirty = true;
			Renderable::setHeight(height);
		}
	}

	void TextField2::checkAlignment(const char *line)
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
	/*
	void TextField2::clearAllStyles()
	{
		mStyles.clear();
	}
	void TextField2::addTextStyle(const TextStyle &style)
	{
		mStyles.push_back(style);
	}
	void TextField2::removeStyleAt(int index)
	{
		if (index < mStyles.size())
		{
			mStyles.erase(mStyles.begin() + index);
		}
	}
	int TextField2::getNumStyles() const
	{
		return static_cast<int>(mStyles.size());
	}
	bool TextField2::getStyleAt(int index, TextStyle &style)
	{
		if (index < mStyles.size() && index >= 0)
		{
			style = mStyles[index];
			return true;
		}
		return false;
	}
	*/
	Node *TextField2::getRootNode()
	{
		return mRootNode;
	}

	void TextField2::parseRawText()
	{
		mNewLinePositions.clear();
		mNewLinePositions.push_back(0);
		TextTokeniser tokeniser(mRawText.c_str());
		const char *token = tokeniser.nextToken();

		enum ParseState {
			BASE, START_FORMAT, END_FORMAT, IN_FORMAT_ATTR, IN_FORMAT_OP, IN_FORMAT_VALUE, POP_FORMAT
		};
		ParseState state = BASE;
		//mStyles.clear();
		//mNodes.clear();
		//mNodes.push_back(new Node("body"));
		Handle<Node> currentNode = mRootNode;
		mRootNode->clear();

		//vector<TextStyleSelector> nodes;
		//nodes.push_back(TextStyleSelector());
		//TextStyleSelector &currentNode = nodes.back();

		string attrName = "";

		//mStyles.push_back(TextStyle());
		//TextStyle &currentStyle = mStyles.back();

		while (token != NULL)
		{
			if (state == BASE && token[0] == '<')
			{
				state = START_FORMAT;
			}
			else if (state != BASE && token[0] == '>')
			{
				state = BASE;
			}
			else if (state == START_FORMAT)
			{
				if (token[0] == '/')
				{
					state = POP_FORMAT;
				}
				else 
				{
					string lower = Utils::toLowerCase(token);
					//nodes.push_back(TextStyleSelector(lower.c_str()));
					//mNodes.push_back(new Node(lower.c_str()));
					//currentNode = mNodes.back();
					Node *newNode = new Node(lower.c_str());
					currentNode->addChild(newNode);
					currentNode = newNode;
					state = IN_FORMAT_ATTR;
				}
			}
			else if (state == IN_FORMAT_ATTR)
			{
				if (token[0] == '>')
				{
					state = END_FORMAT;
				}
				attrName = Utils::toLowerCase(token);
				state = IN_FORMAT_OP;
			}
			else if (state == IN_FORMAT_OP)
			{
				if (token[0] == '>')
				{
					state = END_FORMAT;
				}
				else if (token[0] != '=')
				{
					currentNode->setAttribute(attrName.c_str(), "true");
					state = IN_FORMAT_ATTR;
				}
				else
				{
					state = IN_FORMAT_VALUE;
				}
			}
			else if (state == IN_FORMAT_VALUE)
			{
				if (token[0] == '>')
				{
					stringstream ss;
					ss << "Error parsing node '" << attrName << "', = operator had not value";
					am_log("TEXT", ss);
				}
				else
				{
					currentNode->setAttribute(attrName.c_str(), token);
					state = IN_FORMAT_ATTR;
				}
			}
			else if (state == END_FORMAT)
			{
				//currentStyle = TextStyle::getCalcStyle(currentNode);
				while (token != NULL && token[0] != '>')
				{
					token = tokeniser.nextToken();
				}
				state = BASE;
			}
			else if (state == POP_FORMAT)
			{
				string lower = Utils::toLowerCase(token);
				//nodes.push_back(TextStyleSelector());
				//nodes.pop_back();
				//mNodes.pop_back();
				//currentNode = mNodes.back();
				if (currentNode->getParent() == NULL)
				{
					am_log("NODE", "Pop format to NULL parent");
				}
				else
				{
					currentNode = currentNode->getParent();
				}
				state = END_FORMAT;
			}
			else if (state == BASE)
			{
				//mText += token;
				currentNode->appendText(token);
			}
			if (state != END_FORMAT)
			{
				token = tokeniser.nextToken();
			}
		}

		mTextDirty = false;
	}
	/*void TextField2::parseRawText()
	{
		mText = "";
		mNewLinePositions.clear();
		mNewLinePositions.push_back(0);
		TextTokeniser tokeniser(mRawText.c_str());
		const char *token = tokeniser.nextToken();

		enum ParseState {
			BASE, START_FORMAT, END_FORMAT, IN_FORMAT, POP_FORMAT, 
			IN_COLOUR, IN_GAME_OBJ
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
					else if (lower.compare("gameobj") == 0)
					{
						state = IN_GAME_OBJ;
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
				//mStyles.push_back(TextStyle(c, static_cast<int>(mText.size())));
				state = BASE;
			}
			else if (state == IN_GAME_OBJ)
			{

			}
			else if (state == POP_FORMAT)
			{
				string lower = Utils::toLowerCase(token);
				if (lower.compare("colour") == 0)
				{
					//mStyles.push_back(TextStyle(TextStyle::COLOUR, static_cast<int>(mText.size())));
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
				char n = token[0];
				int offset = mText.length();
				int i = 0;
				while (n != '\0')
				{
					if (n == '\n')
					{
						mNewLinePositions.push_back(1 + i + offset);
					}
					n = token[++i];
				}
				mText += token;
			}
			token = tokeniser.nextToken();
		}

		mTextDirty = false;
	}*/

}
}
