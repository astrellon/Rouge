#pragma once

#include <string>
#include <vector>
using namespace std;

namespace am {
namespace game {

	enum MODIFIER_TYPE {
		MOD_ADD, MOD_MULTIPLY, MOD_MULTIPLY_SET
	};

	class Stats;

	class StatModifier {
	public:

		StatModifier();
		StatModifier(float value, MODIFIER_TYPE type);
		~StatModifier();
		
		virtual void setValue(float value);
		virtual float getValue() const;

		virtual void setType(MODIFIER_TYPE type);
		virtual MODIFIER_TYPE getType() const;

		virtual void setStatsParent(Stats *parent);
		virtual Stats *getStatsParent();

		bool operator==(const StatModifier &rhs) const;
		bool operator!=(const StatModifier &rhs) const;

	protected:

		Stats *mStatsParent;
		float mValue;
		MODIFIER_TYPE mType;
	};

}
}
