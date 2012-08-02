#include "ui_inspector.h"

#include <sstream>

#include <game/tile_type.h>

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
	void Inspector::clearGameObjects()
	{
		mGameObjects.clear();
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
			for (int i = 0; i < tileTypes.size(); i++)
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
			for (iter = mGameObjects.begin(); iter != mGameObjects.end(); ++iter)
			{
				ss << "- <gameobj class='" << (*iter)->getGameObjectTypeName() << "'>" << iter->get()->getName() << "</gameobj>\n";
			}
		}
		
		ss << "</inspector>";
		mInfo->setText(ss.str());
		mTextDirty = false;
	}

}
}
