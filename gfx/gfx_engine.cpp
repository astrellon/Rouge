#include "gfx_engine.h"

#include <math.h>

#include "../gl.h"
#include "IL/il.h"

#include "gfx_renderable.h"
#include "gfx_asset.h"
#include "gfx_texture.h"
#include "gfx_font.h"
#include "gfx_sprite.h"
#include "gfx_text_field.h"
#include "gfx_text_list.h"
#include "gfx_layer.h"

#include "../event.h"
#include "../mouse_event.h"
#include "../keyboard_event.h"

#include "../logger.h"

#include <sstream>

using namespace std;
using namespace am::ui;

namespace am {
namespace gfx {

	GfxEngine *GfxEngine::sMainGfxEngine = NULL;

	GfxEngine::GfxEngine() :
		mCursor(NULL),
		mHideCursor(false),
		mRootLayer(NULL),
		mUILayer(NULL),
		mGameLayer(NULL),
		mCameraX(0),
		mCameraY(0)
	{
		
	}
	GfxEngine::~GfxEngine()
	{

	}

	GfxEngine *GfxEngine::getEngine()
	{
		if (sMainGfxEngine == NULL)
		{
			sMainGfxEngine = new GfxEngine();
		}
		return sMainGfxEngine;
	}

	void GfxEngine::init()
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

		mRootLayer = new Layer();
		mRootLayer->setName("RootLayer");
		mRootLayer->setInteractive(true);

		mGameLayer = new Layer();
		mGameLayer->setName("GameLayer");
		mGameLayer->setInteractive(true);
		mRootLayer->addChild(mGameLayer.get());


		mUILayer = new Layer();
		mUILayer->setName("UILayer");
		mUILayer->setInteractive(true);
		mUILayer->setWidth(static_cast<float>(mScreenWidth));
		mUILayer->setHeight(static_cast<float>(mScreenHeight));
		mRootLayer->addChild(mUILayer.get());

		mDebugLayer = new Layer();
		mDebugLayer->setName("DebugLayer");
		mDebugLayer->setInteractive(true);
		mDebugLayer->setWidth(static_cast<float>(mScreenWidth));
		mDebugLayer->setHeight(static_cast<float>(mScreenHeight));
		
		Asset *mCursorAsset = getAsset("cursor");
		if (mCursorAsset == NULL)
		{
			throw std::runtime_error("Unable to load cursor asset");
		}

		Asset *mBasicFont = getAsset("fontBasic");
		if (mBasicFont == NULL)
		{
			throw std::runtime_error("Unable to load basic font asset");
		}
		mCursor = new Sprite(mCursorAsset);
	}
	void GfxEngine::deinit()
	{
		ilShutDown();
	}
	void GfxEngine::setOrthographic()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, mScreenWidth, mScreenHeight, 0, 0, 1);
	
		glMatrixMode(GL_MODELVIEW);
	}
	void GfxEngine::setPerspective()
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

	void GfxEngine::display(float dt)
	{
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		setOrthographic();

		mGameLayer->setPosition(-mCameraX + static_cast<float>(mScreenWidth / 2), -mCameraY + static_cast<float>(mScreenHeight / 2));

		mRootLayer->render(dt);

		if (mDebugLayer->isVisible())
		{
			mDebugLayer->render(dt);
		}
		
		if (mCursor.get() && !mHideCursor && mCursor->isVisible())
		{
			mCursor->render(dt);
		}
	}

	void GfxEngine::reshape(int width, int height)
	{
		mScreenWidth = width;
		mScreenHeight = height;

		if (mUILayer.get())
		{
			mUILayer->setWidth(static_cast<float>(width));
			mUILayer->setHeight(static_cast<float>(height));
		}
		if (mDebugLayer.get())
		{
			mDebugLayer->setWidth(static_cast<float>(width));
			mDebugLayer->setHeight(static_cast<float>(height));
		}
		glViewport (0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	}

	Sprite *GfxEngine::getCursor()
	{
		return mCursor.get();
	}
	void GfxEngine::setCursor(Sprite *sprite)
	{
		mRootLayer->removeChild(sprite);
		mCursor = sprite;
		mRootLayer->addChild(mCursor.get());
	}

	Asset *GfxEngine::getAsset(const char *assetName)
	{
		string assetNameStr = assetName;
		AssetManager::iterator iter = mAssetManager.find(assetNameStr);
		if (iter != mAssetManager.end())
		{
			return iter->second.get();
		}

		stringstream ss;
		if (assetNameStr[0] == '/')
		{
			ss << "data" << assetNameStr << ".ssff";
		}
		else
		{
			ss << "data/assets/" << assetNameStr << ".ssff";
		}

		JsonValue loaded = JsonValue::import_from_file(ss.str().c_str());
		if (loaded.getType() != JV_OBJ)
		{
			stringstream errss;
			errss << "Unable to load asset '" << assetNameStr << "', using the path '";
			errss << ss.str() << "\'\nLoaded: "; 
			loaded.display(errss);

			am_log("ASST", errss);
			
			return NULL;
		}

		Asset *asset = new Asset(assetName);
		int loadAsset = asset->loadDef(loaded);
		if (loadAsset != 0)
		{
			stringstream errss;
			errss << "Error loading asset definition '" << assetNameStr << "': " << loadAsset;
			am_log("ASST", errss);
			delete asset;
			return NULL;
		}

		mAssetManager[assetNameStr] = asset;

		return asset;
	}

	Texture *GfxEngine::getTexture(const char *filename)
	{
		string fileStr = filename;
		TextureManager::iterator iter = mTextureManager.find(fileStr);
		if (iter != mTextureManager.end())
		{
			return iter->second.get();
		}

		Texture *texture = new Texture(filename);
		if (texture->isLoaded())
		{
			mTextureManager[fileStr] = texture;
			return texture;
		}

		delete texture;
		stringstream errss;
		errss << "Unable to load texture '" << filename << "'";
		am_log("GFX", errss);
		
		return NULL;
	}
	Font *GfxEngine::getFont(const char *fontName)
	{
		string fontNameStr = fontName;
		FontManager::iterator iter = mFontManager.find(fontNameStr);
		if (iter != mFontManager.end())
		{
			return iter->second.get();
		}

		stringstream ss;
		ss << "data/fonts/" << fontNameStr << ".ssff";

		JsonValue loaded = JsonValue::import_from_file(ss.str().c_str());
		if (loaded.getType() != JV_OBJ)
		{
			stringstream errss;
			errss << "Unable to load font '" << fontNameStr << "', using the path '";
			errss << ss.str() << '\''; 
			am_log("FONT", errss);
			return NULL;
		}

		Font *font = new Font(fontName);
		int loadFont = font->loadDef(loaded);
		if (loadFont != 0)
		{
			stringstream errss;
			errss << "Error loading font definition '" << fontNameStr << "': " << loadFont;
			am_log("FONT", errss);
			delete font;
			return NULL;
		}

		mFontManager[fontNameStr] = font;

		return font;
	}

	Layer *GfxEngine::getRootLayer()
	{
		return mRootLayer.get();
	}
	Layer *GfxEngine::getGameLayer()
	{
		return mGameLayer.get();
	}
	Layer *GfxEngine::getUILayer()
	{
		return mUILayer.get();
	}
	Layer *GfxEngine::getDebugLayer()
	{
		return mDebugLayer.get();
	}

	void GfxEngine::setCursorHidden(bool hide)
	{
		mHideCursor = hide;
	}
	bool GfxEngine::isCursorHidden() const
	{
		return mHideCursor;
	}

	void GfxEngine::setCameraLocation(float x, float y)
	{
		mCameraX = x;
		mCameraY = y;
	}
	float GfxEngine::getCameraX() const
	{
		return mCameraX;
	}
	float GfxEngine::getCameraY() const
	{
		return mCameraY;
	}

	void GfxEngine::applyColourStack()
	{
		if (mColourStack.size() > 0)
		{
			mColourStack.back().applyColour();
		}
	}
	void GfxEngine::pushColourStack(const Colour &colour)
	{
		Colour top(1.0f, 1.0f, 1.0f, 1.0f);
		if (mColourStack.size() > 0)
		{
			top = mColourStack.back();
		}
		top.multiply(colour);
		mColourStack.push_back(top);
	}
	void GfxEngine::popColourStack()
	{
		mColourStack.pop_back();
	}

	int GfxEngine::getScreenWidth() const
	{
		return mScreenWidth;
	}
	int GfxEngine::getScreenHeight() const
	{
		return mScreenHeight;
	}
	
	void GfxEngine::onKeyDown(const bool *keys, int key)
	{

	}
	void GfxEngine::onKeyUp(const bool *keys, int key)
	{

	}

}
}
