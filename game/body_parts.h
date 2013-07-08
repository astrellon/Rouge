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
		//typedef map<string, Handle<BodyPart> > PartMap;
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

		data::IData *serialise();
		int deserialise(LoadingState *state, data::IData *data);

		static const int LUA_ID;
		static const char *LUA_TABLENAME;

	protected:

		//PartMap mPartMap;
		PartList mPartList;

	};

}
}
