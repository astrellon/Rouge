#pragma once

#include <ui/event_interface.h>

namespace am {
namespace util {
namespace data {
	class IData;
}
}

namespace game {

	using am::util::data::IData;

	class LoadingState;

	class CoinPurse : public ui::EventInterface
	{
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

		virtual IData *serialise();
		virtual int deserialise(LoadingState *state, IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:
		
		int mCoin;
		int mMaxCoin;

		void changeCoinValue(int coin);

	};

}
}
