#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
#include <map>
using namespace std;

#include <game/item.h>
#include <game/body_part_common.h>
#include <game/body_part.h>

namespace am {
namespace util {
namespace data {
	class IData;
}
}
using namespace am::util;

namespace game {

	class LoadingState;
	class Item;

	class BodyParts {
	public:
		typedef vector< Handle<BodyPart> > PartList;

		BodyParts();
		BodyParts(const BodyParts &copy);
		~BodyParts();

		bool addBodyPart(BodyPart *part);
		bool removeBodyPart(const char *partName);
		bool removeBodyPart(BodyPart *part);
		Item *getItemOnPart(const char *partName) const;
		BodyPart *getBodyPart(const char *partName) const;
		bool hasBodyPart(const char *partName) const;
		bool hasBodyPart(const BodyPart *part) const;
		const PartList &getAllParts() const;

		BodyPart *getNextAttackPart();
		//int getNumMainWeaponParts() const;
		//int getNumOffWeaponParts() const;
		int getNumWeaponParts() const;
		
		bool getLinkedParts(const char *linkedToName, PartList &result) const;
		bool getLinkedParts(BodyPart *linkedTo, PartList &result) const;

		data::IData *serialise();
		int deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		PartList mPartList;
		int mAttackIndex;

	};

}
}
