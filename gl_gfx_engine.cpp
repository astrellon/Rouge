#include "gl_gfx_engine.h"

#include <math.h>

#include "gl.h"
#include "IL/il.h"

#include "igl_renderable.h"
#include "gl_texture.h"
#include "gl_font.h"
#include "gl_text_field.h"

namespace am {
namespace gfx {
	GlGfxEngine::GlGfxEngine()
	{
		
	}
	GlGfxEngine::~GlGfxEngine()
	{

	}

	void GlGfxEngine::init()
	{
		// Initialize the DevIL framework.
		ilInit();

		glClearColor (0.0, 0.3, 0.4, 0.0);
		// Enable texture for the text fields.
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
	
		/*glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);*/

		loadFontDefinitions("data/fontDefs.ssff");
	
		GlTextField *text = new GlTextField(this);
		addObject(text);

		text->setBaseGlFont(getGlFont("basic"));
		text->setText(string("AJSDKLADSJLASDJLASD"));
	}
	void GlGfxEngine::deinit()
	{
		ilShutDown();
	}
	void GlGfxEngine::setOrthographic()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, mScreenWidth, mScreenHeight, 0, 0, 1);
	
		glMatrixMode(GL_MODELVIEW);
	}
	void GlGfxEngine::setPerspective()
	{
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();

		GLfloat zNear = 0.1f;
		GLfloat zFar = 1000.0f;
		GLfloat aspect = float(mScreenWidth)/float(mScreenHeight);
		GLfloat fH = tan( float(65.0f / 360.0f * 3.14159f) ) * zNear;
		GLfloat fW = fH * aspect;
		glFrustum( -fW, fW, -fH, fH, zNear, zFar );

		glMatrixMode(GL_MODELVIEW);
	}

	void GlGfxEngine::display()
	{
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		setOrthographic();

		vector<IGlRenderable *>::iterator iter;
		for (iter = mRenderables.begin(); iter != mRenderables.end(); ++iter)
		{
			(*iter)->render(0, 0);
		}
	}

	void GlGfxEngine::reshape(int width, int height)
	{
		mScreenWidth = width;
		mScreenHeight = height;
		glViewport (0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	}

	int GlGfxEngine::loadFontDefinitions(const char *filename)
	{
		JsonValue loaded = JsonValue::import_from_file(filename);
		if (loaded.getType() == JV_INT)
		{
			// TODO: ERROR HANDLE
			return loaded.getInt();
		}
		if (loaded.getType() != JV_OBJ)
		{
			// TODO: ERROR HANDLE
			return -1;
		}
		
		JsonObject *obj = loaded.getObj();

		for(JsonObject::iterator iter = obj->begin(); iter != obj->end(); ++iter)
		{
			JsonValue fontDef = iter->second;
			if (fontDef.getType() != JV_OBJ)
			{
				// TODO: ERROR HANDLE
				continue;
			}

			GlFont *font = new GlFont(this, iter->first.c_str());
			font->loadDef(fontDef);

			FontManager::iterator fontCheck = mFontManager.find(iter->first);
			if (fontCheck != mFontManager.end())
			{
				delete font;
				// TODO: ERROR HANDLE
				continue;
			}
			mFontManager[iter->first] = font;
		}

		return 0;
	}

	void GlGfxEngine::addObject(IGlRenderable *renderable)
	{
		bool found = false;
		vector<IGlRenderable *>::iterator iter;
		for (iter = mRenderables.begin(); iter != mRenderables.end(); ++iter)
		{
			if (*iter == renderable)
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			mRenderables.push_back(renderable);
		}
	}
	void GlGfxEngine::removeObject(IGlRenderable *renderable)
	{
		vector<IGlRenderable *>::iterator iter;
		for (iter = mRenderables.begin(); iter != mRenderables.end(); ++iter)
		{
			if (*iter == renderable)
			{
				mRenderables.erase(iter);
				break;
			}
		}
	}

	ITexture *GlGfxEngine::loadTexture(const char *filename)
	{
		return loadGlTexture(filename);
	}
	GlTexture *GlGfxEngine::loadGlTexture(const char *filename)
	{
		string fileStr = filename;
		TextureManager::iterator iter = mTextureManager.find(fileStr);
		if (iter != mTextureManager.end())
		{
			return iter->second;
		}
		//GlTexture *loaded = mTextureManager.getLoaded(filename);
		//if (loaded != NULL)
		//{
		//	return loaded;
		//}

		GlTexture *texture = new GlTexture(this, filename);
		if (texture->isLoaded())
		{
			//mTextureManager.assignLoaded(filename, texture);
			mTextureManager[fileStr] = texture;
			return texture;
		}
		return NULL;
	}
	IFont *GlGfxEngine::getFont(const char *fontName)
	{
		return getGlFont(fontName);
	}
	GlFont *GlGfxEngine::getGlFont(const char *fontName)
	{
		string fontNameStr = fontName;
		FontManager::iterator iter = mFontManager.find(fontNameStr);
		if (iter != mFontManager.end())
		{
			return iter->second;
		}

		return NULL;
	}
}
}
