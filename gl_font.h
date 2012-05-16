#pragma once

#include <string>
#include <vector>
#include "json_value.h"
#include "ifont.h"

#include "igl_gfx_component.h"

using namespace std;
using namespace am::util;

namespace am {
namespace gfx {

	class GlTexture;
	class GlGfxEngine;

	class GlFont : public IFont, public IGlGfxComponent {
	public:
		~GlFont();

		virtual ITexture *getTexture();
		GlTexture *getGlTexture();
		virtual void setTexture(ITexture *texture);

		virtual string getName() const;

		virtual int loadDef(JsonValue value);

		virtual void getCharRender(char ch, CharRender &render);

		virtual bool isLoaded() const;
		virtual bool isFixedWidth() const;

		virtual float getCharHeight() const;
		virtual float getFixedCharWidth() const;

		virtual IGfxEngine *getGfxEngine();
		virtual GlGfxEngine *getGlGfxEngine();

		friend class GlGfxEngine;

	protected:

		GlFont(const char *name);

		GlTexture *mTexture;
		GlGfxEngine *mGfxEngine;
		string mName;

		bool mFixedWidth;
		bool mUtfSupport;

		float mCharHeight;
		float mFixedCharWidth;

		vector<CharRender> mCharRenders;

		pair<float, float> processChar(int *data, int xPos, int yPos);
		void postLoad();

	};

}
}
