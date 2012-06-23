#include "ui_ingame_menu.h"

#include "ui_button.h"
#include "rouge_system.h"
#include "logger.h"

namespace am {
namespace ui {

	IngameMenu::IngameMenu(RougeSystem *system) : 
		UIComponent(),
		mRougeSystem(system),
		mResumeGame(new Button("bigButton", "Resume Game")),
		mSaveGame(new Button("bigButton", "Save Game")),
		mLoadGame(new Button("bigButton", "Load Game")),
		mOptions(new Button("bigButton", "Options")),
		mToMainMenu(new Button("bigButton", "To Main Menu")),
		mQuit(new Button("bigButton", "Quit :("))
	{
		setInteractive(true);

		initButton(mResumeGame.get());
		initButton(mSaveGame.get());
		initButton(mLoadGame.get());
		initButton(mOptions.get());
		initButton(mToMainMenu.get());
		initButton(mQuit.get());

		float buttonHeight = mResumeGame->getHeight();
		float buttonWidth = 256.0f;
		mResumeGame->setParentOffsetY(-buttonHeight * 1.5f);
		mResumeGame->addEventListener("click", this);
		mResumeGame->setWidth(buttonWidth);

		mSaveGame->setParentOffset(-buttonWidth * 0.25f, -buttonHeight * 0.5f);
		mSaveGame->addEventListener("click", this);
		mSaveGame->setWidth(buttonWidth * 0.5f);

		mLoadGame->setParentOffset(buttonWidth * 0.25f, -buttonHeight * 0.5f);
		mLoadGame->addEventListener("click", this);
		mLoadGame->setWidth(buttonWidth * 0.5f);

		mOptions->setParentOffsetY(buttonHeight * 0.5f);
		mOptions->addEventListener("click", this);
		mOptions->setWidth(buttonWidth);

		mToMainMenu->setParentOffset(-buttonWidth * 0.25f, buttonHeight * 1.5f);
		mToMainMenu->setWidth(buttonWidth * 0.5f);
		mToMainMenu->addEventListener("click", this);

		mQuit->setParentOffset(buttonWidth * 0.25f, buttonHeight * 1.5f);
		mQuit->setWidth(buttonWidth * 0.5f);
		mQuit->addEventListener("click", this);
	}

	
	IngameMenu::~IngameMenu()
	{
	}

	void IngameMenu::onEvent(Event *e)
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
		if (e->getEventTarget() == mSaveGame.get())
		{
			am_log("BTN", "SAVE GAME");
			return;
		}
		if (e->getEventTarget() == mToMainMenu.get())
		{
			am_log("BTN", "TO MAIN MENU");
			mRougeSystem->toMainMenu();
			return;
		}
		if (e->getEventTarget() == mResumeGame.get())
		{
			am_log("BTN", "RESUME GAME");
			mRougeSystem->resumeGame();
			return;
		}
	}

	void IngameMenu::initButton(Button *btn)
	{
		addChild(btn);
		btn->setParentAnchor(X_CENTER, Y_CENTER);
		btn->setAnchor(X_CENTER, Y_CENTER);
	}

}
}
