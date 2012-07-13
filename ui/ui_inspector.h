#pragma once

#include <game/tile.h>
#include <game/game_object.h>
using namespace am::game;

#include <base/handle.h>
using namespace am::base;

#include <gfx/gfx_text_field.h>
using namespace am::gfx;

#include <vector>
using namespace std;

#include <ui/ui_panel.h>

namespace am {
namespace ui {

	class Inspector : public Panel {
	public:
		Inspector();
		~Inspector();

		void setTile(Tile *tile);
		Tile *getTile() const;

		void addGameObject(GameObject *obj);
		void clearGameObjects();

		virtual TextField *getTextField();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void render(float dt);

	protected:

		Handle<Tile> mTile;
		
		typedef vector< Handle<GameObject> > GameObjectList;
		GameObjectList mGameObjects;

		Handle<TextField> mInfo;

		bool mTextDirty;
		void updateText();
	};

}
}
