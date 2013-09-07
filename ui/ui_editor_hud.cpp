#include "ui_editor_hud.h"

#include <game/engine.h>
#include <game/game.h>
#include <game/map.h>
using namespace am::game;

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_component.h>
#include <gfx/gfx_texture.h>
#include <gfx/gfx_engine.h>
#include <gl.h>

#include <util/utils.h>
using namespace am::util;

#include <ui/keyboard_manager.h>
#include <ui/mouse_manager.h>
#include <ui/ui_label.h>
#include <ui/ui_debug_inspector.h>

#include <sstream>

namespace am {
namespace ui {

	EditorHud::EditorHud() :
		UIComponent(),
		IEventListener()
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

		mTiles = new List();
		mTiles->setParentOffset(20.0f, 90.0f);
		mTiles->setWidth(140.0f);
		mTiles->addEventListener("list_change", this);
		addChild(mTiles);

		MouseManager *manager = MouseManager::getManager();
		manager->addEventListener(MOUSE_DOWN, this);
		manager->addEventListener(MOUSE_MOVE, this);
		manager->addEventListener(MOUSE_UP, this);
		KeyboardManager::getManager()->addEventListener(KEY_UP, this);
	}
	EditorHud::~EditorHud()
	{
		MouseManager *manager = MouseManager::getManager();
		manager->removeEventListener(MOUSE_DOWN, this);
		manager->removeEventListener(MOUSE_MOVE, this);
		manager->removeEventListener(MOUSE_UP, this);

		mTiles->removeEventListener("list_change", this);
	}

	void EditorHud::setGame(Game *game)
	{
		mGame = game;

		Engine *engine = Engine::getEngine();
		engine->usingTileSet("nature:nature");
		mTiles->addItem(new TileListItem(engine->getTile("grass")));
		mTiles->addItem(new TileListItem(engine->getTile("dirt")));
		mTiles->addItem(new TileListItem(engine->getTile("water")));
		mTiles->setHeight(200.0f);
	}
	Game *EditorHud::getGame() const
	{
		return mGame;
	}

	void EditorHud::onEvent(ListEvent *e)
	{
		if (!e)
		{
			return;
		}

		TileListItem *item = dynamic_cast<TileListItem *>(e->getItem());
		if (item)
		{
			stringstream ss;
			ss << "Clicked on tile: " << item->getTile()->getName();
			am_log("TILE", ss);
		}
	}

	void EditorHud::onEvent(KeyboardEvent *e)
	{
		if (!e)
		{
			return;
		}

		//if (e->getKey() == 
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
			if (!mGame)
			{
				am_log("EDITOR", "No current game!");
				return;
			}

			Map *map = mGame->getCurrentMap();
			if (!map)
			{
				map = new Map(mMapName->getText().c_str());
				mGame->setCurrentMap(map);
			}
			else
			{
				map->setName(mMapName->getText().c_str());
			}
			map->setMapSize(width, height);
			map->getTileRenderer()->updateAssetSprites();
		}
	}

	void EditorHud::onEvent(MouseEvent *e)
	{
		if (!e || !mGame)
		{
			return;
		}

		MouseManager *manager = MouseManager::getManager();
		if (e->getMouseEventType() == MOUSE_DOWN)
		{
			manager->setDragOffset(e->getMouseX(), e->getMouseY());
			return;
		}

		if (e->getMouseEventType() == MOUSE_MOVE)
		{
			float dx = static_cast<float>(e->getMouseX() - manager->getDragOffsetX());
			float dy = static_cast<float>(e->getMouseY() - manager->getDragOffsetY());

			Camera *camera = mGame->getCamera();
			float posX = camera->getDestinationX() - dx;
			float posY = camera->getDestinationY() - dy;

			MouseManager *manager = MouseManager::getManager();
			if (manager->getButtonDown(MIDDLE_BUTTON))
			{
				mGame->getCamera()->setDestination(posX, posY);
			}
			manager->setDragOffset(e->getMouseX(), e->getMouseY());
		}
		if (e->getMouseEventType() == MOUSE_UP)
		{

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

	EditorHud::TileListItem::TileListItem(Tile *tile) :
		ListItem(),
		mTile(tile),
		mHitbox(new Renderable()),
		mMouseType(ui::MOUSE_OUT)
	{
		setInteractive(true);

		mGraphic = new Sprite(tile->getGraphicAsset());
		mText = new Label(tile->getFullName());
		addChild(mGraphic);
		addChild(mText);
		mText->setParentOffset(40.0f, 8.0f);

		mHitbox->setSize(100.0f, getHeight());
		mHitbox->setInteractive(true);
		addChild(mHitbox);

		mHitbox->addEventListener(MOUSE_DOWN, this);
		mHitbox->addEventListener(MOUSE_MOVE, this);
		mHitbox->addEventListener(MOUSE_UP, this);
		mHitbox->addEventListener(MOUSE_OUT, this);
		mHitbox->addEventListener(MOUSE_OVER, this);
	}
	EditorHud::TileListItem::~TileListItem()
	{
		mHitbox->removeEventListener(MOUSE_DOWN, this);
		mHitbox->removeEventListener(MOUSE_MOVE, this);
		mHitbox->removeEventListener(MOUSE_UP, this);
		mHitbox->removeEventListener(MOUSE_OUT, this);
		mHitbox->removeEventListener(MOUSE_OVER, this);
	}

	float EditorHud::TileListItem::getHeight()
	{
		return mGraphic->getHeight();
	}

	void EditorHud::TileListItem::preRender(float dt)
	{
		ListItem::preRender(dt);

		bool mouseDown = MouseManager::getManager()->getButtonDown(LEFT_BUTTON);

		bool renderBack = false;
		switch (mMouseType)
		{
		case ui::MOUSE_MOVE:
			if (mouseDown)
			{
				glColor4f(0.9f, 0.9f, 0.9f, 0.7f);
			}
			else
			{
				glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
			}
			renderBack = true;
			break;
		case ui::MOUSE_OVER:
			glColor4f(0.9f, 0.9f, 0.9f, 0.7f);
			renderBack = true;
			break;
		case ui::MOUSE_DOWN:
			glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
			renderBack = true;
			break;
		}
		if (renderBack)
		{
			Texture::bindTexture(0);
			glBegin(GL_QUADS);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(getWidth(), 0.0f);
			glVertex2f(getWidth(), getHeight());
			glVertex2f(0.0f, getHeight());
			glEnd();
			GfxEngine::getEngine()->applyColourStack();
		}
	}

	void EditorHud::TileListItem::onEvent(MouseEvent *e)
	{
		Handle<Event> clickEvent;
		switch (e->getMouseEventType())
		{
		default:
		case ui::MOUSE_OUT:
			mMouseType = ui::MOUSE_OUT;
			break;
		case ui::MOUSE_MOVE:
			mMouseType = ui::MOUSE_MOVE;
			break;
		case ui::MOUSE_UP:
			clickEvent = new Event("click", this);
			fireEvent(clickEvent.get());
		case ui::MOUSE_OVER:
			mMouseType = ui::MOUSE_OVER;
			break;
		case ui::MOUSE_DOWN:
			mMouseType = ui::MOUSE_DOWN;
			break;
		}
	}

	Tile *EditorHud::TileListItem::getTile() const
	{
		return mTile;
	}

	void EditorHud::TileListItem::setWidth(float width)
	{
		Layer::setWidth(width);
		mHitbox->setWidth(width);
	}


}
}
