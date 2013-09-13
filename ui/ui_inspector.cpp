#include "ui_inspector.h"

#include <sstream>

#include <log/logger.h>
#include <util/utils.h>

#include <game/tile_type.h>
#include <game/engine.h>
#include <game/game.h>
#include <game/character.h>

namespace am {
namespace ui {

	Inspector::Inspector() :
		Panel(),
		mTextDirty(true)
	{
		mInfo = new TextField2();
		//mInfo->setColour(0, 0, 0);
		mInfo->setPosition(10, 22);
		mInfo->setInteractive(true);
		addChild(mInfo);

		setTitle("Inspector");

		mInfo->addEventListener(MOUSE_UP, this);
	}
	Inspector::~Inspector()
	{
	}

	void Inspector::setTile(Tile *tile)
	{
		if (tile != mTile)
		{
			mTile = tile;
			mTextDirty = true;
		}
	}
	Tile *Inspector::getTile() const
	{
		return mTile.get();
	}

	void Inspector::addGameObject(GameObject *obj)
	{
		mGameObjects.push_back(obj);
		mTextDirty = true;
	}
	void Inspector::addGameObjects(const ObjectList &list)
	{
		if (list.size() == 0)
		{
			return;
		}
		ObjectList::const_iterator iter;
		for (iter = list.begin(); iter != list.end(); ++iter)
		{
			mGameObjects.push_back(iter->get());
		}
		mTextDirty = true;
	}
	void Inspector::clearGameObjects()
	{
		mGameObjects.clear();
		mTextDirty = true;
	}
	
	void Inspector::setWidth(float width)
	{
		Panel::setWidth(width);
		mInfo->setWidth(mWidth - 18);
	}
	void Inspector::setHeight(float height)
	{
		Panel::setHeight(height);
		mInfo->setHeight(mHeight - 28);
	}

	TextField2 *Inspector::getTextField()
	{
		return mInfo;
	}

	void Inspector::render(float dt)
	{
		if (mTextDirty)
		{
			updateText();
		}
		UIComponent::render(dt);
	}

	void Inspector::onEvent(MouseEvent *e)
	{
		NodeHitbox *textHit = dynamic_cast<NodeHitbox *>(e->getEventTarget());
		if (textHit != nullptr)
		{
			int index = -1;
			if (Utils::fromString<int>(index, textHit->getNodeTarget()->getAttribute("index")))
			{
				GameObject *clickedOn = mGameObjects[index];
				Item *item = dynamic_cast<Item *>(clickedOn);
				if (item)
				{
					Engine::getEngine()->getCurrentGame()->getMainCharacter()->pickupItem(item);
					mGameObjects.erase(mGameObjects.begin() + index);
					mTextDirty = true;
					return;
				}
				Character *character = dynamic_cast<Character *>(clickedOn);
				Character *mainChar = Engine::getGame()->getMainCharacter();
				if (mainChar && mainChar->getDialogueComp() && character && character->getDialogueComp())
				{
					mainChar->getDialogueComp()->talkTo(character);
					return;
				}
			}
		}
		else
		{
			Panel::onEvent(e);
		}
	}

	void Inspector::updateText()
	{
		stringstream ss;
		ss << "<inspector>";
		if (mTile.get())
		{
			ss << "<title>Tile:</title> " << mTile->getFullName() << 
				  "\n<title>Desc:</title> " << mTile->getDescription() << 
				  "\n<title>Types:</title> ";

			Tile::TileTypeList &tileTypes = mTile->getTileTypes();
			for (size_t i = 0; i < tileTypes.size(); i++)
			{
				if (i > 0)
				{
					if (i == tileTypes.size() - 1)
					{
						ss << " and ";
					}
					else
					{
						ss << ", ";
					}
				}

				ss << "<tile>" << tileTypes[i]->getFullName() << "</tile>";
			}
		}
		ss << "\n\n";
		GameObjectList::iterator iter;

		if (mGameObjects.size() > 0)
		{
			ss << "<gameobj>GameObj:</gameobj>\n";
			int i = 0;
			for (iter = mGameObjects.begin(); iter != mGameObjects.end(); ++iter)
			{
				ss << "- <gameobj index=" << i << " class='" << (*iter)->getGameObjectTypeName() << "'>" << iter->get()->getName() << "</gameobj>\n";
				i++;
			}
		}
		
		ss << "</inspector>";
		mInfo->setText(ss.str());
		mTextDirty = false;
	}

}
}
