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

	protected:

		string mRaceName;
		
	};

}
}
