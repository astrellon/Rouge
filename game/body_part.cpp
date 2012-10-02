#include "body_part.h"

namespace am {
namespace game {

	//BodyPart::BodyPartMap BodyPart::sBodyParts;

	BodyPart::BodyPart(const char *name, Item *equipped) :
		mName(name),
		mEqippeditem(equipped)
	{

	}
	BodyPart::~BodyPart()
	{

	}

	const char *BodyPart::getName() const
	{
		return mName.c_str();
	}

	void BodyPart::setEquippedItem(Item *item)
	{
		mEqippeditem = item;
	}
	Item *BodyPart::getEqippedItem() const
	{
		return mEqippeditem;
	}

	/*void BodyPart::addBodyPart(BodyPart *part)
	{
		if (part == NULL)
		{
			return;
		}
		string name = part->getName();
		sBodyParts[name] = part;
	}
	BodyPart *BodyPart::getBodyPart(const char *name)
	{
		if (name == NULL || name[0] == '\0')
		{
			return NULL;
		}
		string nameStr = name;
		BodyPartMap::iterator iter = sBodyParts.find(nameStr);
		if (iter == sBodyParts.end())
		{
			return NULL;
		}
		return iter->second;
	}*/

}
}
