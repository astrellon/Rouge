#include "game_system.h"

#include "gfx/gfx_engine.h"
#include "engine.h"

#include "gfx/gfx_sprite.h"
#include "gfx/gfx_text_field.h"
#include "gfx/gfx_layer.h"
#include "gfx/gfx_font.h"
#include "gfx/gfx_sprite_scale_nine.h"
#include "gfx/gfx_asset.h"
#include "gfx/scale_nine.h"

#include "mouse_manager.h"

#include "transform.h"

#include "transform.h"
#include "vector.h"

#include <sstream>

#include "ui_button.h"
#include "ui_checkbox.h"
#include "ui_label.h"

#include "handle.h"

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
		mInfo->setPosition(0, 200);
		mGfxEngine->getRootLayer()->addChild(mInfo);
		/*
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
		*/

		mGfxEngine->getRootLayer()->setInteractive(true);

		am::util::Handle<Asset> animatedFont(new Asset(mGfxEngine, "fontBasicAnimated"));
		animatedFont->setTexture(mGfxEngine->getTexture("data/textures/fontBasic.png"));
		animatedFont->setAnimation(16, 16, 256, 4.0f);

		am::util::Handle<Sprite> sprite = new Sprite(mGfxEngine, animatedFont.get());
		mGfxEngine->getRootLayer()->addChild(sprite.get());
		sprite->setInteractive(true);
		
		sprite->setWidth(128.0f);
		sprite->setHeight(128.0f);
		
		sprite->setPosition(200, 0);
		

		/*
		mButton = new am::ui::Button(mGfxEngine, "bigButton");
		mGfxEngine->getUILayer()->addChild(mButton);
		mButton->setParentAnchor(X_CENTER, Y_CENTER);
		mButton->setAnchor(X_CENTER, Y_CENTER);
		mButton->setParentOffsetY(-20.0f);
		
		mCheckbox = new am::ui::Checkbox(mGfxEngine, "checkbox", "Test\nLabel");
		mGfxEngine->getUILayer()->addChild(mCheckbox);
		mCheckbox->setParentAnchor(X_CENTER, Y_CENTER);
		mCheckbox->setAnchorX(X_RIGHT);
		mCheckbox->setParentOffset(0.0f, 40.0f);
		
		am::ui::Checkbox *testCheck = new am::ui::Checkbox(mGfxEngine, "checkbox", "Disable");
		mGfxEngine->getUILayer()->addChild(testCheck);
		testCheck->setParentAnchor(X_CENTER, Y_CENTER);
		testCheck->setAnchorX(X_LEFT);
		testCheck->setParentOffset(0.0f, 40.0f);
		testCheck->addEventListener("changed", this);

		Label *labelLeft = new Label(mGfxEngine, "Text on the left");
		mGfxEngine->getUILayer()->addChild(labelLeft);
		labelLeft->getLabelField()->setAlignment(TextField::ALIGN_RIGHT);
		labelLeft->setParentAnchor(X_CENTER, Y_CENTER);
		labelLeft->setAnchorX(X_RIGHT);
		labelLeft->setParentOffsetY(100.0f);
		*/
		/*
		ScaleNine s9(26, 230, 26, 38);
		am::util::Handle<SpriteScaleNine> scaleTest(new SpriteScaleNine(mGfxEngine, "bigButton"));
		scaleTest->setNumFramesX(2);
		scaleTest->setNumFramesY(2);
		scaleTest->setNumTotalFrames(4);
		scaleTest->setWidth(100);
		mGfxEngine->getUILayer()->addChild(scaleTest.get());
		scaleTest->setPosition(100, 50);
		scaleTest->setScaleNine(s9, true);*/
		/*
		scaleTest = new SpriteScaleNine(mGfxEngine, "bigButton");
		scaleTest->setNumFramesX(2);
		scaleTest->setNumFramesY(2);
		scaleTest->setNumTotalFrames(4);
		scaleTest->setWidth(300);
		mGfxEngine->getUILayer()->addChild(scaleTest.get());
		scaleTest->setPosition(100, 150);
		scaleTest->setScaleNine(s9, true);*/

		am::util::Handle<Label> labelRight(new Label(mGfxEngine, "Text on the right"));
		mGfxEngine->getUILayer()->addChild(labelRight.get());
		labelRight->setParentAnchor(X_CENTER, Y_CENTER);
		labelRight->setAnchorX(X_LEFT);
		labelRight->setParentOffsetY(100.0f);
	}
	void GameSystem::onEvent(am::ui::Event *e)
	{
	}
	void GameSystem::onEvent(am::ui::MouseEvent *e)
	{
	}
	void GameSystem::onEvent(am::ui::DataEvent *e)
	{
		/*stringstream ss;
		ss << "Data event (" << e->getType() << "): ";
		e->getData().display(ss);
		mInfo->setText(ss.str());

		bool selected = e->getData().getBool();
		mButton->setEnabled(selected);
		mCheckbox->setEnabled(selected);

		Checkbox *target = dynamic_cast<Checkbox *>(e->getEventTarget());
		if (target)
		{
			target->setLabel(selected ? "Enable" : "Disable");
		}*/
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

	void GameSystem::setCursorHidden(bool hide)
	{
		mLinkedSystem->setCursorHidden(hide);
	}
	void GameSystem::onCursorHiddenChange(bool hidden)
	{
		// If the OS cursor is hidden, we want to show our in game cursor.
		mGfxEngine->setCursorHidden(!hidden);
	}
	bool GameSystem::isCursorHidden() const
	{
		return mLinkedSystem->isCursorHidden();
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
