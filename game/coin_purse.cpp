#include "coin_purse.h"

#include <game/loading_state.h>

#include <util/data_table.h>
#include <util/data_number.h>

#include <lua/wrappers/lua_id_table.h>

namespace am {
namespace game {

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
	
	void CoinPurse::setCoin(unsigned int coin)
	{
		if (mMaxCoin > 0 && coin > mMaxCoin)
		{
			coin = mMaxCoin;
		}
		mCoin = coin;
	}
	unsigned int CoinPurse::getCoin() const
	{
		return mCoin;
	}

	unsigned int CoinPurse::canAddCoin(unsigned int coin)
	{
		int diff = static_cast<int>(coin + mCoin - mMaxCoin);
		if (mMaxCoin > 0 && diff > 0)
		{
			return diff;
		}
		return 0;
	}
	unsigned int CoinPurse::canRemoveCoin(unsigned int coin)
	{
		if (mCoin < coin) 
		{
			return coin - mCoin;
		}
		return 0;
	}
	
	void CoinPurse::addCoin(unsigned int coin)
	{
		if (mMaxCoin > 0 && coin + mCoin > mMaxCoin)
		{
			mCoin = mMaxCoin;
		}
		else
		{
			mCoin += coin;
		}
	}
	void CoinPurse::removeCoin(unsigned int coin)
	{
		if (mCoin < coin)
		{
			mCoin = 0;
		}
		else
		{
			mCoin -= coin;
		}
	}

	void CoinPurse::setMaxCoin(unsigned int maxCoin)
	{
		mMaxCoin = maxCoin;
		if (maxCoin > 0 && mCoin > maxCoin)
		{
			mCoin = maxCoin;
		}
	}
	unsigned int CoinPurse::getMaxCoin() const
	{
		return mMaxCoin;
	}

	data::IData *CoinPurse::serialise()
	{
		data::Table *output = new data::Table();
		output->at("coin", mCoin);
		output->at("maxCoin", mMaxCoin);

		return output;
	}
	int CoinPurse::deserialise(LoadingState *state, data::IData *data)
	{
		Handle<data::Table> dataMap(data::Table::checkDataType(data, "coin purse"));
		if (!dataMap)
		{
			return 0;
		}

		Handle<data::Number> num(dataMap->at<data::Number>("maxCoin"));
		if (num)
		{
			setMaxCoin(num->value<unsigned int>());
		}
		num = dataMap->at<data::Number>("coin");
		if (num)
		{
			setCoin(num->value<unsigned int>());
		}

		return 1;
	}

}
}
