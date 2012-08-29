#pragma once

#include <util/json_value.h>

namespace am {
namespace util {

	class SelectorValue {
	public:

		enum SelectorValueType {
			EQUAL, OR, LESS_THAN, GREATER_THAN, LESS_THAN_EQUAL, GREATER_THAN_EQUAL
		};

		SelectorValue();
		~SelectorValue();

		void setType(SelectorValueType type);
		SelectorValueType getType() const;

		void setValue(const JsonValue &value);
		JsonValue getValue() const;

		bool match(const JsonValue &value) const;

	protected:

		SelectorValueType mType;
		JsonValue mValue;

	};

}
}
