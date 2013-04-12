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

	class BodyPart {
	public:
		typedef map<string, BodyPart *> BodyPartMap;

		BodyPart(const char *name, Item *equipped = NULL);
		BodyPart(const BodyPart &copy);
		~BodyPart();

		const char *getName() const;

		virtual void setEquippedItem(Item *item);
		virtual Item *getEqippedItem() const;

		virtual data::IData *getSaveObject();

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		string mName;
		Handle<Item> mEquippedItem;

	};

}
}
