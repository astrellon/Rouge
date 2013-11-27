#pragma once

#include <base/handle.h>

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
			am::base::Handle<Tile> mTile;
			am::base::Handle<Sprite> mGraphic;
			am::base::Handle<Label> mText;
			am::base::Handle<Renderable> mHitbox;
			ui::Mouse::EventType mMouseType;

			void preRender(float dt);
		};

		bool mMouseDown;

		am::base::Handle<Game> mGame;
		am::base::Handle<Sprite> mSideSprite;
		am::base::Handle<TextInput> mMapName;
		am::base::Handle<TextInput> mMapFullName;
		am::base::Handle<TextInput> mMapWidth;
		am::base::Handle<TextInput> mMapHeight;
		am::base::Handle<TextButton> mMakeMap;
		am::base::Handle<TextButton> mLoadMap;
		am::base::Handle<TextButton> mSaveMap;
		am::base::Handle<Label> mStatus;
		am::base::Handle<List> mTiles;

		am::base::Handle<FileDialog> mLoadFileDialog;
		am::base::Handle<FileDialog> mSaveFileDialog;

		am::base::Handle<Tile> mCurrentTile;

		void updateStatus();
		void setTile(float mouseX, float mouseY, Tile *tile);
	};

}
}
