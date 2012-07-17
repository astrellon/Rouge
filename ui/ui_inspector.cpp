#include "ui_inspector.h"

#include <sstream>

#include <game/tile_type.h>

namespace am {
namespace ui {

	Inspector::Inspector() :
		Panel(),
		mTextDirty(true)
	{
		mInfo = new TextField();
		mInfo->setColour(0, 0, 0);
		mInfo->setPosition(10, 22);
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
		mInfo->setWidth(width - 18);
	}
	void Inspector::setHeight(float height)
	{
		Panel::setHeight(height);
		mInfo->setHeight(height - 28);
	}

	TextField *Inspector::getTextField()
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
		if (mTile.get())
		{
			ss << "<colour red>Tile:</colour> " << mTile->getFullName() << 
				  "\n<colour red>Desc:</colour> " << mTile->getDescription() << 
				  "\n<colour red>Types:</colour> ";

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

				ss << tileTypes[i]->getFullName();
			}
			//vector<string> typeNames;

			/*if (mTile->isTileType(TILE_TYPE_LAND))
			{
				typeNames.push_back(string("Land"));
			}
			if (mTile->isTileType(TILE_TYPE_WATER))
			{
				typeNames.push_back(string("Water"));
			}
			if (mTile->isTileType(TILE_TYPE_LAVA))
			{
				typeNames.push_back(string("Lava"));
			}
			if (mTile->isTileType(TILE_TYPE_WALL))
			{
				typeNames.push_back(string("Wall"));
			}
			if (mTile->isTileType(TILE_TYPE_AIR))
			{
				typeNames.push_back(string("Air"));
			}

			for (int i = 0; i < typeNames.size(); i++)
			{
				if (i > 0)
				{
					if (i == typeNames.size() - 1)
					{
						ss << " and ";
					}
					else
					{
						ss << ", ";
					}
				}

				ss << typeNames[i];
			}*/
		}
		GameObjectList::iterator iter;
		for (iter = mGameObjects.begin(); iter != mGameObjects.end(); ++iter)
		{
			ss << "<colour red>GameObj:</colour> " << iter->get()->getName() << "\n\n";
		}
		mInfo->setText(ss.str());
		mTextDirty = false;
	}

}
}
