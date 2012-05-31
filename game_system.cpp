#include "game_system.h"

#include "gfx/gfx_engine.h"
#include "engine.h"

#include "gfx/gfx_sprite.h"
#include "gfx/gfx_text_field.h"
#include "gfx/gfx_layer.h"

#include "transform.h"

#include "transform.h"
#include "vector.h"

#include <sstream>

using namespace std;
using namespace am::math;

namespace am {
namespace sys {

	GameSystem::GameSystem(ISystem *linked, Engine *engine, GfxEngine *gfxEngine) :
		mLinkedSystem(linked),
		mEngine(engine),
		mGfxEngine(gfxEngine)
	{

	}
	GameSystem::~GameSystem()
	{
		deinit();
	}

	void GameSystem::setSize(int width, int height)
	{
		mLinkedSystem->setSize(width, height);
	}
	void GameSystem::setPosition(int x, int y)
	{
		mLinkedSystem->setPosition(x, y);
	}

	int GameSystem::getWidth() const
	{
		return mLinkedSystem->getWidth();
	}
	int GameSystem::getHeight() const
	{
		return mLinkedSystem->getHeight();
	}

	int GameSystem::getX() const
	{
		return mLinkedSystem->getX();
	}
	int GameSystem::getY() const
	{
		return mLinkedSystem->getY();
	}

	void GameSystem::setTitle(const char *title)
	{
		mLinkedSystem->setTitle(title);
	}
	const char *GameSystem::getTitle() const
	{
		return mLinkedSystem->getTitle();
	}

	void GameSystem::init()
	{
		mEngine->init();
		mGfxEngine->init();

		mInfo = new TextField(mGfxEngine);
		mInfo->getTransform().setPosition(Vector4f(0, 100, 0));
		mGfxEngine->getRootLayer()->addChild(mInfo);

		Sprite *sprite = new Sprite(mGfxEngine, "fontBasic");
		mGfxEngine->getRootLayer()->addChild(sprite);
		
		sprite->setNumFramesX(16);
		sprite->setNumFramesY(16);
		sprite->setNumTotalFrames(256);

		sprite->setWidth(128.0f);
		sprite->setHeight(128.0f);
		sprite->setFrameRate(4.0f);

		sprite->getTransform().translate(200.0f, 0.0f, 0, true);

		sprite->addEventListener("mouse_move", this);
	}
	void GameSystem::onEvent(am::ui::MouseEvent &e)
	{
		stringstream ss;
		ss << "MouseMove: " << e.getMouseX() << ", " << e.getMouseY();
		ss << " | " << e.getLocalMouseX() << ", " << e.getLocalMouseY();
		mInfo->setText(ss.str());
	}
	void GameSystem::reshape(int width, int height)
	{
		mGfxEngine->reshape(width, height);
	}
	void GameSystem::update(float dt)
	{
		mEngine->update(dt);
	}
	void GameSystem::display(float dt)
	{
		mGfxEngine->display(dt);
	}
	void GameSystem::deinit()
	{
		mGfxEngine->deinit();
		mEngine->deinit();
	}

	void GameSystem::onMouseDown(int mouseButton, int x, int y)
	{
		mGfxEngine->onMouseDown(mouseButton, x , y);
	}
	void GameSystem::onMouseMove(int mouseButton, int x, int y)
	{
		mGfxEngine->onMouseMove(mouseButton, x , y);
		//mEngine->mouseFunc(mouseButton, x, y);
	}
	void GameSystem::onMouseUp(int mouseButton, int x, int y)
	{
		mGfxEngine->onMouseUp(mouseButton, x , y);
	}
	void GameSystem::onKeyDown(const bool *keys, int key)
	{
		mGfxEngine->onKeyDown(keys, key);
	}
	void GameSystem::onKeyUp(const bool *keys, int key)
	{
		mGfxEngine->onKeyUp(keys, key);
	}
	
	bool GameSystem::isProgramRunning() const
	{
		return mLinkedSystem->isProgramRunning();
	}
	void GameSystem::setProgramRunning(bool running)
	{
		mLinkedSystem->setProgramRunning(running);
	}

	bool GameSystem::isRunning() const
	{
		return mLinkedSystem->isRunning();
	}
	int GameSystem::startLoop()
	{
		return mLinkedSystem->startLoop();
	}
	void GameSystem::stopLoop()
	{
		mLinkedSystem->stopLoop();
	}

	ISystem *GameSystem::getLinkedSystem()
	{
		return mLinkedSystem;
	}
	GfxEngine *GameSystem::getGfxEngine()
	{
		return mGfxEngine;
	}
	Engine *GameSystem::getEngine()
	{
		return mEngine;
	}
}
}
