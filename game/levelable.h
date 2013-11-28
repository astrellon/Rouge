#pragma once

#include <ui/event_interface.h>

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class LoadingState;

	class Levelable 
	{
	public:

		Levelable();
		~Levelable();

		virtual void addExperience(int experience);
		virtual void setExperience(int experience);
		virtual int getExperience() const;

		virtual void addLevel(short level);
		virtual void setLevel(short level);
		virtual short getLevel() const;

		virtual void setMaxLevel(short level);
		virtual short getMaxLevel() const;

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		virtual int getExperienceForLevel(short level) const;
		virtual short getLevelFromExperience(int experience) const;

	protected:

		short mLevel;
		short mMaxLevel;
		int mExperience;

		virtual void onLevelUp() {}
		virtual void onExperienceChange() {}
		void _setExperience(int exp);

		virtual void addLevelsUntil(short level);

	};

}
}
