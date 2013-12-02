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

namespace sys {

	class OsSystem;

	class RougeSystem : public GameSystem 
	{
	public:

		~RougeSystem();

		virtual void init();
		virtual void reshape(int width, int height);

		virtual void onKeyUp(ui::Keyboard::Key key);

		virtual void onEvent(ui::Event *e);
		virtual void onEvent(ui::DialogueEvent *e);

		static RougeSystem *createRougeSystem(OsSystem *linked, game::Engine *engine);
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

		gfx::TooltipItem *getItemTooltip() const;

	protected:
		
		RougeSystem(OsSystem *linked, game::Engine *engine);

		base::Handle<ui::MainMenu> mMainMenu;
		base::Handle<ui::OptionsPanel> mOptionsPanel;
		base::Handle<ui::IngameMenu> mIngameMenu;
		base::Handle<ui::GameHud> mGameHud;
		base::Handle<ui::EditorHud> mEditorHud;
		base::Handle<ui::UIComponent> mCurrentMenu;

		base::Handle<ui::Image> mPausedImage;

		base::Handle<game::Character> mPlayer;
		base::Handle<gfx::TooltipItem> mItemTooltip;

		void checkPaused();
		bool mPausedGame;
		bool mInDialogue;

		game::PlayerHand *mPlayerHand;
		
		void setCurrentMenu(ui::UIComponent *menu);

		static RougeSystem *sRougeSystem;
	};

}
}
