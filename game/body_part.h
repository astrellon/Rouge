#pragma once

#include <base/handle.h>
#include <base/imanaged.h>
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
		BodyPart(const char *name, BodyPartType::PartType type = BodyPartType::UNKNOWN_PART, Item *equipped = NULL);
		BodyPart(const BodyPart &copy);
		~BodyPart();

		const char *getName() const;

		void setType(BodyPartType::PartType type);
		BodyPartType::PartType getType() const;

		void setWeaponPart(bool asWeapon);
		bool isWeaponPart() const;

		//void setOffWeapon(bool offWeapon);
		//bool isOffWeapon() const;

		//void setCanHoldOntoName(const char *partName);
		//const char *getCanHoldOntoName() const;

		void setCanHoldOnto(BodyPart *part);
		BodyPart *getCanHoldOnto() const;

		void setIsHoldingOnto(bool setHolding);
		bool isHoldingOnto() const;

		virtual bool setEquippedItem(Item *item, bool forceEquip = true);
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
