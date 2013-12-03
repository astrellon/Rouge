#pragma once

#include <game/tile.h>
#include <game/tile_instance.h>
#include <game/game_object.h>

#include <base/handle.h>

#include <gfx/gfx_text_field2.h>

#include <vector>

#include <ui/ui_panel.h>

namespace am {
namespace ui {

	class Inspector : public Panel
	{
	public:
		Inspector();
		~Inspector();

		virtual void setTileInstance(game::TileInstance *instance);
		virtual game::TileInstance *getTileInstance() const;

		virtual void addGameObject(game::GameObject *obj);
		virtual void addGameObjects(const game::ObjectList &list);
		virtual void clearGameObjects();

		virtual void setInspectObject(game::GameObject *obj);

		virtual gfx::TextField2 *getTextField();

		virtual void setWidth(float width);
		virtual void setHeight(float height);

		virtual void onEvent(MouseEvent *e);

		virtual void render(float dt);

	protected:

		TileInstance *mTileInstance;
		
		typedef std::vector< base::Handle<game::GameObject> > GameObjectList;
		GameObjectList mGameObjects;

		base::Handle<gfx::TextField2> mInfo;

		bool mInspectObject;
		bool mTextDirty;
		void updateText();
	};

}
}
