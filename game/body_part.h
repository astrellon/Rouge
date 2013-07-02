#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
#include <map>
using namespace std;

#include <game/item.h>

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

		enum BodyPartType {
			UNKNOWN_PART, HAND, ARM, LEG, HEAD, TORSO, NECK, SHOULDERS, LEGS, FEET, MAX_BODY_TYPE_LENGTH
		};

		BodyPart(const char *name, BodyPartType type = BodyPart::UNKNOWN_PART, Item *equipped = NULL);
		BodyPart(const BodyPart &copy);
		~BodyPart();

		const char *getName() const;

		void setType(BodyPartType type);
		BodyPartType getType() const;

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

		static const char *getNiceBodyPartName(BodyPartType type);
		static const char *getBodyPartName(BodyPartType type);
		static const char *getBodyPartName(int type);

		static BodyPartType getBodyPartType(int typeValue);
		static BodyPartType getBodyPartTypeFromNice(const char *typeName);
		static BodyPartType getBodyPartType(const char *typeName);

	protected:

		static const char *sNiceBodyPartTypeNames[];
		static const char *sBodyPartTypeNames[];

		string mName;
		Handle<Item> mEquippedItem;
		BodyPartType mType;
		bool mMainWeapon;
		bool mOffWeapon;

	};

}
}
