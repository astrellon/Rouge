#include "idefinition.h"

namespace am {
namespace util {

	void IDefinition::setLoadedName(const char *name)
	{
		if (name && name[0] != '\0')
		{
			mLoadedName = name;
		}
	}
	const char *IDefinition::getLoadedName() const
	{
		return mLoadedName.c_str();
	}

}
}
