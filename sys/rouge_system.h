#pragma once

#include "game_system.h"

#include <base/handle.h>

#include <game/character.h>

namespace am {

namespace game {
	class Engine;
	class PlayerHand;
}

namespace gfx {
	class GfxEngine;
	class Layer;
	class TooltipItem;
}
using namespace am::gfx;

namespace ui {
	class MouseManager;
	class MainMenu;
	class IngameMenu;
	class GameHud;
	class Image;
	class OptionsPanel;
	class EditorHud;
	class UIComponent;
}
using namespace am::ui;

namespace sys {

	class OsSystem;

	class RougeSystem : public GameSystem {
	public:

		~RougeSystem();

		virtual void init();
		virtual void reshape(int width, int height);

		virtual void onKeyUp(ui::Keyboard::Key key);

		virtual void onEvent(Event *e);
		virtual void onEvent(DialogueEvent *e);

		static RougeSystem *createRougeSystem(OsSystem *linked, Engine *engine);
		static RougeSystem *getRougeSystem();

		void newGame();
		void startGame();
		void saveGame(const char *savename);
		void loadGame(const char *savename);
		void startEditor();
		void togglePause();
		void toggleInGameMenu();
		bool isPaused();
		void pauseGame(bool paused);
		void resumeGame();
		void quitGame();
		void toMainMenu();
		void showOptionsPanel();
		void closeOptionsPanel();

		TooltipItem *getItemTooltip() const;

	protected:
		
		RougeSystem(OsSystem *linked, Engine *engine);

		base::Handle<MainMenu> mMainMenu;
		base::Handle<OptionsPanel> mOptionsPanel;
		base::Handle<IngameMenu> mIngameMenu;
		base::Handle<GameHud> mGameHud;
		base::Handle<EditorHud> mEditorHud;
		base::Handle<UIComponent> mCurrentMenu;

		base::Handle<Image> mPausedImage;

		base::Handle<Character> mPlayer;
		base::Handle<TooltipItem> mItemTooltip;

		void checkPaused();
		bool mPausedGame;
		bool mInDialogue;

		PlayerHand *mPlayerHand;
		
		void setCurrentMenu(UIComponent *menu);

		static RougeSystem *sRougeSystem;
	};

}
}
