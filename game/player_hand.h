#pragma once

#include <base/handle.h>
using namespace am::base;

#include "item.h"

namespace am {
namespace game {

	class PlayerHand {
	public:
		PlayerHand();
		~PlayerHand();

		void setInhand(Item *item);
		Item *getInhand() const;

		void setHandEnabled(bool enable);
		bool isHandEnabled() const;

		static void setPlayerHand(PlayerHand *hand);
		static PlayerHand *getPlayerHand();

	protected:

		bool mEnabled;
		Handle<Item> mInhand;
		
		void updateGraphic();

		static PlayerHand *sPlayerHand;

	};

}
}
