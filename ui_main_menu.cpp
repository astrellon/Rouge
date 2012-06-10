#include "ui_main_menu.h"

#include "ui_button.h"

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
		mLoadGame->setParentOffsetY(-buttonHeight * 0.5f);
		mOptions->setParentOffsetY(buttonHeight * 0.5f);
		mQuit->setParentOffsetY(buttonHeight * 1.5f);
	}
	
	MainMenu::~MainMenu()
	{
	}

	void MainMenu::initButton(Button *btn)
	{
		addChild(btn);
		btn->setParentAnchor(X_CENTER, Y_CENTER);
		btn->setAnchor(X_CENTER, Y_CENTER);
	}


}
}
