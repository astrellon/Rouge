#include "ui_editor_hud.h"

#include <game/engine.h>
#include <game/game.h>
#include <game/map.h>
using namespace am::game;

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_component.h>

#include <util/utils.h>
using namespace am::util;

#include <sstream>

namespace am {
namespace ui {

	EditorHud::EditorHud()
	{
		setName("EditorHud");
		mSideSprite = new Sprite("editor:side_bar");
		addChild(mSideSprite);

		mMapName = new TextInput();
		addChild(mMapName);
		mMapName->setSize(90.0f, 16.0f);
		mMapName->setParentOffset(20.0f, 20.0f);

		mMapWidth = new TextInput();
		addChild(mMapWidth);
		mMapWidth->setSize(40.0f, 16.0f);
		mMapWidth->setParentOffset(20.0f, 42.0f);
		mMapWidth->setMaxCharacters(4);
		mMapWidth->setRestriction(TextInput::INTEGER);

		mMapHeight = new TextInput(*mMapWidth);
		mMapHeight->setParentOffsetX(70.0f);
		addChild(mMapHeight);

		mMakeMap = new TextButton("ui:small_button", "Set Size");
		mMakeMap->setSize(90.0f, 20.0f);
		mMakeMap->setParentOffset(20.0f, 64.0f);
		mMakeMap->getLabelField()->getGfxComponent()->setColour(1, 1, 1, 1);
		mMakeMap->addEventListener("click", this);
		addChild(mMakeMap);
	}
	EditorHud::~EditorHud()
	{

	}

	void EditorHud::onEvent(Event *e)
	{
		if (!e)
		{
			return;
		}
		if (e->getEventTarget() == mMakeMap)
		{
			int width = Utils::tryFromString(mMapWidth->getText(), 0);
			int height = Utils::tryFromString(mMapHeight->getText(), 0);

			if (width <= 0 || height <= 0)
			{
				return;
			}
			Game *game = Engine::getGame();
			if (!game)
			{
				am_log("EDITOR", "No current game!");
				return;
			}
			Map *map = game->getCurrentMap();
			if (!map)
			{
				map = new Map(mMapName->getText().c_str());
				game->setCurrentMap(map);
			}
			else
			{
				map->setName(mMapName->getText().c_str());
			}
			map->setMapSize(width, height);
		}
	}

	void EditorHud::loadAssets()
	{

	}

	Sprite *EditorHud::getSideSprite()
	{
		return mSideSprite.get();
	}

	void EditorHud::setHeight(float height)
	{
		mSideSprite->setHeight(height);
		UIComponent::setHeight(height);
	}

}
}
