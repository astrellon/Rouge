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

		virtual void addLevel(int level);
		virtual void setLevel(int level);
		virtual int getLevel() const;

		virtual void setMaxLevel(int level);
		virtual int getMaxLevel() const;

		virtual int getExperienceForLevel(int level) const;
		virtual int getLevelFromExperience(int experience) const;

	protected:

		int mLevel;
		int mMaxLevel;
		int mExperience;

		virtual void onLevelUp() {}

		virtual void addLevels(int level);

	};

}
}
