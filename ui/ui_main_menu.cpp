#include "ui_main_menu.h"

#include <sys/rouge_system.h>
#include <log/logger.h>

#include "ui_text_button.h"
#include <gfx/gfx_tooltip_string.h>

namespace am {
namespace ui {

	MainMenu::MainMenu(sys::RougeSystem *system) : 
		UIComponent(),
		mRougeSystem(system),
		mStartGame(new TextButton("ui:big_button", "Start Game")),
		mLoadGame(new TextButton("ui:big_button", "Load Game")),
		mEditor(new TextButton("ui:big_button", "Editor")),
		mOptions(new TextButton("ui:big_button", "Options")),
		mQuit(new TextButton("ui:big_button", "Quit :("))
	{
		setInteractive(true);

		initButton(mStartGame);
		initButton(mLoadGame);
		initButton(mEditor);
		initButton(mOptions);
		initButton(mQuit);

		float buttonHeight = mStartGame->getHeight();
		mStartGame->setWidth(220.0f);
		mStartGame->setTooltip(new gfx::TooltipString("Start Game", "Really start das game."));
		mStartGame->setParentOffsetY(-buttonHeight * 2.0f);
		mStartGame->addEventListener("click", this);

		mLoadGame->setParentOffsetY(-buttonHeight * 1.0f);
		mLoadGame->setTooltip(new gfx::TooltipString("Load Game!"));
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
			mRougeSystem->startEditor();
			return;
		}
		if (e->getEventTarget() == mLoadGame.get())
		{
			am_log("BTN", "LOAD GAME");
			mRougeSystem->loadGame("save1");
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
