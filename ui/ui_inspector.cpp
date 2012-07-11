#include "ui_inspector.h"

#include <sstream>

namespace am {
namespace ui {

	Inspector::Inspector() :
		mTextDirty(true)
	{
		mInfo = new TextField();
		mInfo->setAlignment(TextField::ALIGN_RIGHT);
		addChild(mInfo);
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
		UIComponent::setWidth(width);
		mInfo->setWidth(width);
	}
	void Inspector::setHeight(float height)
	{
		UIComponent::setHeight(height);
		mInfo->setHeight(height);
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
		ss << "Inspector:\n";
		if (mTile.get())
		{
			ss << "Tile: " << mTile->getFullName() << "\nDesc: " << mTile->getDescription() << "\n\n";
		}
		GameObjectList::iterator iter;
		for (iter = mGameObjects.begin(); iter != mGameObjects.end(); ++iter)
		{
			ss << "GameObj: " << iter->get()->getName() << "\n\n";
		}
		mInfo->setText(ss.str());
		mTextDirty = false;
	}

}
}
