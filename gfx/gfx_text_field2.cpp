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
#include "gfx_node_hitbox.h"

namespace am {
namespace gfx {

	TextField2::TextField2() :
		Layer(),
		mRawText(""),
		mMeasuredWidth(0),
		mMeasuredHeight(0),
		mRenderedHeight(0),
		mDirty(true),
		mTextDirty(true),
		mNewLineDirty(true),
		mAlignment(ALIGN_LEFT),
		mRootNode(new Node("body"))
	{
		mFont = GfxEngine::getEngine()->getFont("basic");
		setInteractive(true);
	}

	TextField2::~TextField2()
	{

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
		return mText.c_str();
	}
	const char *TextField2::getRawText() const
	{
		return mRawText.c_str();
	}

	int TextField2::length()
	{
		if (mTextDirty)
		{
			parseRawText();
		}
		return static_cast<int>(mText.length());
	}
	int TextField2::rawLength() const
	{
		return static_cast<int>(mRawText.length());
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
			if (mNewLineDirty)
			{
				Node::NodeHitboxList &list = mCurrentNode->getHitboxes();
				list.clear();
				Handle<NodeHitbox> hitbox(new NodeHitbox(mCurrentNode));
				addChild(hitbox);
				hitbox->setPosition(mCurrXpos, mCurrYpos);
				hitbox->setHeight(mFont->getCharHeight());
				hitbox->addEventListener(MOUSE_UP, this);
				list.push_back(hitbox);
			}
			TextStyle currentStyle = mCurrentNode->getTextStyle();
			GfxEngine::getEngine()->popColourStack();
			if (currentStyle.hasColour())
			{
				GfxEngine::getEngine()->pushColourStack(currentStyle.getColour());
			}
			else
			{
				GfxEngine::getEngine()->pushColourStack(mColour);
			}
			GfxEngine::getEngine()->applyColourStack();
			renderText(mCurrentNode->getText());
			mCurrentNode = mCurrentNode->nextSibling();
		}
		mNewLineDirty = false;

		ChildList::iterator iter;
		for (iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			mColour.applyColour();
			(*iter)->render(dt);
		}

		postRender(dt);
	}

	void TextField2::calcSize()
	{
		if (mTextDirty)
		{
			parseRawText();
		}
		mFont->measureText(mText.c_str(), mWidth, mMeasuredWidth, mMeasuredHeight);
		mDirty = false;
	}

	void TextField2::preRender(float dt)
	{
		Renderable::preRender(dt);

		glBindTexture(GL_TEXTURE_2D, mFont->getAsset()->getTexture()->getTextureId());
		glBegin(GL_QUADS);

		mTextPosition = 0;

		if (mTextDirty)
		{
			parseRawText();
		}

		mCurrXpos = 0.0f;
		mCurrYpos = 0.0f;
		checkAlignment(mText.c_str());
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
		int line = localY / static_cast<int>(mFont->getCharHeight() + mFont->getLeading());
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
		return index;
	}

	void TextField2::newLine()
	{
		mCurrXpos = 0.0f;
		mCurrYpos += mFont->getCharHeight() + mFont->getLeading();
		if (mNewLineDirty)
		{
			mNewLinePositions.push_back(mTextPosition);

			Handle<NodeHitbox> hitbox(new NodeHitbox(mCurrentNode));
			addChild(hitbox);
			hitbox->setWidth(0);
			hitbox->setHeight(mFont->getCharHeight());
			hitbox->setPosition(mCurrXpos, mCurrYpos);
			hitbox->addEventListener(MOUSE_UP, this);
			mCurrentNode->getHitboxes().push_back(hitbox);
		}
	}
	void TextField2::renderText(const string &text)
	{
		int len = static_cast<int>(text.size());
		for (int i = 0; i < len; i++, mTextPosition++)
		{
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
			
			TextureWindow charRender;
			mFont->getTextureWindow(ch, charRender);

			glTexCoord2f(charRender.getLeftX(), charRender.getTopY());
			glVertex2f(mCurrXpos, mCurrYpos);
				
			glTexCoord2f(charRender.getRightX(), charRender.getTopY());
			glVertex2f(mCurrXpos + charRender.getWidth(), mCurrYpos);

			glTexCoord2f(charRender.getRightX(), charRender.getBottomY());
			glVertex2f(mCurrXpos + charRender.getWidth(), mCurrYpos + charRender.getHeight());

			glTexCoord2f(charRender.getLeftX(), charRender.getBottomY());
			glVertex2f(mCurrXpos, mCurrYpos + charRender.getHeight());

			mCurrXpos += charRender.getWidth();
			if (text[i + 1] > ' ')
			{
				mCurrXpos += mFont->getKerning();
			}
			if (mNewLineDirty)
			{
				Node::NodeHitboxList &list = mCurrentNode->getHitboxes();
				Handle<Renderable> hitbox = list[list.size() - 1];
				hitbox->setWidth(mCurrXpos - hitbox->getPositionX());
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
		mText.clear();
		mNewLinePositions.clear();
		mNewLinePositions.push_back(0);
		mNewLineDirty = true;
		bool firstLine = true;

		enum ParseState {
			BASE, START_FORMAT, END_FORMAT, IN_FORMAT_ATTR, IN_FORMAT_OP, IN_FORMAT_VALUE, POP_FORMAT
		};
		ParseState state = BASE;
		
		Handle<Node> currentNode = mRootNode;
		mRootNode->clear();

		string attrName = "";

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
				while (token != NULL && token[0] != '>')
				{
					token = tokeniser.nextToken();
				}
				state = BASE;
			}
			else if (state == POP_FORMAT)
			{
				string lower = Utils::toLowerCase(token);
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
				currentNode->appendText(token);
				mText += token;
			}
			if (state != END_FORMAT)
			{
				token = tokeniser.nextToken();
			}
		}

		mTextDirty = false;
	}
	
}
}
