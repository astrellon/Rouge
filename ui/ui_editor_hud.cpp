#include "ui_editor_hud.h"

#include <game/engine.h>
#include <game/game.h>
#include <game/map.h>
#include <game/tile_instance.h>
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

#include <sys/game_system.h>

#include <sstream>

namespace am {
namespace ui {

	EditorHud::EditorHud() :
		UIComponent(),
		IEventListener(),
		mMouseDown(false)
	{
		setName("EditorHud");
		mSideSprite = new Sprite("editor:side_bar");
		addChild(mSideSprite);

		float y = 20.0f;
		mLoadMap = new TextButton("ui:small_button", "Load");
		mLoadMap->setSize(50.0f, 20.0f);
		mLoadMap->setParentOffset(20.0f, y);
		addChild(mLoadMap);

		mSaveMap = new TextButton("ui:small_button", "Save");
		mSaveMap->setSize(50.0f, 20.0f);
		mSaveMap->setParentOffset(80.0f, y);
		addChild(mSaveMap);

		mLoadMap->addEventListener("click", this);
		mSaveMap->addEventListener("click", this);

		y += 22.0f;
		mMapName = new TextInput();
		addChild(mMapName);
		mMapName->setSize(140.0f, 16.0f);
		mMapName->setParentOffset(20.0f, y);

		y += 22.0f;
		mMapFullName = new TextInput();
		addChild(mMapFullName);
		mMapFullName->setSize(140.0f, 40.0f);
		mMapFullName->setParentOffset(20.0f, y);

		y += 46.0f;
		mMapWidth = new TextInput();
		addChild(mMapWidth);
		mMapWidth->setSize(40.0f, 16.0f);
		mMapWidth->setParentOffset(20.0f, y);
		mMapWidth->setMaxCharacters(4);
		mMapWidth->setRestriction(TextInput::INTEGER);

		mMapHeight = new TextInput(*mMapWidth);
		mMapHeight->setParentOffsetX(70.0f);
		addChild(mMapHeight);

		y += 22.0f;
		mMakeMap = new TextButton("ui:small_button", "Set Size");
		mMakeMap->setSize(90.0f, 20.0f);
		mMakeMap->setParentOffset(20.0f, y);
		mMakeMap->getLabelField()->getGfxComponent()->setColour(1, 1, 1, 1);
		mMakeMap->addEventListener("click", this);
		addChild(mMakeMap);

		y += 24.0f;
		mTiles = new List();
		mTiles->setParentOffset(20.0f, y);
		mTiles->setWidth(140.0f);
		mTiles->addEventListener("list_change", this);
		addChild(mTiles);

		mLoadFileDialog = new FileDialog("data/maps", "Load Map");
		mLoadFileDialog->setVisible(false);
		mLoadFileDialog->setSize(200.0f, 80.0f);
		mLoadFileDialog->setParentAnchor(X_CENTER, Y_CENTER);
		mLoadFileDialog->addEventListener("dialog_ok", this);
		addChild(mLoadFileDialog);

		mSaveFileDialog = new FileDialog("data/maps", "Save Map");
		mSaveFileDialog->setVisible(false);
		mSaveFileDialog->setSize(200.0f, 80.0f);
		mSaveFileDialog->setParentAnchor(X_CENTER, Y_CENTER);
		mSaveFileDialog->addEventListener("dialog_ok", this);
		addChild(mSaveFileDialog);

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

		mLoadMap->removeEventListener("click", this);
		mSaveMap->removeEventListener("click", this);
		mLoadFileDialog->removeEventListener("dialog_ok", this);
		mSaveFileDialog->removeEventListener("dialog_ok", this);
	}

	void EditorHud::setGame(Game *game)
	{
		mGame = game;

		Engine *engine = Engine::getEngine();
		ISystem::FolderEntryList tileSetNames;
		if (GameSystem::getGameSystem()->listDirectory("data/tilesets", tileSetNames) == SUCCESS)
		{
			for (auto iter = tileSetNames.begin(); iter != tileSetNames.end(); ++iter)
			{
				if (iter->isDirectory)
				{
					continue;
				}
				string path("data/tilesets/");
				path += iter->name;
				string filename = iter->name;
				size_t index = filename.find_last_of('.');
				if (index != string::npos)
				{
					filename = filename.substr(0, index);
				}
				engine->loadDefinitionFile(path.c_str(), filename.c_str());
			}
		}
		TileSetMap &tileSets = engine->getTileSets();
		for (auto iter = tileSets.begin(); iter != tileSets.end(); ++iter)
		{
			const TileSet::TileMap &tiles = iter->second->getTiles();
			for (auto tileIter = tiles.begin(); tileIter != tiles.end(); ++tileIter)
			{
				mTiles->addItem(new TileListItem(tileIter->second));
			}

		}
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
			mCurrentTile = item->getTile();
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
				map = new Map(mMapName->getText());
				mGame->setCurrentMap(map);
			}
			else
			{
				map->setName(mMapName->getText());
				map->setFullName(mMapFullName->getText());
			}
			map->setMapSize(width, height);
			map->getTileRenderer()->updateAssetSprites();
		}
		else if (e->getEventTarget() == mLoadMap)
		{
			mLoadFileDialog->setVisible(true);
		}
		else if (e->getEventTarget() == mSaveMap)
		{
			mSaveFileDialog->setVisible(true);
		}
		else if (e->getType() == "dialog_ok")
		{
			if (e->getEventTarget() == mLoadFileDialog)
			{
				mGame->setCurrentMap(mLoadFileDialog->getFilename());
				Map *map = mGame->getCurrentMap();
				if (map)
				{
					mMapWidth->setText(Utils::toString(map->getMapWidth()));
					mMapHeight->setText(Utils::toString(map->getMapHeight()));
					mMapName->setText(map->getName());
					mMapFullName->setText(map->getFullName());
				}
			}
			if (e->getEventTarget() == mSaveFileDialog)
			{
				Map *map = mGame->getCurrentMap();
				if (map)
				{
					ReturnCode result = map->saveMap(mSaveFileDialog->getFullPath().c_str());
					am_log("SAVE", getErrorMessage(result));
				}
				//am_log("SAVE", mSaveFileDialog->getFilename());
			}
		}
	}

	void EditorHud::onEvent(MouseEvent *e)
	{
		if (!e || !mGame || !e->isPropagating())
		{
			return;
		}

		MouseManager *manager = MouseManager::getManager();
		if (e->getMouseEventType() == MOUSE_DOWN)
		{
			manager->setDragOffset(e->getMouseX(), e->getMouseY());

			if (manager->getButtonDown(LEFT_BUTTON))
			{
				setTile(e->getMouseX(), e->getMouseY(), mCurrentTile);
			}

			mMouseDown = true;
			return;
		}

		if (e->getMouseEventType() == MOUSE_MOVE && mMouseDown)
		{
			if (manager->getButtonDown(MIDDLE_BUTTON))
			{
				float dx = static_cast<float>(e->getMouseX() - manager->getDragOffsetX());
				float dy = static_cast<float>(e->getMouseY() - manager->getDragOffsetY());

				Camera *camera = mGame->getCamera();
				float posX = camera->getDestinationX() - dx;
				float posY = camera->getDestinationY() - dy;
				mGame->getCamera()->setDestination(posX, posY);
			}
			else if (manager->getButtonDown(LEFT_BUTTON))
			{
				setTile(e->getMouseX(), e->getMouseY(), mCurrentTile);
			}
			manager->setDragOffset(e->getMouseX(), e->getMouseY());
		}
		if (e->getMouseEventType() == MOUSE_UP)
		{
			mMouseDown = false;
		}
	}

	void EditorHud::setTile(float mouseX, float mouseY, Tile *tile)
	{
		Map *map = mGame->getCurrentMap();
		if (tile && map && map->getTileRenderer())
		{
			Vector2f pos(0.0f, 0.0f);
			map->getTileRenderer()->getScreenToLocal(mouseX, mouseY, pos.x, pos.y);
			Vector2i grid = Engine::getEngine()->worldToGrid(pos);
			if (grid.x >= 0 && grid.y >= 0 && grid.x < map->getMapWidth() && grid.y < map->getMapHeight())
			{
				TileInstance *instance = mGame->getCurrentMap()->getTileInstance(grid.x, grid.y);
				instance->setTile(tile);
				Asset *asset = tile->getGraphicAsset();
				if (asset->getFrameRate() <= 0.0f)
				{
					if (asset->isSubWindowAnimation())
					{
						instance->setBaseVariation(Utils::rand(0, asset->getTotalSubWindows()));
					}
					else
					{
						instance->setBaseVariation(Utils::rand(0, asset->getTotalTextures()));
					}
				}
				else
				{
					instance->setBaseVariation(0);
				}
				map->getTileRenderer()->updateAssetSprite(tile);
				map->calcTileEdgeValuesAround(grid.x, grid.y);
			}
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
