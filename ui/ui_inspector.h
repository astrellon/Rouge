#pragma once

#include <game/tile.h>
#include <game/tile_instance.h>
#include <game/game_object.h>
using namespace am::game;

#include <base/handle.h>

#include <gfx/gfx_text_field2.h>
using namespace am::gfx;

#include <vector>

#include <ui/ui_panel.h>

namespace am {
namespace ui {

	class Inspector : public Panel {
	public:
		Inspector();
		~Inspector();

		virtual void setTileInstance(TileInstance *instance);
		virtual TileInstance *getTileInstance() const;

		virtual void addGameObject(GameObject *obj);
		virtual void addGameObjects(const ObjectList &list);
		virtual void clearGameObjects();

		virtual void setInspectObject(GameObject *obj);

		virtual TextField2 *getTextField();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(MouseEvent *e);

		virtual void render(float dt);

	protected:

		TileInstance *mTileInstance;
		
		typedef std::vector< base::Handle<GameObject> > GameObjectList;
		GameObjectList mGameObjects;

		base::Handle<TextField2> mInfo;

		bool mInspectObject;
		bool mTextDirty;
		void updateText();
	};

}
}
