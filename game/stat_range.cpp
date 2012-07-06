#include "stat_range.h"

#include <util/stat_tokeniser.h>
#include <util/utils.h>
using namespace am::util;

#include <string>
#include <cstring>
#include <time.h>

namespace am {
namespace game {

	StatRange::StatRange() :
		mMin(0.0),
		mMax(0.0f)
	{
	}
	StatRange::StatRange(float value) :
		mMin(value),
		mMax(value)
	{
	}
	StatRange::StatRange(float min, float max) :
		mMin(min),
		mMax(max)
	{
	}
	StatRange::~StatRange()
	{
	}

	void StatRange::fromDiceString(const char *input)
	{
		float temp = 0.0f;
		
		vector<StatRange> values;

		StatTokeniser tokeniser(input, "+-dD");
		const char *token = tokeniser.nextToken();
		char ch = '\0';
		while (token != NULL)
		{
			ch = token[0];
			if (ch == '+')
			{
				StatRange v1 = values.back();
				values.pop_back();
				StatRange v2 = values.back();
				values.pop_back();
				values.push_back(v1 + v2);
			}
			else if (ch == '-')
			{
				StatRange v1 = values.back();
				values.pop_back();
				StatRange v2 = values.back();
				values.pop_back();
				values.push_back(v1 - v2);
			}
			else if (ch == 'd' || ch == 'D')
			{
				StatRange v1 = values.back();
				values.pop_back();
				StatRange v2 = values.back();
				values.pop_back();
				StatRange r;
				r.mMin = v1.mMin;
				r.mMax = v1.mMin * v2.mMin;
				values.push_back(r);
			}
			else
			{
				bool result = from_string<float>(temp, token);
				if (result)
				{
					StatRange v(temp);
					values.push_back(v);
				}
			}
			token = tokeniser.nextToken();
		}

		*this = values.back();
	}

	StatRange StatRange::operator+(const StatRange &rhs)
	{
		return StatRange(mMin + rhs.mMin, mMax + rhs.mMax);
	}
	StatRange StatRange::operator-(const StatRange &rhs)
	{
		return StatRange(mMin - rhs.mMin, mMax - rhs.mMax);
	}

	void StatRange::setMin(float min)
	{
		mMin = min;
	}
	float StatRange::getMin() const
	{
		return mMin;
	}

	void StatRange::setMax(float max)
	{
		mMax = max;
	}
	float StatRange::getMax() const
	{
		return mMax;
	}

	float StatRange::genValue() const
	{
		float randValue = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
		return randValue * (mMax - mMin) + mMin;
	}
	
}
}
