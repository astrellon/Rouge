#include "coin_purse.h"

namespace am {
namespace game {

	CoinPurse::CoinPurse() :
		mCoin(0),
		mMaxCoin(0)
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

}
}
