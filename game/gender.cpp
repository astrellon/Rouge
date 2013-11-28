#include "gender.h"

#include <string>

namespace am {
namespace game {

	const char *Gender::sNiceGenderNames[] = 
	{
		"Male", "Female", "MAX_GENDER_LENGTH"
	};
	const char *Gender::sGenderNames[] = 
	{
		"male", "female", "max_gender_length"
	};

	const char *Gender::getNiceGenderName(GenderType type)
	{
		return sNiceGenderNames[type];
	}
	const char *Gender::getGenderName(GenderType type)
	{
		return sGenderNames[type];
	}

	Gender::GenderType Gender::getGenderTypeFromNice(const char *name)
	{
		int i = 0;
		for (; i < MAX_GENDER_LENGTH; i++)
		{
			if (strcmp(name, sNiceGenderNames[i]) == 0)
			{
				break;
			}
		}
		return static_cast<GenderType>(i);
	}
	Gender::GenderType Gender::getGenderType(const char *name)
	{
		int i = 0;
		for (; i < MAX_GENDER_LENGTH; i++)
		{
			if (strcmp(name, sGenderNames[i]) == 0)
			{
				break;
			}
		}
		return static_cast<GenderType>(i);
	}

	Gender::GenderType Gender::getGenderInt(int genderValue)
	{
		if (genderValue < 0 || genderValue >= Gender::MAX_GENDER_LENGTH) 
		{
			return Gender::MAX_GENDER_LENGTH;
		}
		return static_cast<Gender::GenderType>(genderValue);
	}
	
}
}
