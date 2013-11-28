#pragma once

namespace am {
namespace game {

	class Gender 
	{
	public:
		enum GenderType 
		{
			MALE, FEMALE, MAX_GENDER_LENGTH
		};

		static const char *getNiceGenderName(GenderType gender);
		static const char *getGenderName(GenderType gender);

		static GenderType getGenderTypeFromNice(const char *name);
		static GenderType getGenderType(const char *name);

		static GenderType getGenderInt(int genderValue);

	protected:

		static const char *sNiceGenderNames[];
		static const char *sGenderNames[];
	};

}
}
