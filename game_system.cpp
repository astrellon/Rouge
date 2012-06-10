#include "game_system.h"

#include "gfx/gfx_engine.h"
#include "engine.h"

#include "gfx/gfx_sprite.h"
#include "gfx/gfx_text_field.h"
#include "gfx/gfx_layer.h"
#include "gfx/gfx_font.h"
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
#include "ui_options_panel.h"
#include "ui_main_menu.h"

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
		
		mGfxEngine->getRootLayer()->setInteractive(true);
		
		mMainMenu = new MainMenu(mGfxEngine);
		mMainMenu->addEventListener("quit", this);
		mMainMenu->addEventListener("options", this);
		mGfxEngine->getUILayer()->addChild(mMainMenu.get());
		mMainMenu->setWidth(mGfxEngine->getScreenWidth());
		mMainMenu->setHeight(mGfxEngine->getScreenHeight());

		mOptionsPanel = new OptionsPanel(mGfxEngine);
		mOptionsPanel->addEventListener("close_options", this);
		mGfxEngine->getUILayer()->addChild(mOptionsPanel.get());
		mOptionsPanel->setWidth(mGfxEngine->getScreenWidth());
		mOptionsPanel->setHeight(mGfxEngine->getScreenHeight());
		mOptionsPanel->setVisible(false);

		am::util::Handle<Label> labelRight(new Label(mGfxEngine, "Text on the right"));
		mGfxEngine->getUILayer()->addChild(labelRight.get());
		labelRight->setParentAnchor(X_CENTER, Y_CENTER);
		labelRight->setAnchorX(X_LEFT);
		labelRight->setParentOffsetY(100.0f);
	}
	void GameSystem::onEvent(am::ui::Event *e)
	{
		if (e->getType().compare("quit") == 0)
		{
			setProgramRunning(false);
			return;
		}
		if (e->getType().compare("options") == 0)
		{
			mMainMenu->setVisible(false);
			mOptionsPanel->setVisible(true);
			return;
		}
		if (e->getType().compare("close_options") == 0)
		{
			mMainMenu->setVisible(true);
			mOptionsPanel->setVisible(false);
			return;
		}
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
		if (mMainMenu.get())
		{
			mMainMenu->setWidth(width);
			mMainMenu->setHeight(height);
		}
		if (mOptionsPanel.get())
		{
			mOptionsPanel->setWidth(width);
			mOptionsPanel->setHeight(height);
		}
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
