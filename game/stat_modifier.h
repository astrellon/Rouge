#pragma once

#include <string>
#include <vector>
using namespace std;

namespace am {
namespace game {

	enum StatModifierType 
	{
		MOD_ADD, MOD_MULTIPLY, MOD_MULTIPLY_SET, MOD_MAX_LENGTH
	};

	class Stats;

	class StatModifier {
	public:

		StatModifier();
		StatModifier(float value, StatModifierType type, bool magical = true);
		~StatModifier();
		
		virtual void setValue(float value);
		virtual float getValue() const;

		virtual void setType(StatModifierType type);
		virtual StatModifierType getType() const;

		virtual void setMagical(bool magical);
		virtual bool isMagical() const;

		virtual void setStatsParent(Stats *parent);
		virtual Stats *getStatsParent();

		bool operator==(const StatModifier &rhs) const;
		bool operator!=(const StatModifier &rhs) const;

		static StatModifierType getModifierType(const char *name);
		static StatModifierType getModifierType(int value);

	protected:

		Stats *mStatsParent;
		float mValue;
		bool mMagical;
		StatModifierType mType;

		static const char *sStatModifierNames[];
	};

}
}
