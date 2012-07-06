#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
using namespace std;

#include <gfx/gfx_sprite.h>
using namespace am::gfx;

#include "game_object.h"
#include "item_common.h"

namespace am {
namespace game {

	class Item : public GameObject {
	public:
		Item();
		~Item();

		virtual void setGraphic(Sprite *graphic);
		virtual Sprite *getGraphic();

		virtual void setGroundGraphic(Sprite *graphic);
		virtual Sprite *getGroundGraphic();

		virtual void setItemType(ItemType type);
		virtual ItemType getItemType() const;

		virtual void setInventorySize(short sizeX, short sizeY);
		virtual short getInventorySizeX() const;
		virtual short getInventorySizeY() const;

		virtual void setMinDamage(float dmg);
		virtual float getMinDamage() const;

		virtual void setMaxDamage(float dmg);
		virtual float getMaxDamage() const;

		virtual void setArmourClass(float ac);
		virtual float getArmourClass() const;

		virtual void setOnGround(bool ground);
		virtual bool isOnGround() const;

		virtual void setQuestItemId(int questItemId);
		virtual int getQuestItemId() const;
		virtual bool isQuestItem() const;

		virtual void setItemFrom(const Item &item);

		virtual void update(float dt);

	protected:
		
		Handle<Sprite> mGraphic;
		Handle<Sprite> mGroundGraphic;
		
		ItemType mItemType;
		bool mOnGround;

		short mInventorySizeX;
		short mInventorySizeY;

		int mQuestItemId;

		float mMinDamage;
		float mMaxDamage;
		float mArmourClass;

		void updateGraphic();
	};

}
}
