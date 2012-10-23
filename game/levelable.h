#pragma once

namespace am {
namespace game {

	class Levelable {
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

		virtual int getExperienceForLevel(short level) const;
		virtual short getLevelFromExperience(int experience) const;

	protected:

		short mLevel;
		short mMaxLevel;
		int mExperience;

		virtual void onLevelUp() {}

		virtual void addLevels(short level);

	};

}
}
