#pragma once

#include <base/imanaged.h>
using namespace am::base;

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class LoadingState;

	class CoinPurse : public IManaged {
	public:

		CoinPurse();
		CoinPurse(const CoinPurse &copy);
		~CoinPurse();

		virtual void setCoin(unsigned int coin);
		virtual unsigned int getCoin() const;

		virtual unsigned int canAddCoin(unsigned int coin);
		virtual unsigned int canRemoveCoin(unsigned int coin);
		
		virtual void addCoin(unsigned int coin);
		virtual void removeCoin(unsigned int coin);

		virtual void setMaxCoin(unsigned int maxCoin);
		virtual unsigned int getMaxCoin() const;

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		
		unsigned int mCoin;
		unsigned int mMaxCoin;

	};

}
}
