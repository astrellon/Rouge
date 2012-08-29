#include "selector_value.h"

#include <util/utils.h>

namespace am {
namespace util {

	SelectorValue::SelectorValue() :
		mType(EQUAL)
	{

	}
	SelectorValue::SelectorValue(JsonValue value, SelectorValueType type) :
		mType(type),
		mValue(value)
	{

	}
	SelectorValue::~SelectorValue()
	{

	}

	void SelectorValue::setType(SelectorValueType type)
	{
		mType = type;
	}
	SelectorValue::SelectorValueType SelectorValue::getType() const
	{
		return mType;
	}

	void SelectorValue::setValue(const JsonValue &value)
	{
		mValue = value;
	}
	JsonValue SelectorValue::getValue() const
	{
		return mValue;
	}

	bool SelectorValue::match(const JsonValue &value) const
	{
		switch(mType)
		{
		default:
		case EQUAL:
			return mValue == value;
		case OR:
			if (mValue.getType() != JV_ARR)
			{
				return false;
			}
			for (JsonArray::const_iterator iter = mValue.getArr()->begin(), 
				 end = mValue.getArr()->end();
				 iter != end; ++iter)
			{
				if (*iter == value)
				{
					return true;
				}
			}
			return false;
		case LESS_THAN:
			{
				float valueThis = 0.0f;
				bool parsedThis = mValue.castFloat(valueThis);
				float valueRhs = 0.0f;
				bool parsedRhs = value.castFloat(valueRhs);

				if (parsedRhs && parsedThis)
				{
					return valueRhs < valueThis;
				}
				else if (!parsedRhs && !parsedThis && mValue.getType() == JV_STR && value.getType() == JV_STR)
				{
					return *mValue.getStr() < *value.getStr();
				}
			}
			return false;
		case LESS_THAN_EQUAL:
			{
				float valueThis = 0.0f;
				bool parsedThis = mValue.castFloat(valueThis);
				float valueRhs = 0.0f;
				bool parsedRhs = value.castFloat(valueRhs);

				if (parsedRhs && parsedThis)
				{
					return valueRhs <= valueThis;
				}
				else if (!parsedRhs && !parsedThis && mValue.getType() == JV_STR && value.getType() == JV_STR)
				{
					return *mValue.getStr() <= *value.getStr();
				}
			}
			return false;
		break;
		case GREATER_THAN:
			{
				float valueThis = 0.0f;
				bool parsedThis = mValue.castFloat(valueThis);
				float valueRhs = 0.0f;
				bool parsedRhs = value.castFloat(valueRhs);

				if (parsedRhs && parsedThis)
				{
					return valueRhs > valueThis;
				}
				else if (!parsedRhs && !parsedThis && mValue.getType() == JV_STR && value.getType() == JV_STR)
				{
					return *mValue.getStr() > *value.getStr();
				}
			}
			return false;
		case GREATER_THAN_EQUAL:
			{
				float valueThis = 0.0f;
				bool parsedThis = mValue.castFloat(valueThis);
				float valueRhs = 0.0f;
				bool parsedRhs = value.castFloat(valueRhs);

				if (parsedRhs && parsedThis)
				{
					return valueRhs >= valueThis;
				}
				else if (!parsedRhs && !parsedThis && mValue.getType() == JV_STR && value.getType() == JV_STR)
				{
					return *mValue.getStr() >= *value.getStr();
				}
			}
			return false;
			break;
		}
	}

}
}
