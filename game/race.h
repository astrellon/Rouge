#pragma once

#include <string>
#include <map>
using namespace std;

namespace am {
namespace game {

	class Race {
	public:

		Race(const char *raceName);
		~Race();

		const char *getRaceName() const;

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		string mRaceName;
		
	};

}
}
