#pragma once

#include <base/handle.h>

#include <util/data_table.h>

namespace am {
namespace game {

	using namespace am::util;

	class LoadingState;

	class IAttributeData 
	{
	public:

		IAttributeData();
		IAttributeData(const IAttributeData &copy);
		
		virtual void setAttributes(data::Table *table);
		virtual data::Table *getAttributes(bool create = false);

		virtual void serialise(data::Table *data);
		virtual void deserialise(LoadingState *state, data::Table *data);

	protected:

		base::Handle<data::Table> mAttributes;

	};

}
}
