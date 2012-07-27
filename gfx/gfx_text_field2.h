#pragma once

#include <vector>
using namespace std;

#include <ui/ievent_listener.h>
#include <ui/mouse_event.h>
using namespace am::ui;

#include "gfx_layer.h"
#include "texture_window.h"
#include "gfx_text_style.h"
#include "gfx_node.h"

namespace am {
namespace gfx {

	class Font;

	class TextField2 : public IEventListener, public Layer {
	public:
		// GlTextField methods
		enum TextAlignment {
			ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER
		};

		TextField2();
		~TextField2();

		virtual void onEvent(MouseEvent *e);

		float getRenderedHeight() const;

		Font *getBaseFont();
		void setBaseFont(Font *font);
		void setBaseFont(const char *fontName);
		
		void setText(const char *str);
		void setText(const string &str);
		void appendText(const char *str);
		void appendText(const string &str);
		const char *getText();
		const char *getRawText() const;

		void setAlignment(TextAlignment align);
		TextAlignment getAlignment() const;

		float getMeasuredWidth();
		float getMeasuredHeight();

		string getFirstLine() const;
		
		int getTextPosition(int localX, int localY) const;

		// Renderable methods
		virtual void render(float dt);

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		void parseRawText();
		Node *getRootNode();
		
	protected:

		string mText;
		string mRawText;
		Font *mFont;

		float mMeasuredWidth;
		float mMeasuredHeight;

		float mRenderedHeight;

		float mCurrXpos;
		float mCurrYpos;

		bool mInWord;
		bool mDirty;
		bool mTextDirty;
		bool mNewLineDirty;

		int mTextPosition;
		
		Handle<Node> mRootNode;
		Handle<Node> mCurrentNode;

		typedef vector<int> NewLineList;
		NewLineList mNewLinePositions;

		TextAlignment mAlignment;

		void calcSize();

		virtual void preRender(float dt);
		virtual void postRender(float dt);
		void newLine();
		void renderText(const string &text);
		void checkAlignment(const char *line);
	};

}
}
