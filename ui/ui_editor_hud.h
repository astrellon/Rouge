#pragma once

#include <base/handle.h>
using namespace am::base;

#include <game/tile.h>
using namespace am::game;

#include <gfx/gfx_sprite.h>
#include <gfx/gfx_layer.h>
using namespace am::gfx;

#include <ui/ui_component.h>
#include <ui/ui_text_input.h>
#include <ui/ui_text_button.h>
#include <ui/ui_list.h>
#include <ui/ui_label.h>
#include <ui/ui_file_dialog.h>

#include <game/game.h>

namespace am {
namespace ui {

	class EditorHud : public UIComponent, public IEventListener {
	public:
		EditorHud();
		~EditorHud();

		Sprite *getSideSprite();
		void loadAssets();

		virtual void setHeight(float width);

		virtual void onEvent(Event *e);
		virtual void onEvent(KeyboardEvent *e);
		virtual void onEvent(MouseEvent *e);
		virtual void onEvent(ListEvent *e);

		virtual void setGame(Game *game);
		virtual Game *getGame() const;

	protected:

		class TileListItem : public ListItem {
		public:
			TileListItem(Tile *tile);
			~TileListItem();

			virtual void onEvent(MouseEvent *e);

			virtual void setWidth(float width);
			virtual float getHeight();

			virtual Tile *getTile() const;
			
		protected:
			Handle<Tile> mTile;
			Handle<Sprite> mGraphic;
			Handle<Label> mText;
			Handle<Renderable> mHitbox;
			ui::Mouse::EventType mMouseType;

			void preRender(float dt);
		};

		bool mMouseDown;

		Handle<Game> mGame;
		Handle<Sprite> mSideSprite;
		Handle<TextInput> mMapName;
		Handle<TextInput> mMapFullName;
		Handle<TextInput> mMapWidth;
		Handle<TextInput> mMapHeight;
		Handle<TextButton> mMakeMap;
		Handle<TextButton> mLoadMap;
		Handle<TextButton> mSaveMap;
		Handle<Label> mStatus;
		Handle<List> mTiles;

		Handle<FileDialog> mLoadFileDialog;
		Handle<FileDialog> mSaveFileDialog;

		Handle<Tile> mCurrentTile;

		void updateStatus();
		void setTile(float mouseX, float mouseY, Tile *tile);
	};

}
}
