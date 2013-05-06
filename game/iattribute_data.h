#pragma once

#include <base/handle.h>
using namespace am::base;

#include <util/data_table.h>
using namespace am::util;

namespace am {
namespace game {

	class LoadingState;

	class IAttributeData {
	public:

		IAttributeData();
		IAttributeData(const IAttributeData &copy);
		
		virtual void setAttributes(data::Table *table);
		virtual data::Table *getAttributes(bool create = false);

		virtual void serialise(data::Table *data);
		virtual void deserialise(LoadingState *state, data::Table *data);

	protected:

		Handle<data::Table> mAttributes;

	};

}
}
