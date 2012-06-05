#include "game_system.h"

#include "gfx/gfx_engine.h"
#include "engine.h"

#include "gfx/gfx_sprite.h"
#include "gfx/gfx_text_field.h"
#include "gfx/gfx_layer.h"
#include "gfx/gfx_font.h"

#include "mouse_manager.h"

#include "transform.h"

#include "transform.h"
#include "vector.h"

#include <sstream>

#include "ui_button.h"
#include "ui_checkbox.h"

using namespace std;
using namespace am::math;

namespace am {
namespace sys {

	GameSystem::GameSystem(ISystem *linked, Engine *engine, GfxEngine *gfxEngine,
		MouseManager *mouseManager) :
		mLinkedSystem(linked),
		mEngine(engine),
		mGfxEngine(gfxEngine),
		mMouseManager(mouseManager)
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
		mInfo->getTransform().setPosition(Vector4f(0, 200, 0));
		mGfxEngine->getRootLayer()->addChild(mInfo);

		const char *txt = "Hello there how are you today my friend?\nI am good thank you.";
		//const char *txt = "Hello \nthere";
		
		mInfo->setText(txt);
		mInfo->setWidth(140.0f);
		mInfo->setAlignment(TextField::ALIGN_RIGHT);

		mInfo2 = new TextField(mGfxEngine);
		mInfo2->getTransform().setPosition(Vector4f(0, 120, 0));
		mGfxEngine->getRootLayer()->addChild(mInfo2);

		mInfo2->setText(txt);
		mInfo2->setWidth(140.0f);

		mInfo3 = new TextField(mGfxEngine);
		mInfo3->getTransform().setPosition(Vector4f(0, 280, 0));
		mGfxEngine->getRootLayer()->addChild(mInfo3);

		mInfo3->setText(txt);
		mInfo3->setWidth(140.0f);
		mInfo3->setAlignment(TextField::ALIGN_CENTER);

		Sprite *sprite = new Sprite(mGfxEngine, "fontBasic");
		mGfxEngine->getRootLayer()->addChild(sprite);
		mGfxEngine->getRootLayer()->setInteractive(true);
		sprite->setInteractive(true);
		
		sprite->setNumFramesX(16);
		sprite->setNumFramesY(16);
		sprite->setNumTotalFrames(256);

		sprite->setWidth(128.0f);
		sprite->setHeight(128.0f);
		sprite->setFrameRate(4.0f);

		sprite->getTransform().translate(200.0f, 0.0f, 0, true);

		am::ui::Button *testBtn = new am::ui::Button(mGfxEngine, "bigButton");
		mGfxEngine->getUILayer()->addChild(testBtn);
		testBtn->setAnchorX(X_CENTER);
		testBtn->setAnchorY(Y_CENTER);
		testBtn->setOffsetY(-20.0f);
		
		am::ui::Checkbox *testCheck = new am::ui::Checkbox(mGfxEngine, "checkbox", "Test Label");
		mGfxEngine->getUILayer()->addChild(testCheck);
		testCheck->setAnchorX(X_CENTER);
		testCheck->setAnchorY(Y_CENTER);
		testCheck->setOffsetY(40.0f);
		
		sprite = new Sprite(mGfxEngine, "checkbox");
		mGfxEngine->getRootLayer()->addChild(sprite);
		sprite->setNumFramesX(2);
		sprite->setNumFramesY(4);
		sprite->setNumTotalFrames(8);
		sprite->setFrameRate(4.0f);
		
	}
	void GameSystem::onEvent(am::ui::Event *e)
	{
	}
	void GameSystem::onEvent(am::ui::MouseEvent *e)
	{
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

	void GameSystem::onMouseDown(am::ui::MouseButton mouseButton, int x, int y)
	{
		mGfxEngine->getCursor()->setPosition(x, y);
		mMouseManager->onMouseDown(mouseButton, x, y);
	}
	void GameSystem::onMouseMove(am::ui::MouseButton mouseButton, int x, int y)
	{
		mGfxEngine->getCursor()->setPosition(x, y);
		mMouseManager->onMouseMove(mouseButton, x, y);
	}
	void GameSystem::onMouseUp(am::ui::MouseButton mouseButton, int x, int y)
	{
		mGfxEngine->getCursor()->setPosition(x, y);
		mMouseManager->onMouseUp(mouseButton, x, y);
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
