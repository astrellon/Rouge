#include "ui_main_menu.h"

#include <sys/rouge_system.h>
#include <log/logger.h>

#include "ui_button.h"

namespace am {
namespace ui {

	MainMenu::MainMenu(RougeSystem *system) : 
		UIComponent(),
		mRougeSystem(system),
		mStartGame(new Button("bigButton", "Start Game")),
		mLoadGame(new Button("bigButton", "Load Game")),
		mOptions(new Button("bigButton", "Options")),
		mQuit(new Button("bigButton", "Quit :("))
	{
		setInteractive(true);

		initButton(mStartGame.get());
		initButton(mLoadGame.get());
		initButton(mOptions.get());
		initButton(mQuit.get());

		float buttonHeight = mStartGame->getHeight();
		mStartGame->setWidth(220.0f);
		mStartGame->setParentOffsetY(-buttonHeight * 1.5f);
		mStartGame->addEventListener("click", this);

		mLoadGame->setParentOffsetY(-buttonHeight * 0.5f);
		mLoadGame->setWidth(200.0f);
		mLoadGame->addEventListener("click", this);

		mOptions->setParentOffsetY(buttonHeight * 0.5f);
		mOptions->setWidth(180.0f);
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
			mRougeSystem->quitGame();
			return;
		}
		if (e->getEventTarget() == mOptions.get())
		{
			am_log("BTN", "OPTIONS BUTTON");
			mRougeSystem->showOptionsPanel();
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
			mRougeSystem->newGame();
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
