#include "body_part_common.h"

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
			return NULL;
		}
		return sNiceBodyPartTypeNames[type];
	}
	const char *BodyPartType::getBodyPartName(PartType type)
	{
		if (type < 0 || type >= MAX_BODY_TYPE_LENGTH)
		{
			return NULL;
		}
		return sBodyPartTypeNames[type];
	}
	const char *BodyPartType::getBodyPartName(int type)
	{
		if (type < 0 || type >= MAX_BODY_TYPE_LENGTH)
		{
			return NULL;
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
		if (typeName == NULL || typeName[0] == '\0')
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
		if (typeName == NULL || typeName[0] == '\0')
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

}
}
