#include "iattribute_data.h"

namespace am {
namespace game {

	IAttributeData::IAttributeData()
	{
	}

	IAttributeData::IAttributeData(const IAttributeData &copy)
	{
		if (copy.mAttributes)
		{
			mAttributes = dynamic_cast<data::Table *>(copy.mAttributes->clone());
		}
	}

	void IAttributeData::setAttributes(data::Table *table)
	{
		mAttributes = table;
	}
	data::Table *IAttributeData::getAttributes(bool create)
	{
		if (!mAttributes && create)
		{
			mAttributes = new data::Table();
		}
		return mAttributes;
	}

	void IAttributeData::serialise(data::Table *output)
	{
		if (mAttributes)
		{
			output->at("attributes", mAttributes);
		}
	}
	void IAttributeData::deserialise(LoadingState *state, data::Table *data)
	{
		base::Handle<data::Table> attrs(data->at<data::Table>("attributes"));
		if (attrs)
		{
			mAttributes = attrs;
		}
	}

}
}
