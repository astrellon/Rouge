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

		virtual void onKeyUp(am::ui::Keyboard::Key key);

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

		am::base::Handle<MainMenu> mMainMenu;
		am::base::Handle<OptionsPanel> mOptionsPanel;
		am::base::Handle<IngameMenu> mIngameMenu;
		am::base::Handle<GameHud> mGameHud;
		am::base::Handle<EditorHud> mEditorHud;
		am::base::Handle<UIComponent> mCurrentMenu;

		am::base::Handle<Image> mPausedImage;

		am::base::Handle<Character> mPlayer;
		am::base::Handle<TooltipItem> mItemTooltip;

		void checkPaused();
		bool mPausedGame;
		bool mInDialogue;

		PlayerHand *mPlayerHand;
		
		void setCurrentMenu(UIComponent *menu);

		static RougeSystem *sRougeSystem;
	};

}
}
