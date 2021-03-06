#include "gfx_engine.h"

#include <math.h>

#include <ui/event.h>
#include <ui/mouse_event.h>
#include <ui/mouse_manager.h>
#include <ui/keyboard_event.h>
using namespace am::ui;

#include <log/logger.h>

#include <sstream>

#include <lua/lua_state.h>
using namespace am::lua;

#include <gl.h>
#include "IL/il.h"

#include "gfx_renderable.h"
#include "gfx_asset.h"
#include "gfx_texture.h"
#include "gfx_font.h"
#include "gfx_sprite.h"
#include "gfx_text_field.h"
#include "gfx_text_list.h"
#include "gfx_layer.h"
#include "gfx_camera.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace gfx {

	const int GfxEngine::LUA_ID = LUA_ID_GFXENGINE;
	const char *GfxEngine::LUA_TABLENAME = LUA_TABLE_GFXENGINE;

	GfxEngine *GfxEngine::sMainGfxEngine = nullptr;

	GfxEngine::GfxEngine() :
		mHideCursor(false),
		mScreenWidth(2),
		mScreenHeight(2),
		mForceReloadMode(false)
	{
		
	}
	GfxEngine::~GfxEngine()
	{

	}

	GfxEngine *GfxEngine::getEngine()
	{
		if (sMainGfxEngine == nullptr)
		{
			sMainGfxEngine = new GfxEngine();
		}
		return sMainGfxEngine;
	}
	void GfxEngine::deinitGfxEngine()
	{
		if (sMainGfxEngine != nullptr)
		{
			sMainGfxEngine->deinit();
			GfxEngine *engine = sMainGfxEngine;
			sMainGfxEngine = nullptr;
			delete engine;
		}
	}

	void GfxEngine::init()
	{
		// Initialize the DevIL framework.
		ilInit();

		glClearColor (0.2f, 0.3f, 0.4f, 0.0f);
		// Enable texture for the text fields.
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);

		/*glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glFrontFace(GL_CW);*/

        if (!mCamera)
        {
            mCamera = new Camera();
        }

		mRootLayer = new Layer();
		mRootLayer->setName("RootLayer");
		mRootLayer->setInteractive(true);

		mGameLayer = new Layer();
		mGameLayer->setName("GameLayer");
		mGameLayer->setInteractive(true);
		mRootLayer->addChild(mGameLayer);

		mUILayer = new Layer();
		mUILayer->setName("UILayer");
		mUILayer->setInteractive(true);
		mUILayer->setWidth(static_cast<float>(mScreenWidth));
		mUILayer->setHeight(static_cast<float>(mScreenHeight));
		mRootLayer->addChild(mUILayer);

		mTooltipLayer = new Layer();
		mTooltipLayer->setName("TooltipLayer");
		mTooltipLayer->setInteractive(false);
		mTooltipLayer->setWidth(static_cast<float>(mScreenWidth));
		mTooltipLayer->setHeight(static_cast<float>(mScreenHeight));
		mRootLayer->addChild(mTooltipLayer);

		mDebugLayer = new Layer();
		mDebugLayer->setName("DebugLayer");
		mDebugLayer->setInteractive(true);
		mDebugLayer->setWidth(static_cast<float>(mScreenWidth));
		mDebugLayer->setHeight(static_cast<float>(mScreenHeight));
        mRootLayer->addChild(mDebugLayer);

		//Asset *cursorAsset = getAssetLua("cursor");
		Asset *cursorAsset = getAsset("ui:cursor");
		if (cursorAsset == nullptr)
		{
			throw std::runtime_error("Unable to load cursor asset");
		}

		Asset *basicFont = getAsset("fonts:basic");
		if (basicFont == nullptr)
		{
			// Incase there is an issue loading the basic font.
			throw std::runtime_error("Unable to load basic font asset");
		}

		mCursor = new Sprite(cursorAsset);
		mDefaultCursor = mCursor;
	}
	void GfxEngine::deinit()
	{
		mDebugLayer->deinit();
		mTooltipLayer->deinit();
		mUILayer->deinit();
		mGameLayer->deinit();
		mRootLayer->deinit();

		{
			FontMap fonts = mFontManager;
			FontMap::iterator iter;
			for (iter = fonts.begin(); iter != fonts.end(); ++iter)
			{
				iter->second = nullptr;
			}
			mFontManager.clear();
		}
		{
			AssetMap assets = mAssetManager;
			AssetMap::iterator iter;
			for (iter = assets.begin(); iter != assets.end(); ++iter)
			{
				iter->second = nullptr;
			}
			mAssetManager.clear();
		}
		{
			TextureMap textures = mTextureManager;
			TextureMap::iterator iter;
			for (iter = textures.begin(); iter != textures.end(); ++iter)
			{
				iter->second = nullptr;
			}
			mTextureManager.clear();
		}

		ilShutDown();
	}
	void GfxEngine::setOrthographic()
	{
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(0, mScreenWidth, mScreenHeight, 0, 0, 255);
		glMatrixMode(GL_MODELVIEW);
	}
	void GfxEngine::setPerspective()
	{
		glMatrixMode (GL_PROJECTION);
		glLoadIdentity();

		GLfloat zNear = 1.0f;
		GLfloat zFar = 10000.0f;
		GLfloat aspect = float(mScreenWidth)/float(mScreenHeight);
		GLfloat fH = tan( float(65.0f / 360.0f * 3.14159f) ) * zNear;
		GLfloat fW = fH * aspect;
		glFrustum( -fW, fW, -fH, fH, zNear, zFar );

        //glEnable(GL_MULTISAMPLE);

		glMatrixMode(GL_MODELVIEW);
	}

	void GfxEngine::display(float dt)
	{
		glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

        mCamera->apply(mScreenWidth, mScreenHeight);
        mGameLayer->render(dt);

        glLoadIdentity();
        setOrthographic();
        mUILayer->render(dt);
        mTooltipLayer->render(dt);
        mDebugLayer->render(dt);
		
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
		glViewport (0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));
	}

	Renderable *GfxEngine::getCursor()
	{
		return mCursor.get();
	}
	void GfxEngine::setCursor(Renderable *sprite)
	{
		mCursor = sprite;
		if (sprite)
		{
			sprite->setPosition(MouseManager::getManager()->getMouseX(), MouseManager::getManager()->getMouseY());
		}
	}

	Renderable *GfxEngine::getDefaultCursor()
	{
		return mDefaultCursor;
	}
	
    AssetMap &GfxEngine::getAssetMap()
	{
		return mAssetManager;
	}
	void GfxEngine::addAsset(Asset *asset)
	{
		addDefinition<Asset>(asset, mAssetManager, asset->getName());
	}
	Asset *GfxEngine::findAsset(const char *name)
	{
		return findDefinition<Asset>(mAssetManager, name);
	}
	Asset *GfxEngine::getAsset(const char *name, bool reload)
	{
		if (reload)
		{
			setForceReloadMode(true);
		}
		Asset *result = getDefinition<Asset>(mAssetManager, name, reload, 0);
		if (reload)
		{
			setForceReloadMode(false);
		}
		return result;
	}

	base::ReturnCode GfxEngine::getTexture(const char *filename, Texture *&texture)
	{
		std::string fileStr = filename;
		if (!mForceReloadMode)
		{
			TextureMap::iterator iter = mTextureManager.find(fileStr);
			if (iter != mTextureManager.end())
			{
				texture = iter->second.get();
				return base::SUCCESS;
			}
		}

		Texture *temp = new Texture();
		base::ReturnCode result = temp->setFilename(filename);
		if (result == base::SUCCESS)
		{
			mTextureManager[fileStr] = temp;
			texture = temp;
			return base::SUCCESS;
		}

		delete temp;
		std::stringstream errss;
		errss << "Unable to load texture '" << filename << "'";
		am_log("GFX", errss);
		
		return result;
	}

	TextureMap &GfxEngine::getTextureMap()
	{
		return mTextureManager;
	}
	
	Font *GfxEngine::getFont(const char *fontName)
	{
		return getDefinition<Font>(mFontManager, fontName, false, 1);
	}
	void GfxEngine::addFont(Font *font)
	{
		addDefinition<Font>(font, mFontManager, font->getName().c_str());
	}
	int GfxEngine::reloadFont(const char *fontName)
	{
		return 0;
	}
	FontMap &GfxEngine::getFontMap()
	{
		return mFontManager;
	}

	Layer *GfxEngine::getRootLayer()
	{
		return mRootLayer;
	}
	Layer *GfxEngine::getGameLayer()
	{
		return mGameLayer;
	}
	Layer *GfxEngine::getUILayer()
	{
		return mUILayer;
	}
	Layer *GfxEngine::getTooltipLayer()
	{
		return mTooltipLayer;
	}
	Layer *GfxEngine::getDebugLayer()
	{
		return mDebugLayer;
	}

	void GfxEngine::setCursorHidden(bool hide)
	{
		mHideCursor = hide;
	}
	bool GfxEngine::isCursorHidden() const
	{
		return mHideCursor;
	}

    void GfxEngine::setCamera(Camera *camera)
    {
        if (!camera)
        {
            // Cannot have a null camera.
            return;
        }
        mCamera = camera;
    }
    Camera *GfxEngine::getCamera() const
    {
        return mCamera;
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

	void GfxEngine::setForceReloadMode(bool reload)
	{
		mForceReloadMode = reload;
	}
	bool GfxEngine::isForeReloadMode() const
	{
		return mForceReloadMode;
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

	const char *GfxEngine::getBaseDefinitionPath(int id) const
	{
		if (id == 1)
		{
			return "data/fonts/";
		}
		return "data/assets/";
	}

}
}
