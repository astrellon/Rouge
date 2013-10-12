#pragma once

#include <base/imanaged.h>
using namespace am::base;

#include <ui/event_interface.h>
using namespace am::ui;

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class LoadingState;

	class CoinPurse : public EventInterface {
	public:

		CoinPurse();
		CoinPurse(const CoinPurse &copy);
		~CoinPurse();

		virtual void setCoin(int coin);
		virtual int getCoin() const;

		virtual int canAddCoin(int coin);
		virtual int canRemoveCoin(int coin);
		
		virtual void addCoin(int coin);
		virtual void removeCoin(int coin);

		virtual void setMaxCoin(int maxCoin);
		virtual int getMaxCoin() const;

		virtual data::IData *serialise();
		virtual int deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		
		int mCoin;
		int mMaxCoin;

		void changeCoinValue(int coin);

	};

}
}
