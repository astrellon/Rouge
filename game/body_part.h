#pragma once

#include <base/handle.h>
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

	class BodyPart {
	public:
		typedef map<string, BodyPart *> BodyPartMap;

		BodyPart(const char *name, BodyPartType::PartType type = BodyPartType::UNKNOWN_PART, Item *equipped = NULL);
		BodyPart(const BodyPart &copy);
		~BodyPart();

		const char *getName() const;

		void setType(BodyPartType::PartType type);
		BodyPartType::PartType getType() const;

		void setMainWeapon(bool mainWeapon);
		bool isMainWeapon() const;

		void setOffWeapon(bool offWeapon);
		bool isOffWeapon() const;

		virtual void setEquippedItem(Item *item);
		virtual Item *getEqippedItem() const;

		virtual data::IData *serialise();
		virtual bool deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		string mName;
		Handle<Item> mEquippedItem;
		BodyPartType::PartType mType;
		bool mMainWeapon;
		bool mOffWeapon;

	};

}
}
