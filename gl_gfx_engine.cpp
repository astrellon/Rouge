#include "gl_gfx_engine.h"

#include <math.h>

#include "gl.h"
#include "IL/il.h"

#include "igl_renderable.h"
#include "gl_asset.h"
#include "gl_texture.h"
#include "gl_font.h"
#include "gl_text_field.h"
#include "gl_sprite.h"
#include "gl_layer.h"
#include "gl_text_list.h"

#include "logger.h"

#include <sstream>

using namespace std;

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

		glClearColor (0.0f, 0.3f, 0.4f, 0.0f);
		// Enable texture for the text fields.
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
	
		/*glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);*/

		//GlTextArea *area = new GlTextArea(this);
		//addObject(area);
		GlTextList *list = new GlTextList(this);
		addObject(list);

		list->setWidth(600.0f);
		list->setBaseGlFont(getGlFont("basic"));
		list->addEntry("Hello there, how are you today?", "Info");
		list->addEntry("I am good thank you, how are you?", "Info");

		//GlTextField *text = new GlTextField(this);
		//addObject(text);
		//area->addGlTextField("info", text);

		//text->setBaseGlFont(getGlFont("basic"));
		//text->setText(string("Hello there, how are you today? I am good thank you"));

		//text = new GlTextField(this);
		//addObject(text);
		//area->addGlTextField("info", text);

		//text->setWidth(30.0f);
		//text->getTransform().translate(0.0f, -40.0f, 0.0f, true);

		//text->setBaseGlFont(getGlFont("basic"));
		//text->setText(string("Hello there Melli\nTest:\tData\nTist: \tData 2"));
		//text->setText(string("Hello there, how are you today? I am good thank you"));

		GlSprite *sprite = new GlSprite(this, getGlAsset("fontBasic"));
		
		sprite->setNumFramesX(16);
		sprite->setNumFramesY(16);
		sprite->setNumTotalFrames(256);

		sprite->setWidth(128.0f);
		sprite->setHeight(128.0f);
		sprite->setFrameRate(4.0f);

		GlLayer *layer = new GlLayer(this);
		addObject(layer);

		layer->getTransform().translate(0.0f, 0.0f, 0.0f, true);
		layer->addGlChild(sprite);

		layer->getTransform().rotate(0.5f, 0.0f);
		layer->getTransform().translate(200.0f, 0.0f, 0.0f, true);

		sprite->getTransform().rotate(-0.5f, 0.0f);
		sprite->getTransform().translate(50.0f, 0.0f, 0.0f, true);
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

	void GlGfxEngine::display(float dt)
	{
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		setOrthographic();
		//setPerspective();

		vector<IGlRenderable *>::iterator iter;
		for (iter = mRenderables.begin(); iter != mRenderables.end(); ++iter)
		{
			(*iter)->render(dt);
		}
	}

	void GlGfxEngine::reshape(int width, int height)
	{
		mScreenWidth = width;
		mScreenHeight = height;
		glViewport (0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
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

	IAsset *GlGfxEngine::getAsset(const char *assetName)
	{
		return getGlAsset(assetName);
	}
	GlAsset *GlGfxEngine::getGlAsset(const char *assetName)
	{
		string assetNameStr = assetName;
		AssetManager::iterator iter = mAssetManager.find(assetNameStr);
		if (iter != mAssetManager.end())
		{
			return iter->second;
		}

		stringstream ss;
		ss << "data/assets/" << assetNameStr << ".ssff";

		JsonValue loaded = JsonValue::import_from_file(ss.str().c_str());
		if (loaded.getType() != JV_OBJ)
		{
			stringstream errss;
			errss << "Unable to load asset '" << assetNameStr << "', using the path '";
			errss << ss.str() << '\''; 
			am_log("ASST", errss.str().c_str());
			return NULL;
		}

		GlAsset *asset = new GlAsset(this, assetName);
		int loadAsset = asset->loadDef(loaded);
		if (loadAsset != 0)
		{
			stringstream errss;
			errss << "Error loading asset definition '" << assetNameStr << "': " << loadAsset;
			am_log("ASST", errss.str().c_str());
			delete asset;
			return NULL;
		}

		mAssetManager[assetNameStr] = asset;

		return asset;
	}

	ITexture *GlGfxEngine::getTexture(const char *filename)
	{
		return getGlTexture(filename);
	}
	GlTexture *GlGfxEngine::getGlTexture(const char *filename)
	{
		string fileStr = filename;
		TextureManager::iterator iter = mTextureManager.find(fileStr);
		if (iter != mTextureManager.end())
		{
			return iter->second;
		}

		GlTexture *texture = new GlTexture(this, filename);
		if (texture->isLoaded())
		{
			mTextureManager[fileStr] = texture;
			return texture;
		}
		stringstream errss;
		errss << "Unable to load texture '" << filename << "'";
		am_log("GFX", errss.str().c_str());
		
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

		stringstream ss;
		ss << "data/fonts/" << fontNameStr << ".ssff";

		JsonValue loaded = JsonValue::import_from_file(ss.str().c_str());
		if (loaded.getType() != JV_OBJ)
		{
			stringstream errss;
			errss << "Unable to load font '" << fontNameStr << "', using the path '";
			errss << ss.str() << '\''; 
			am_log("FONT", errss.str().c_str());
			return NULL;
		}

		GlFont *font = new GlFont(this, fontName);
		int loadFont = font->loadDef(loaded);
		if (loadFont != 0)
		{
			stringstream errss;
			errss << "Error loading font definition '" << fontNameStr << "': " << loadFont;
			am_log("FONT", errss.str().c_str());
			delete font;
			return NULL;
		}

		mFontManager[fontNameStr] = font;

		return font;
	}
}
}