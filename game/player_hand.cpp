#include "player_hand.h"

#include <gfx/gfx_engine.h>

namespace am {
namespace game {

	PlayerHand *PlayerHand::sPlayerHand = nullptr;

	PlayerHand::PlayerHand() :
		mEnabled(false)
	{

	}
	PlayerHand::~PlayerHand()
	{

	}

	void PlayerHand::setInhand( Item *item )
	{
		mInhand = item;
		updateGraphic();
	}
	Item *PlayerHand::getInhand() const
	{
		return mInhand;
	}

	void PlayerHand::setHandEnabled(bool enable)
	{
		if (mEnabled != enable)
		{
			mEnabled = enable;
			updateGraphic();
		}
	}
	bool PlayerHand::isHandEnabled() const
	{
		return mEnabled;
	}

	void PlayerHand::updateGraphic()
	{
		if (!mEnabled || mInhand.get() == nullptr)
		{
			gfx::GfxEngine::getEngine()->setCursor(gfx::GfxEngine::getEngine()->getDefaultCursor());
		}
		else if (mInhand.get() != nullptr)
		{
			gfx::GfxEngine::getEngine()->setCursor(mInhand);
		}
	}

	void PlayerHand::setPlayerHand(PlayerHand *hand)
	{
		sPlayerHand = hand;
	}
	PlayerHand *PlayerHand::getPlayerHand()
	{
		return sPlayerHand;
	}

}
}
