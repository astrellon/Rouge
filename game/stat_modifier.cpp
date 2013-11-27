#include "stat_modifier.h"

#include <util/utils.h>
#include <util/data_string.h>
#include <util/data_boolean.h>
#include <util/data_number.h>
#include <util/data_table.h>
using namespace am::util;

#include <string>
#include <cstring>
#include <time.h>

#include "stats.h"
#include "loading_state.h"

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	const int StatModifier::LUA_ID = LUA_ID_STATMODIFIER;
	const char *StatModifier::LUA_TABLENAME = LUA_TABLE_STATMODIFIER;

	const char *StatModifier::sStatModifierNames[] = {
		"+", "*", "=", "MOD_MAX_LENGTH"
	};

	StatModifier::StatModifier() :
		mValue(0),
		mType(MOD_ADD),
		mStatsParent(nullptr),
		mMagical(true)
	{

	}
	StatModifier::StatModifier(float value, StatModifierType type, bool magical) :
		mValue(value),
		mType(type),
		mStatsParent(nullptr),
		mMagical(magical)
	{

	}
	StatModifier::~StatModifier()
	{
	}

	void StatModifier::setStatsParent(Stats *parent)
	{
		mStatsParent = parent;
	}
	Stats *StatModifier::getStatsParent()
	{
		return mStatsParent;
	}
		
	void StatModifier::setValue(float value)
	{
		mValue = value;
	}
	float StatModifier::getValue() const
	{
		return mValue;
	}

	void StatModifier::setMagical(bool magical)
	{
		mMagical = magical;
	}
	bool StatModifier::isMagical() const
	{
		return mMagical;
	}

	void StatModifier::setType(StatModifierType type)
	{
		mType = type;
	}
	StatModifierType StatModifier::getType() const
	{
		return mType;
	}

	bool StatModifier::operator==(const StatModifier &rhs) const
	{
		float diff = rhs.mValue - mValue;
		return diff < 0.00001f && diff > -0.00001f && rhs.mType == mType && rhs.mStatsParent == mStatsParent;
	}
	bool StatModifier::operator!=(const StatModifier &rhs) const
	{
		float diff = rhs.mValue - mValue;
		return diff > 0.00001f || diff < -0.00001f || rhs.mType != mType || rhs.mStatsParent != mStatsParent;
	}

	StatModifierType StatModifier::getModifierType(const char *name)
	{
		for (int i = 0; i < MOD_MAX_LENGTH; i++)
		{
			if (strcmp(name, sStatModifierNames[i]) == 0)
			{
				return static_cast<StatModifierType>(i);
			}
		}
		return MOD_MAX_LENGTH;
	}
	StatModifierType StatModifier::getModifierType(int value)
	{
		if (value < 0 || value >= MOD_MAX_LENGTH)
		{
			return MOD_MAX_LENGTH;
		}
		return static_cast<StatModifierType>(value);
	}
	const char *StatModifier::getModifierTypeString(StatModifierType type)
	{
		if (type < 0 || type >= MOD_MAX_LENGTH)
		{
			return nullptr;
		}
		return sStatModifierNames[type];
	}

	data::IData *StatModifier::serialise()
	{
		data::Table *output = new data::Table();
		output->at("value", mValue);
		output->at("magical", mMagical);
		output->at("type", getModifierTypeString(mType));

		return output;
	}
	bool StatModifier::deserialise(LoadingState *state, data::IData *data)
	{
		am::base::Handle<data::Table> dataMap(data::Table::checkDataType(data, "stat modifier"));
		if (!dataMap)
		{
			return false;
		}

		am::base::Handle<data::Number> num(dataMap->at<data::Number>("value"));
		if (num)
		{
			setValue(num->value<float>());
		}
		
		am::base::Handle<data::String> str(dataMap->at<data::String>("type"));
		if (str)
		{
			setType(getModifierType(str->string()));
		}

		am::base::Handle<data::Boolean> boo(dataMap->at<data::Boolean>("magical"));
		if (boo)
		{
			setMagical(boo->boolean());
		}

		return true;
	}
	
}
}
