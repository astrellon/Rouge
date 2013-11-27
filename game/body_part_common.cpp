#include "body_part_common.h"

#include <util/data_table.h>
#include <util/data_string.h>

#include <cstring>

namespace am {
namespace game {

	const char *BodyPartType::sNiceBodyPartTypeNames[] = {
		"Unknown Part", "None", "Any", "Hand", "Arm", "Leg", "Head", "Torso", "Neck", "Shoulders", "Legs", "Feet", "MAX_BODY_TYPE_LENGTH"
	};
	const char *BodyPartType::sBodyPartTypeNames[] = {
		"unknown_part", "none", "any", "hand", "arm", "leg", "head", "torso", "neck", "shoulders", "legs", "feet", "MAX_BODY_TYPE_LENGTH"
	};

	const char *BodyPartType::getNiceBodyPartName(PartType type)
	{
		if (type < 0 || type >= MAX_BODY_TYPE_LENGTH)
		{
			return nullptr;
		}
		return sNiceBodyPartTypeNames[type];
	}
	const char *BodyPartType::getBodyPartName(PartType type)
	{
		if (type < 0 || type >= MAX_BODY_TYPE_LENGTH)
		{
			return nullptr;
		}
		return sBodyPartTypeNames[type];
	}
	const char *BodyPartType::getBodyPartName(int type)
	{
		if (type < 0 || type >= MAX_BODY_TYPE_LENGTH)
		{
			return nullptr;
		}
		return sBodyPartTypeNames[type];
	}
	
	BodyPartType::PartType BodyPartType::getBodyPartType(int typeValue)
	{
		if (typeValue < 0 || typeValue > MAX_BODY_TYPE_LENGTH)
		{
			return MAX_BODY_TYPE_LENGTH;
		}
		return static_cast<PartType>(typeValue);
	}
	BodyPartType::PartType BodyPartType::getBodyPartTypeFromNice(const char *typeName)
	{
		if (typeName == nullptr || typeName[0] == '\0')
		{
			return MAX_BODY_TYPE_LENGTH;
		}
		for (int i = 0; i < MAX_BODY_TYPE_LENGTH; i++)
		{
			if (strcmp(typeName, sNiceBodyPartTypeNames[i]) == 0)
			{
				return static_cast<PartType>(i);
			}
		}
		return MAX_BODY_TYPE_LENGTH;
	}
	BodyPartType::PartType BodyPartType::getBodyPartType(const char *typeName)
	{
		if (typeName == nullptr || typeName[0] == '\0')
		{
			return MAX_BODY_TYPE_LENGTH;
		}
		for (int i = 0; i < MAX_BODY_TYPE_LENGTH; i++)
		{
			if (strcmp(typeName, sBodyPartTypeNames[i]) == 0)
			{
				return static_cast<PartType>(i);
			}
		}
		return MAX_BODY_TYPE_LENGTH;
	}

	data::IData *BodyPartType::serialiseTypeList(const BodyPartType::TypeList &list)
	{
		data::Table *output = new data::Table();
		for (size_t i = 0; i < list.size(); i++)
		{
			output->push(getBodyPartName(list[i]));
		}
		return output;
	}
	void BodyPartType::deserialiseTypeList(data::IData *data, BodyPartType::TypeList &result)
	{
		am::base::Handle<data::Table> typeList(data::Table::checkDataType(data, "body part type list"));
		if (!typeList)
		{
			return;
		}

		for (auto iter = typeList->beginArray(); iter != typeList->endArray(); ++iter)
		{
			BodyPartType::PartType type = getBodyPartType(iter->get()->string());
			if (type != BodyPartType::MAX_BODY_TYPE_LENGTH)
			{
				result.push_back(type);
			}
		}
	}

}
}
