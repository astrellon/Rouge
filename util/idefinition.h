#pragma once

#include <string>
using namespace std;

namespace am {
namespace util {

	class IDefinition {
	public :

		void setLoadedName(const char *name);
		const char *getLoadedName() const;

	protected:

		string mLoadedName;

	};

}
}
