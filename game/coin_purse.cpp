#include "coin_purse.h"

#include <game/loading_state.h>

#include <util/data_table.h>
#include <util/data_number.h>

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

	using am::util::data::Table;
	using am::util::data::Number;
	using base::Handle;

	const int CoinPurse::LUA_ID = LUA_ID_COINPURSE;
	const char *CoinPurse::LUA_TABLENAME = LUA_TABLE_COINPURSE;

	CoinPurse::CoinPurse() :
		mCoin(0),
		mMaxCoin(0)
	{
		
	}
	CoinPurse::CoinPurse(const CoinPurse &copy) :
		mCoin(copy.mCoin),
		mMaxCoin(copy.mMaxCoin)
	{

	}
	CoinPurse::~CoinPurse()
	{

	}
	
	void CoinPurse::setCoin(int coin)
	{
		if (mMaxCoin > 0 && coin > mMaxCoin)
		{
			coin = mMaxCoin;
		}
		changeCoinValue(coin);
	}
	int CoinPurse::getCoin() const
	{
		return mCoin;
	}

	int CoinPurse::canAddCoin(int coin)
	{
		int diff = static_cast<int>(coin + mCoin - mMaxCoin);
		if (mMaxCoin > 0 && diff > 0)
		{
			return diff;
		}
		return 0;
	}
	int CoinPurse::canRemoveCoin(int coin)
	{
		if (mCoin < coin) 
		{
			return mCoin - coin;
		}
		return 0;
	}
	
	void CoinPurse::addCoin(int coin)
	{
		if (mMaxCoin > 0 && coin + mCoin > mMaxCoin)
		{
			changeCoinValue(mMaxCoin);
		}
		else
		{
			changeCoinValue(mCoin + coin);
		}
	}
	void CoinPurse::removeCoin(int coin)
	{
		if (mCoin < coin)
		{
			changeCoinValue(0);
		}
		else
		{
			changeCoinValue(mCoin - coin);
		}
	}

	void CoinPurse::setMaxCoin(int maxCoin)
	{
		mMaxCoin = maxCoin;
		if (maxCoin > 0 && mCoin > maxCoin)
		{
			changeCoinValue(maxCoin);
		}
	}
	int CoinPurse::getMaxCoin() const
	{
		return mMaxCoin;
	}

	IData *CoinPurse::serialise()
	{
		Table *output = new Table();
		output->at("coin", mCoin);
		output->at("maxCoin", mMaxCoin);

		return output;
	}
	int CoinPurse::deserialise(LoadingState *state, IData *data)
	{
		Handle<Table> dataMap(Table::checkDataType(data, "coin purse"));
		if (!dataMap)
		{
			return 0;
		}

		Handle<Number> num(dataMap->at<Number>("maxCoin"));
		if (num)
		{
			setMaxCoin(num->value<int>());
		}
		num = dataMap->at<Number>("coin");
		if (num)
		{
			setCoin(num->value<int>());
		}

		return 1;
	}

	void CoinPurse::changeCoinValue(int coin)
	{
		if (coin != mCoin)
		{
			mCoin = coin;
			Handle<ui::Event> e(new ui::Event("coin_change"));
			fireEvent<ui::Event>(e);
		}
	}
}
}
