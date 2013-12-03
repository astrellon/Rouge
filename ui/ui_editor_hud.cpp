#include "ui_editor_hud.h"

#include <game/engine.h>
#include <game/game.h>
#include <game/map.h>
#include <game/tile_instance.h>

#include <gfx/gfx_text_field.h>
#include <gfx/gfx_component.h>
#include <gfx/gfx_texture.h>
#include <gfx/gfx_engine.h>
#include <gl.h>

#include <util/utils.h>

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
		mSideSprite->addEventListener(ui::Mouse::MOUSE_DOWN, this);
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
		mStatus = new Label();
		mStatus->getLabelField()->setBaseFont("default:arial");
		mStatus->setParentOffset(20.0f, y);
		mStatus->setGfxComponent(new gfx::GfxComponent());
		mStatus->getGfxComponent()->setColour(1.0f, 0.9f, 0.8f, 1.0f);
		addChild(mStatus);

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
		manager->addEventListener(ui::Mouse::MOUSE_DOWN, this);
		manager->addEventListener(ui::Mouse::MOUSE_MOVE, this);
		manager->addEventListener(ui::Mouse::MOUSE_UP, this);
		KeyboardManager::getManager()->addEventListener(ui::Keyboard::KEY_UP, this);
	}
	EditorHud::~EditorHud()
	{
		MouseManager *manager = MouseManager::getManager();
		manager->removeEventListener(ui::Mouse::MOUSE_DOWN, this);
		manager->removeEventListener(ui::Mouse::MOUSE_MOVE, this);
		manager->removeEventListener(ui::Mouse::MOUSE_UP, this);
		KeyboardManager::getManager()->removeEventListener(ui::Keyboard::KEY_UP, this);

		mTiles->removeEventListener("list_change", this);

		mLoadMap->removeEventListener("click", this);
		mSaveMap->removeEventListener("click", this);
		mLoadFileDialog->removeEventListener("dialog_ok", this);
		mSaveFileDialog->removeEventListener("dialog_ok", this);
	}

	void EditorHud::setGame(Game *game)
	{
		mGame = game;

		am::game::Engine *engine = am::game::Engine::getEngine();
		am::sys::ISystem::FolderEntryList tileSetNames;
		if (am::sys::GameSystem::getGameSystem()->listDirectory("data/tilesets", tileSetNames) == base::SUCCESS)
		{
			for (auto iter = tileSetNames.begin(); iter != tileSetNames.end(); ++iter)
			{
				if (iter->isDirectory)
				{
					continue;
				}
				std::string path("data/tilesets/");
				path += iter->name;
				std::string filename = iter->name;
				size_t index = filename.find_last_of('.');
				if (index != std::string::npos)
				{
					filename = filename.substr(0, index);
				}
				engine->loadDefinitionFile(path.c_str(), filename.c_str());
			}
		}
		am::game::TileSetMap &tileSets = engine->getTileSets();
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
			updateStatus();
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
			map->setMapSize(width, height, mCurrentTile);
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
					map->calcAllTileEdgeValues();
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
					std::string path = mSaveFileDialog->getFullPath();
					if (path.find_last_of(".lua") != path.size() - 1)
					{
						path += ".lua";
					}
					base::ReturnCode result = map->saveMap(path.c_str());
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

		if (e->getEventTarget() == mSideSprite && e->getMouseEventType() == ui::Mouse::MOUSE_DOWN)
		{
			return;
		}

		MouseManager *manager = MouseManager::getManager();
		if (e->getMouseEventType() == ui::Mouse::MOUSE_DOWN)
		{
			manager->setDragOffset(e->getMouseX(), e->getMouseY());

			if (manager->getButtonDown(ui::Mouse::LEFT_BUTTON))
			{
				setTile(e->getMouseX(), e->getMouseY(), mCurrentTile);
			}

			mMouseDown = true;
			return;
		}

		if (e->getMouseEventType() == ui::Mouse::MOUSE_MOVE)
		{
			updateStatus();
			if (mMouseDown)
			{
				if (manager->getButtonDown(ui::Mouse::MIDDLE_BUTTON))
				{
					float dx = static_cast<float>(e->getMouseX() - manager->getDragOffsetX());
					float dy = static_cast<float>(e->getMouseY() - manager->getDragOffsetY());

					Camera *camera = mGame->getCamera();
					float posX = camera->getDestinationX() - dx;
					float posY = camera->getDestinationY() - dy;
					mGame->getCamera()->setDestination(posX, posY);
				}
				else if (manager->getButtonDown(ui::Mouse::LEFT_BUTTON))
				{
					setTile(e->getMouseX(), e->getMouseY(), mCurrentTile);
				}
				manager->setDragOffset(e->getMouseX(), e->getMouseY());
			}
		}
		if (e->getMouseEventType() == ui::Mouse::MOUSE_UP)
		{
			mMouseDown = false;
		}
	}

	void EditorHud::setTile(float mouseX, float mouseY, Tile *tile)
	{
		Map *map = mGame->getCurrentMap();
		if (tile && map && map->getTileRenderer())
		{
			math::Vector2f pos(0.0f, 0.0f);
			map->getTileRenderer()->getScreenToLocal(mouseX, mouseY, pos.x, pos.y);
			math::Vector2i grid = Engine::getEngine()->worldToGrid(pos);
			if (grid.x >= 0 && grid.y >= 0 && grid.x < map->getMapWidth() && grid.y < map->getMapHeight())
			{
				game::TileInstance *instance = mGame->getCurrentMap()->getTileInstance(grid.x, grid.y);
				instance->setTile(tile);
				gfx::Asset *asset = tile->getGraphicAsset();
				instance->randomiseVaritation();
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
		mMouseType(ui::Mouse::MOUSE_OUT)
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

		mHitbox->addEventListener(ui::Mouse::MOUSE_DOWN, this);
		mHitbox->addEventListener(ui::Mouse::MOUSE_MOVE, this);
		mHitbox->addEventListener(ui::Mouse::MOUSE_UP, this);
		mHitbox->addEventListener(ui::Mouse::MOUSE_OUT, this);
		mHitbox->addEventListener(ui::Mouse::MOUSE_OVER, this);
	}
	EditorHud::TileListItem::~TileListItem()
	{
		mHitbox->removeEventListener(ui::Mouse::MOUSE_DOWN, this);
		mHitbox->removeEventListener(ui::Mouse::MOUSE_MOVE, this);
		mHitbox->removeEventListener(ui::Mouse::MOUSE_UP, this);
		mHitbox->removeEventListener(ui::Mouse::MOUSE_OUT, this);
		mHitbox->removeEventListener(ui::Mouse::MOUSE_OVER, this);
	}

	float EditorHud::TileListItem::getHeight()
	{
		return mGraphic->getHeight();
	}

	void EditorHud::TileListItem::preRender(float dt)
	{
		ListItem::preRender(dt);

		bool mouseDown = MouseManager::getManager()->getButtonDown(ui::Mouse::LEFT_BUTTON);

		bool renderBack = false;
		switch (mMouseType)
		{
		case ui::Mouse::MOUSE_MOVE:
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
		case ui::Mouse::MOUSE_OVER:
			glColor4f(0.9f, 0.9f, 0.9f, 0.7f);
			renderBack = true;
			break;
		case ui::Mouse::MOUSE_DOWN:
			glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
			renderBack = true;
			break;
		}
		if (renderBack)
		{
			gfx::Texture::bindTexture(0);
			glBegin(GL_QUADS);
			glVertex2f(0.0f, 0.0f);
			glVertex2f(getWidth(), 0.0f);
			glVertex2f(getWidth(), getHeight());
			glVertex2f(0.0f, getHeight());
			glEnd();
			gfx::GfxEngine::getEngine()->applyColourStack();
		}
	}

	void EditorHud::TileListItem::onEvent(MouseEvent *e)
	{
		base::Handle<Event> clickEvent;
		switch (e->getMouseEventType())
		{
		default:
		case ui::Mouse::MOUSE_OUT:
			mMouseType = ui::Mouse::MOUSE_OUT;
			break;
		case ui::Mouse::MOUSE_MOVE:
			mMouseType = ui::Mouse::MOUSE_MOVE;
			break;
		case ui::Mouse::MOUSE_UP:
			clickEvent = new Event("click", this);
			fireEvent(clickEvent.get());
		case ui::Mouse::MOUSE_OVER:
			mMouseType = ui::Mouse::MOUSE_OVER;
			break;
		case ui::Mouse::MOUSE_DOWN:
			mMouseType = ui::Mouse::MOUSE_DOWN;
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

	void EditorHud::updateStatus()
	{
		std::stringstream status;
		if (mCurrentTile)
		{
			if (mCurrentTile->getTileSet())
			{
				status << mCurrentTile->getTileSet()->getName() << ": ";
			}
			status << mCurrentTile->getName() << ' ';
		}
		else
		{
			status << "<no tile>: ";
		}

		Map *map = mGame->getCurrentMap();
		if (map)
		{
			MouseManager *manager = MouseManager::getManager();
			Vector2f pos(0.0f, 0.0f);
			map->getTileRenderer()->getScreenToLocal(manager->getMouseX(), manager->getMouseY(), pos.x, pos.y);
			Vector2i grid = Engine::getEngine()->worldToGrid(pos);
			status << grid.x << ", " << grid.y;
		}
		else
		{
			status << "<no map>";
		}
		mStatus->setLabel(status.str());
	}
}
}
