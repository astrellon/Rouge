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
		mTextDirty(true),
		mInspectObject(false)
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
		mInspectObject = false;
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
		mInspectObject = false;
	}
	void Inspector::clearGameObjects()
	{
		mGameObjects.clear();
		mTextDirty = true;
		mInspectObject = false;
	}

	void Inspector::setInspectObject(GameObject *obj)
	{
		mGameObjects.clear();
		mGameObjects.push_back(obj);
		mTextDirty = true;
		mInspectObject = true;
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
		NodeHitbox *textHit = dynamic_cast<NodeHitbox *>(e->getOriginalTarget());
		if (textHit != nullptr)
		{
			int index = -1;
			if (Utils::fromString<int>(index, textHit->getNodeTarget()->getAttribute("index")))
			{
				GameObject *clickedOn = mGameObjects[index];
				if (clickedOn->getGameObjectType() == GameObject::ITEM)
				{
					Item *item = dynamic_cast<Item *>(clickedOn);
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
		if (mInspectObject)
		{
			if (mGameObjects.size() >= 1)
			{
				GameObject *obj = mGameObjects[0];
				ss << "<title>Name:</title> " << obj->getName() << 
					"\n<title>Description:</title>\n" << obj->getDescription();
			}
		}
		else
		{
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
			GameObjectList::iterator iter;

			if (mGameObjects.size() > 0)
			{
				ss << "\n<gameobj>GameObj:</gameobj>\n";
				int i = 0;
				for (iter = mGameObjects.begin(); iter != mGameObjects.end(); ++iter)
				{
					ss << "- <gameobj index=" << i << " class='" << (*iter)->getGameObjectTypeName() << "'>" << iter->get()->getName() << "</gameobj>\n";
					i++;
				}
			}
		}
		ss << "</inspector>";
		mInfo->setText(ss.str());
		mTextDirty = false;
	}

}
}
