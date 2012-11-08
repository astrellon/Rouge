#include "ui_main_menu.h"

#include <sys/rouge_system.h>
#include <log/logger.h>

#include "ui_text_button.h"

namespace am {
namespace ui {

	MainMenu::MainMenu(RougeSystem *system) : 
		UIComponent(),
		mRougeSystem(system),
		mStartGame(new TextButton("bigButton", "Start Game")),
		mLoadGame(new TextButton("bigButton", "Load Game")),
		mEditor(new TextButton("bigButton", "Editor")),
		mOptions(new TextButton("bigButton", "Options")),
		mQuit(new TextButton("bigButton", "Quit :("))
	{
		setInteractive(true);

		initButton(mStartGame);
		initButton(mLoadGame);
		initButton(mEditor);
		initButton(mOptions);
		initButton(mQuit);

		float buttonHeight = mStartGame->getHeight();
		mStartGame->setWidth(220.0f);
		mStartGame->setParentOffsetY(-buttonHeight * 2.0f);
		mStartGame->addEventListener("click", this);

		mLoadGame->setParentOffsetY(-buttonHeight * 1.0f);
		mLoadGame->setWidth(200.0f);
		mLoadGame->addEventListener("click", this);

		mEditor->setParentOffsetY(-buttonHeight * 0.0f);
		mEditor->setWidth(200.0f);
		mEditor->addEventListener("click", this);

		mOptions->setParentOffsetY(buttonHeight * 1.0f);
		mOptions->setWidth(180.0f);
		mOptions->addEventListener("click", this);

		mQuit->setParentOffsetY(buttonHeight * 2.0f);
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
		if (e->getEventTarget() == mEditor.get())
		{
			am_log("BTN", "EDITOR BUTTON");
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

	void MainMenu::initButton(TextButton *btn)
	{
		addChild(btn);
		btn->setParentAnchor(X_CENTER, Y_CENTER);
		btn->setAnchor(X_CENTER, Y_CENTER);
	}


}
}
