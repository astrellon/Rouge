#include "body_parts.h"

#include <util/data_table.h>
#include <util/data_number.h>
#include <util/data_boolean.h>
#include <util/data_string.h>
#include <util/utils.h>
using namespace am::util;

#include "loading_state.h"
#include "body_part.h"
#include "item.h"

namespace am {
namespace game {

	const int BodyParts::LUA_ID = 0x31;
	const char *BodyParts::LUA_TABLENAME = "am_game_BodyParts";

	BodyParts::BodyParts() :
		mAttackIndex(-1)
	{

	}
	BodyParts::BodyParts(const BodyParts &copy) :
		mAttackIndex(copy.mAttackIndex),
		mPartList(copy.mPartList)
	{

	}
	BodyParts::~BodyParts()
	{

	}

	bool BodyParts::addBodyPart(BodyPart *part)
	{
		if (!part || hasBodyPart(part))
		{
			return false;
		}
		mPartList.push_back(part);
		if (mAttackIndex < 0 && (part->isMainWeapon() || part->isOffWeapon()))
		{
			mAttackIndex = static_cast<int>(mPartList.size()) - 1;
		}
		return true;
	}
	bool BodyParts::removeBodyPart(const char *partName)
	{
		if (!partName || partName[0] == '\0')
		{
			return false;
		}
		for (size_t i = 0; i < mPartList.size(); i++)
		{
			if (strcmp(mPartList[i]->getName(), partName) == 0)
			{
				mPartList.erase(mPartList.begin() + i);
				if (i == mAttackIndex)
				{
					mAttackIndex--;
				}
				return true;
			}
		}
		return false;
	}
	bool BodyParts::removeBodyPart(BodyPart *part)
	{
		if (!part)
		{
			return false;
		}
		for (size_t i = 0; i < mPartList.size(); i++)
		{
			if (mPartList[i].get() == part)
			{
				mPartList.erase(mPartList.begin() + i);
				if (i == mAttackIndex)
				{
					mAttackIndex--;
				}
				return true;
			}
		}
		return false;
	}

	BodyPart *BodyParts::getBodyPart(const char *partName) const
	{
		if (!partName || partName == '\0')
		{
			return NULL;
		}
		for (auto iter = mPartList.begin(); iter != mPartList.end(); ++iter)
		{
			if (strcmp(iter->get()->getName(), partName) == 0)
			{
				return iter->get();
			}
		}
		return NULL;
	}
	Item *BodyParts::getItemOnPart(const char *partName) const
	{
		BodyPart *part = getBodyPart(partName);
		if (part)
		{
			return part->getEquippedItem();
		}
		return NULL;
	}
	bool BodyParts::hasBodyPart(const char *partName) const
	{
		if (!partName || partName == '\0')
		{
			return false;
		}
		for (auto iter = mPartList.begin(); iter != mPartList.end(); ++iter)
		{
			if (strcmp(iter->get()->getName(), partName) == 0)
			{
				return true;
			}
		}
		return false;
	}
	bool BodyParts::hasBodyPart(const BodyPart *part) const
	{
		if (!part)
		{
			return false;
		}
		for (auto iter = mPartList.begin(); iter != mPartList.end(); ++iter)
		{
			if (iter->get() == part)
			{
				return true;
			}
		}
		return false;
	}

	const BodyParts::PartList &BodyParts::getAllParts() const
	{
		return mPartList;
	}

	BodyPart *BodyParts::getNextAttackPart()
	{
		int startIndex = mAttackIndex;
		mAttackIndex++;
		BodyPart *result = NULL;
		while (mAttackIndex != startIndex)
		{
			BodyPart *part = mPartList[mAttackIndex];
			if (part->isMainWeapon() || part->isOffWeapon())
			{
				result = part;
				break;
			}
			mAttackIndex++;
			if (mAttackIndex >= static_cast<int>(mPartList.size()))
			{
				mAttackIndex = 0;
			}
		}
		return result;
	}

	int BodyParts::getNumMainWeaponParts() const
	{
		int total = 0;
		for (size_t i = 0; i < mPartList.size(); i++)
		{
			if (mPartList[i]->isMainWeapon())
			{
				total++;
			}
		}
		return total;
	}
	int BodyParts::getNumOffWeaponParts() const
	{
		int total = 0;
		for (size_t i = 0; i < mPartList.size(); i++)
		{
			if (mPartList[i]->isOffWeapon())
			{
				total++;
			}
		}
		return total;
	}
	int BodyParts::getNumTotalWeaponParts() const
	{
		int total = 0;
		for (size_t i = 0; i < mPartList.size(); i++)
		{
			if (mPartList[i]->isMainWeapon() || mPartList[i]->isOffWeapon())
			{
				total++;
			}
		}
		return total;
	}

	data::IData *BodyParts::serialise()
	{
		data::Table *output = new data::Table();

		data::Table *partList = new data::Table();
		for (auto iter = mPartList.begin(); iter != mPartList.end(); ++iter)
		{
			partList->push(iter->get()->serialise());
		}
		output->at("partList", partList);

		output->at("attackIndex", mAttackIndex);

		return output;
	}

	int BodyParts::deserialise(LoadingState *state, data::IData *data)
	{
		Handle<data::Table> dataMap(data::Table::checkDataType(data, "body parts"));
		if (!dataMap)
		{
			return 0;
		}

		for (auto iter = dataMap->beginArray(); iter != dataMap->endArray(); ++iter)
		{
			BodyPart *part = BodyPart::fromDeserialise(state, iter->get());
			if (!part)
			{
				continue;
			}
			mPartList.push_back(part);
		}

		Handle<data::Number> num(dataMap->at<data::Number>("attackIndex"));
		if (num)
		{
			mAttackIndex = num->integer();
		}
		return 1;
	}

}
}