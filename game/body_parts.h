#pragma once

#include <base/handle.h>
#include <base/return_codes.h>

#include <string>
#include <map>

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

	class BodyParts 
	{
	public:
		typedef std::vector< base::Handle<BodyPart> > PartList;

		BodyParts();
		BodyParts(const BodyParts &copy);
		~BodyParts();

		/**
		 * Attempts to add the given body part to this collection of body parts.
		 * Return codes:
		 *  SUCCESS: Part was added successfully.
		 *  BODY_PART_FOUND: The given part was already found in the collection.
		 *  NULL_PARAMETER: part was nullptr.
		 */
		base::ReturnCode addBodyPart(BodyPart *part);

		/**
		 * Attempts to remove a body part based on its name.
		 * Return codes:
		 *  SUCCESS: Part was removed.
		 *  BODY_PART_NOT_FOUND: The given part was not found.
		 *  NULL_PARAMETER: part was nullptr.
		 */
		base::ReturnCode removeBodyPart(const char *partName);
		/**
		 * Attempts to remove a body part.
		 * Return codes:
		 *  SUCCESS: Part was removed.
		 *  BODY_PART_NOT_FOUND: The given part was not found.
		 *  NULL_PARAMETER: part was nullptr.
		 */
		base::ReturnCode removeBodyPart(BodyPart *part);

		/**
		 * Returns the equipped item on the part based on its name.
		 */
		Item *getItemOnPart(const char *partName) const;
		/**
		 * Looks for a body part with the given name in this collection.
		 */
		BodyPart *getBodyPart(const char *partName) const;
		/**
		 * Looks for a body part with the same name.
		 */
		bool hasBodyPart(const char *partName) const;
		/**
		 * Looks for the body part in this collection.
		 */
		bool hasBodyPart(const BodyPart *part) const;
		/**
		 * Returns the list of all the body parts in this collection.
		 */
		const PartList &getAllParts() const;

		/**
		 * Looks for the next part in the weapon list.
		 * Returns nullptr only where there are no weapon parts.
		 * Loop is set to true when the body parts index has wrapped around.
		 */
		BodyPart *getNextWeaponPart(bool &looped);
		/**
		 * Returns the total number of weapon parts in this collection.
		 */
		int getNumWeaponParts() const;
		int getCurrentAttackIndex() const;
		
		/**
		 * Returns all the parts that are linked to the given part with the same name.
		 * This means finding all the other parts in this collection
		 * that have the given part as the part that it can hold onto.
		 * Return codes:
		 *  SUCCESS: All went well. The result list can be unaffect if there were no linked parts.
		 *  NULL_PARAMETER: linkedToName was nullptr or an empty string.
		 */
		base::ReturnCode getLinkedParts(const char *linkedToName, PartList &result) const;
		/**
		 * Returns all the parts that are linked to the given part.
		 * This means finding all the other parts in this collection
		 * that have the given part as the part that it can hold onto.
		 * Return codes:
		 *  SUCCESS: All went well. The result list can be unaffect if there were no linked parts.
		 *  NULL_PARAMETER: linkedTo was nullptr.
		 */
		base::ReturnCode getLinkedParts(BodyPart *linkedTo, PartList &result) const;

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
