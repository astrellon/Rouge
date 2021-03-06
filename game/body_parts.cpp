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

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int BodyParts::LUA_ID = LUA_ID_BODYPARTS;
	const char *BodyParts::LUA_TABLENAME = LUA_TABLE_BODYPARTS;

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

	base::ReturnCode BodyParts::addBodyPart(BodyPart *part)
	{
		if (!part || hasBodyPart(part))
		{
			return base::NULL_PARAMETER;
		}
		if (hasBodyPart(part))
		{
			return base::BODY_PART_FOUND;
		}
		mPartList.push_back(part);
		if (mAttackIndex < 0 && (part->isWeaponPart()))
		{
			mAttackIndex = static_cast<int>(mPartList.size()) - 1;
		}
		return base::SUCCESS;
	}
	base::ReturnCode BodyParts::removeBodyPart(const char *partName)
	{
		if (!partName || partName[0] == '\0')
		{
			return base::NULL_PARAMETER;
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
				return base::SUCCESS;
			}
		}
		return base::BODY_PART_NOT_FOUND;
	}
	base::ReturnCode BodyParts::removeBodyPart(BodyPart *part)
	{
		if (!part)
		{
			return base::NULL_PARAMETER;
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
				return base::SUCCESS;
			}
		}
		return base::BODY_PART_NOT_FOUND;
	}

	BodyPart *BodyParts::getBodyPart(const char *partName) const
	{
		if (!partName || partName == '\0')
		{
			return nullptr;
		}
		for (auto iter = mPartList.begin(); iter != mPartList.end(); ++iter)
		{
			if (strcmp(iter->get()->getName(), partName) == 0)
			{
				return iter->get();
			}
		}
		return nullptr;
	}
	Item *BodyParts::getItemOnPart(const char *partName) const
	{
		BodyPart *part = getBodyPart(partName);
		if (part)
		{
			return part->getEquippedItem();
		}
		return nullptr;
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

	BodyPart *BodyParts::getNextWeaponPart(bool &looped)
	{
		int len = static_cast<int>(mPartList.size());
		int startIndex = mAttackIndex;
		looped = false;
		if (startIndex < 0 || startIndex >= len)
		{
			startIndex = len - 1;
		}
		mAttackIndex++;
		if (mAttackIndex >= len)
		{
			mAttackIndex = 0;
		}

		BodyPart *result = nullptr;
		while (mAttackIndex != startIndex)
		{
			BodyPart *part = mPartList[mAttackIndex];
			// Has to be a weapon part and not currently holding something else.
			if (part->isWeaponPart() && !part->isHoldingOnto())
			{
				result = part;
				break;
			}
			mAttackIndex++;
			if (mAttackIndex >= len)
			{
				mAttackIndex = 0;
				looped = true;
			}
		}
		return result;
	}
	int BodyParts::getCurrentAttackIndex() const
	{
		return mAttackIndex;
	}

	base::ReturnCode BodyParts::getLinkedParts(const char *linkedToName, PartList &result) const
	{
		if (!linkedToName || linkedToName[0] == '\0')
		{
			return base::NULL_PARAMETER;
		}
		BodyPart *part = getBodyPart(linkedToName);
		if (!part)
		{
			return base::BODY_PART_NOT_FOUND;
		}
		return getLinkedParts(part, result);
	}
	base::ReturnCode BodyParts::getLinkedParts(BodyPart *linkedTo, PartList &result) const
	{
		if (!linkedTo)
		{
			return base::NULL_PARAMETER;
		}
		if (!hasBodyPart(linkedTo))
		{
			return base::BODY_PART_NOT_FOUND;
		}

		for (size_t i = 0; i < mPartList.size(); i++)
		{
			BodyPart *part = mPartList[i];
			if (part == linkedTo)
			{
				continue;
			}
			if (part->getCanHoldOnto() == linkedTo)
			{
				result.push_back(part);
			}
		}
		return base::SUCCESS;
	}

	int BodyParts::getNumWeaponParts() const
	{
		int total = 0;
		for (size_t i = 0; i < mPartList.size(); i++)
		{
			if (mPartList[i]->isWeaponPart())
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
		base::Handle<data::Table> dataMap(data::Table::checkDataType(data, "body parts"));
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

		base::Handle<data::Number> num(dataMap->at<data::Number>("attackIndex"));
		if (num)
		{
			mAttackIndex = num->integer();
		}
		return 1;
	}

}
}
