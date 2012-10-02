#pragma once

#include <base/handle.h>
using namespace am::base;

#include <string>
#include <map>
using namespace std;

#include <game/item.h>

namespace am {
namespace game {

	class BodyPart {
	public:
		typedef map<string, BodyPart *> BodyPartMap;

		BodyPart(const char *name, Item *equipped = NULL);
		~BodyPart();

		const char *getName() const;

		virtual void setEquippedItem(Item *item);
		virtual Item *getEqippedItem() const;

		//static void addBodyPart(BodyPart *part);
		//static BodyPart *getBodyPart(const char *name);

	protected:

		string mName;
		Handle<Item> mEqippeditem;
		//static BodyPartMap sBodyParts;
	};

}
}
