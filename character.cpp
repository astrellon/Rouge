#include "character.h"

namespace am {
namespace base {

	Character::Character() :
		GameObject(),
		mGraphic(NULL)
	{

	}
	Character::~Character()
	{

	}

	void Character::setGraphic(Sprite *graphic)
	{
		if (mGraphic.get())
		{
			removeChild(mGraphic.get());
		}
		mGraphic = graphic;
		if (graphic)
		{
			addChild(graphic);
		}
	}
	Sprite *Character::getGraphic()
	{
		return mGraphic.get();
	}

	void Character::setName(const char *name)
	{
		mName = name;
	}
	void Character::setName(const string &name)
	{
		mName = name;
	}
	string Character::getName() const
	{
		return mName;
	}

}
}
