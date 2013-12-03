#pragma once

#include <string>

namespace am {
namespace util {

	class IDefinition 
	{
	public :

		void setLoadedName(const char *name);
		const char *getLoadedName() const;

	protected:

		std::string mLoadedName;

	};

}
}
