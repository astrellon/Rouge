#include "return_codes.h"

namespace am {
namespace base {

	const char *getErrorMessage(ReturnCode code)
	{
		switch (code)
		{
			// General
		case SUCCESS:			return "success";
		case NULL_PARAMETER:	return "a parameter was null or an empty string.";
		case INTERNAL_ERROR:	return "there was an unexpected internal error.";
		case FILE_NOT_FOUND:	return "file not found.";
			// Character
		case ABLE_TO_EQUIP:		return "the character is able to equip the item.";
		case CAN_EQUIP:			return "the character can equip but is currently unable to.";
		case BODY_PART_TYPE_MISMATCH:	return "item cannot be equipped because the body part type on the part to be equipped was not found in the list of types on the items.";
		case NOT_ENOUGH_BODY_PARTS:	return "not enough body parts to equip the item.";
		case OUT_OF_RANGE:		return "target is out of range.";
		case NO_WEAPON_PART:	return "this character has no body parts that can attack.";
		case OFF_THE_MAP:		return "given location is outside of the map bounds.";
		case INVALID_LOCATION:	return "given location is invalid for the game object.";
			// Body Parts
		case BODY_PART_FOUND:		return "body part was already found in the parts list.";
		case BODY_PART_NOT_FOUND:	return "unable to find the body part with the given part name.";
		case BODY_PART_TYPE_UNKNOWN:	return "the body part type name does not match a known type name.";
			// Texture
		case TEXTURE_LOAD_FAILED:	return "unable to load";
			// System
		case NO_LINKED_SYSTEM:	return "unable to make call because there is no linked OS system.";
			// Inventory
		case NOT_ENOUGH_INVENTORY_SPACE:	return "not enough space for the given item.";
			// Default
		default:
			return "unknown return code.";
		}
	}

}
}
