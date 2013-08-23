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
		mRootNode(new Node("body")),
		mLineScroll(0),
		mDisplayNumLines(0)
	{
		mFont = GfxEngine::getEngine()->getFont("default:basic");
		setInteractive(true);

		mTextHitboxes = new Layer();
		mTextHitboxes->setInteractive(true);
		addChild(mTextHitboxes);

		mScrollbar = new Scrollbar("ui:scrollbar_up", "ui:scrollbar_down", "ui:scrollbar_bar", "ui:scrollbar_back");
		mScrollbar->setHeight(100.0f);
		mScrollbar->setVisible(false);
		addChild(mScrollbar);
		mScrollbar->addEventListener(Scrollbar::SCROLL_VALUE_CHANGE, this);
	}

	TextField2::~TextField2()
	{
		mCurrentNode = nullptr;
		//mRootNode = nullptr;
		if (mRootNode.get())
		{
			mRootNode->deinit();
		}
		mRootNode = nullptr;
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
		setTextDirty(true);
		mDirty = true;
	}
	void TextField2::setText(const string &str)
	{
		mRawText = str;
		mDirty = true;
		setTextDirty(true);
	}
	void TextField2::appendText(const char *str)
	{
		mRawText.append(str);
		mDirty = true;
		setTextDirty(true);
	}
	void TextField2::appendText(const string &str)
	{
		mRawText.append(str);
		mDirty = true;
		setTextDirty(true);
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

	int TextField2::getTotalNumLines()
	{
		if (mTextDirty)
		{
			parseRawText();
		}
		return static_cast<int>(mNewLinePositions.size());
	}

	int TextField2::getLineScroll() const
	{
		return mLineScroll;
	}
	void TextField2::setLineScroll(int lineScroll, bool silent)
	{
		mLineScroll = lineScroll;
		checkScrollbar();
		float scroll = -(mFont->getCharHeight() + mFont->getLeading()) * static_cast<float>(lineScroll);
		mTextHitboxes->setPosition(0.0f, scroll);
		if (!silent)
		{
			mScrollbar->setValue(mLineScroll);
		}
	}

	int TextField2::getDisplayNumLines() const
	{
		return mDisplayNumLines;
	}
	void TextField2::setDisplayNumLines(int numLines)
	{
		mDisplayNumLines = numLines;
		checkScrollbar();
	}

	float TextField2::getDisplayHeight() const
	{
		if (mFont != nullptr)
		{
			return mFont->getCharHeight() * mDisplayNumLines;
		}
		return 0.0f;
	}
	void TextField2::setDisplayHeight(float height)
	{
		if (mFont != nullptr)
		{
			mDisplayNumLines = static_cast<int>(height / mFont->getCharHeight());
		}
		else
		{
			mDisplayNumLines = static_cast<int>(height);
		}
		checkScrollbar();
	}

	void TextField2::onEvent(Event *e)
	{
		if (e->getEventTarget() == mScrollbar.get())
		{
			setLineScroll(mScrollbar->getValue(), true);
		}
	}

	void TextField2::render(float dt)
	{
		if (!mVisible || mFont == nullptr || !mFont->isLoaded())
		{
			return;
		}

		mCurrentLine = 0;
		preRender(dt);
		mFont->getAsset()->getTexture()->bindTexture();
		glBegin(GL_QUADS);
		while (mCurrentNode.get() != nullptr)
		{
			if (mNewLineDirty)
			{
				Node::NodeHitboxList &list = mCurrentNode->getHitboxes();
				list.clear();
				Handle<NodeHitbox> hitbox(new NodeHitbox(mCurrentNode));
				mTextHitboxes->addChild(hitbox.get());
				hitbox->setPosition(mCurrXpos, mCurrYpos - mTextHitboxes->getPositionY());
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
				if (mGfxComponent)
				{
					GfxEngine::getEngine()->pushColourStack(mGfxComponent->getColour());
				}
				else
				{
					GfxEngine::getEngine()->pushColourStack(Colour::WHITE);
				}
			}
			GfxEngine::getEngine()->applyColourStack();
			renderText(mCurrentNode->getText());
			mCurrentNode = mCurrentNode->nextSibling();
		}
		glEnd();
		mScrollbar->setMaxValue(getTotalNumLines() - mDisplayNumLines - 1);
		mNewLineDirty = false;

		for (ChildList::iterator iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			if (mGfxComponent)
			{
				mGfxComponent->getColour().applyColour();
			}
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

		mTextPosition = 0;

		if (mTextDirty)
		{
			parseRawText();
		}

		mCurrXpos = 0.0f;
		mCurrYpos = -mLineScroll * (mFont->getCharHeight() + mFont->getLeading());
		checkAlignment(mText.c_str());
		mInWord = false;

		mCurrentNode = mRootNode->firstChild();
	}

	void TextField2::postRender(float dt)
	{
		mRenderedHeight = mCurrYpos + mFont->getCharHeight();
		if (mScrollbar.get() != nullptr && mScrollbar->isVisible())
		{
			mScrollbar->render(dt);
		}

		Renderable::postRender(dt);
	}

	int TextField2::getTextPosition(int localX, int localY) const
	{
		int line = localY / static_cast<int>(mFont->getCharHeight() + mFont->getLeading());
		if (line < 0)
		{
			return -1;
		}
		if (line >= static_cast<int>(mNewLinePositions.size()))
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
		mCurrentLine++;
		mCurrYpos += mFont->getCharHeight() + mFont->getLeading();
		if (mNewLineDirty)
		{
			mNewLinePositions.push_back(mTextPosition);

			Handle<NodeHitbox> hitbox(new NodeHitbox(mCurrentNode));
			mTextHitboxes->addChild(hitbox);
			hitbox->setWidth(0);
			hitbox->setHeight(mFont->getCharHeight());
			hitbox->setPosition(mCurrXpos, mCurrYpos - mTextHitboxes->getPositionY());
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

			bool display = mCurrentLine >= mLineScroll && (mCurrentLine < (mLineScroll + mDisplayNumLines) || mDisplayNumLines == 0);
			if (display)
			{
				glTexCoord2f(charRender.getLeftX(), charRender.getTopY());
				glVertex2f(mCurrXpos, mCurrYpos);
				
				glTexCoord2f(charRender.getRightX(), charRender.getTopY());
				glVertex2f(mCurrXpos + charRender.getWidth(), mCurrYpos);

				glTexCoord2f(charRender.getRightX(), charRender.getBottomY());
				glVertex2f(mCurrXpos + charRender.getWidth(), mCurrYpos + charRender.getHeight());

				glTexCoord2f(charRender.getLeftX(), charRender.getBottomY());
				glVertex2f(mCurrXpos, mCurrYpos + charRender.getHeight());
			}
			
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
			mScrollbar->setPosition(width - mScrollbar->getWidth(), 0.0f);
		}
	}
	void TextField2::setHeight(float height)
	{
		if (height != mHeight)
		{
			mDirty = true;
			Renderable::setHeight(height);
			mScrollbar->setHeight(height);
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
		TextTokeniser tokeniser(mRawText.c_str());
		const char *token = tokeniser.nextToken();
		mText = "";
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

		mTextHitboxes->clear();

		string attrName = "";

		while (token != nullptr)
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
				while (token != nullptr && token[0] != '>')
				{
					token = tokeniser.nextToken();
				}
				state = BASE;
			}
			else if (state == POP_FORMAT)
			{
				string lower = Utils::toLowerCase(token);
				if (currentNode->getParent() == nullptr)
				{
					am_log("NODE", "Pop format to nullptr parent");
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

		setTextDirty(false);
	}

	void TextField2::checkScrollbar()
	{
		mScrollbar->setVisible(mLineScroll != 0 || mDisplayNumLines != 0);
	}

	void TextField2::setTextDirty(bool dirty)
	{
		mTextDirty = dirty;
	}
	
}
}

