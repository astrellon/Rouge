#pragma once

#include <string>
#include <vector>
using namespace std;

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	enum StatModifierType 
	{
		MOD_ADD, MOD_MULTIPLY, MOD_SET, MOD_MAX_LENGTH
	};

	class Stats;
	class LoadingState;

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

		virtual data::IData *serialise();
		virtual bool deserialise(LoadingState *state, data::IData *data);

		static StatModifierType getModifierType(const char *name);
		static StatModifierType getModifierType(int value);
		static const char *getModifierTypeString(StatModifierType type);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		Stats *mStatsParent;
		float mValue;
		bool mMagical;
		StatModifierType mType;

		static const char *sStatModifierNames[];
	};

}
}
