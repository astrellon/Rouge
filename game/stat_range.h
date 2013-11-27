#pragma once

#include <string>
#include <vector>

namespace am {
namespace game {

	class StatRange {
	public:

		StatRange();
		StatRange(float value);
		StatRange(float min, float max);
		~StatRange();

		virtual void fromDiceString(const char *input);

		virtual void setMin(float min);
		virtual float getMin() const;

		virtual void setMax(float max);
		virtual float getMax() const;
		
		virtual float genValue() const;

		StatRange operator+(const StatRange &rhs);
		StatRange operator-(const StatRange &rhs);

	protected:

		float mMin;
		float mMax;
	};

}
}
