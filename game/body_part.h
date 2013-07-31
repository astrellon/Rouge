#pragma once

#include <base/handle.h>
#include <base/imanaged.h>
#include <base/return_codes.h>
using namespace am::base;

#include <string>
#include <map>
using namespace std;

#include <game/item.h>
#include <game/body_part_common.h>

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class LoadingState;

	class BodyPart : public IManaged {
	public:
		BodyPart(const char *name, BodyPartType::PartType type = BodyPartType::UNKNOWN_PART, Item *equipped = nullptr);
		BodyPart(const BodyPart &copy);
		~BodyPart();

		const char *getName() const;

		void setType(BodyPartType::PartType type);
		BodyPartType::PartType getType() const;

		void setWeaponPart(bool asWeapon);
		bool isWeaponPart() const;

		void setCanHoldOnto(BodyPart *part);
		BodyPart *getCanHoldOnto() const;

		void setIsHoldingOnto(bool setHolding);
		bool isHoldingOnto() const;

		/**
		 * Attemps to set the equipped item on this body part.
		 * Return codes:
		 *  SUCCESS: The item was equipped.
		 *  BODY_PART_TYPE_MISMATCH: Unable to equip the item because the item does
		 *		not support this body part type.
		 */
		virtual ReturnCode setEquippedItem(Item *item, bool forceEquip = true);
		virtual bool canEquipItem(Item *item) const;
		virtual Item *getEquippedItem() const;

		virtual data::IData *serialise();
		virtual bool deserialise(LoadingState *state, data::IData *data);

		static BodyPart *fromDeserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		BodyPart();

		string mName;
		Handle<BodyPart> mCanHoldOnto;
		Handle<Item> mEquippedItem;
		BodyPartType::PartType mType;
		bool mIsHoldingOnto;
		bool mWeaponPart;
		//bool mOffWeapon;

	};

}
}
