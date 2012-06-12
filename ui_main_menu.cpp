#include "ui_main_menu.h"

#include "ui_button.h"

#include "logger.h"

namespace am {
namespace ui {

	MainMenu::MainMenu(GfxEngine *engine) : 
		UIComponent(engine),
		mStartGame(new Button(engine, "bigButton", "Start Game")),
		mLoadGame(new Button(engine, "bigButton", "Load Game")),
		mOptions(new Button(engine, "bigButton", "Options")),
		mQuit(new Button(engine, "bigButton", "Quit :("))
	{
		setInteractive(true);

		initButton(mStartGame.get());
		initButton(mLoadGame.get());
		initButton(mOptions.get());
		initButton(mQuit.get());

		float buttonHeight = mStartGame->getHeight();
		mStartGame->setWidth(120.0f);
		mStartGame->setParentOffsetY(-buttonHeight * 1.5f);
		mStartGame->addEventListener("click", this);

		mLoadGame->setParentOffsetY(-buttonHeight * 0.5f);
		mLoadGame->addEventListener("click", this);

		mOptions->setParentOffsetY(buttonHeight * 0.5f);
		mOptions->addEventListener("click", this);

		mQuit->setParentOffsetY(buttonHeight * 1.5f);
		mQuit->setWidth(120.0f);
		mQuit->addEventListener("click", this);

	}
	
	MainMenu::~MainMenu()
	{
	}

	void MainMenu::onEvent(Event *e)
	{
		if (e->getEventTarget() == mQuit.get())
		{
			am_log("BTN", "QUIT BUTTON");
			Event *e = new Event("quit", this);
			fireEvent(e);
			delete e;
			return;
		}
		if (e->getEventTarget() == mOptions.get())
		{
			am_log("BTN", "OPTIONS BUTTON");
			Event *e = new Event("options", this);
			fireEvent(e);
			delete e;
			return;
		}
		if (e->getEventTarget() == mLoadGame.get())
		{
			am_log("BTN", "LOAD GAME");
			return;
		}
		if (e->getEventTarget() == mStartGame.get())
		{
			am_log("BTN", "START GAME");
			Event *e = new Event("new_game", this);
			fireEvent(e);
			delete e;
			return;
		}
	}

	void MainMenu::initButton(Button *btn)
	{
		addChild(btn);
		btn->setParentAnchor(X_CENTER, Y_CENTER);
		btn->setAnchor(X_CENTER, Y_CENTER);
	}


}
}
