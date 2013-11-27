#include "return_codes.h"

namespace am {
namespace base {

	const char *getErrorName(am::base::ReturnCode code)
	{
		switch (code)
		{
			// General
		case SUCCESS:			return "success";
		case NULL_PARAMETER:	return "null_parameter";
		case INTERNAL_ERROR:	return "internal_error";
		case FILE_NOT_FOUND:	return "file_not_found";
			// Character
		case ABLE_TO_EQUIP:		return "able_to_equip";
		case CAN_EQUIP:			return "can_equip";
		case BODY_PART_TYPE_MISMATCH:	return "body_part_type_mismatch";
		case NOT_ENOUGH_BODY_PARTS:	return "not_enough_body_parts";
		case OUT_OF_RANGE:		return "out_of_range";
		case NO_WEAPON_PART:	return "no_weapon_part";
		case OFF_THE_MAP:		return "off_the_map";
		case INVALID_LOCATION:	return "invalid_location";
			// Body Parts
		case BODY_PART_FOUND:		return "body_part_found";
		case BODY_PART_NOT_FOUND:	return "body_part_not_found";
		case BODY_PART_TYPE_UNKNOWN:	return "body_part_type_unknown";
			// Texture
		case TEXTURE_LOAD_FAILED:	return "texture_load_failed";
			// System
		case NO_LINKED_SYSTEM:	return "no_linked_system";
			// Inventory
		case NOT_ENOUGH_INVENTORY_SPACE:	return "not_enough_inventory_space";
		case ITEM_NOT_IN_INVENTORY:	return "item_not_in_inventory";
			// Store
		case NOT_ENOUGH_COIN:		return "not_enough_coin";
		case NO_STORE_OWNER:		return "no_store_owner";
		case NO_INVENTORIES:		return "no_inventories";
			// Default
		default:
			return "unknown_return_code";
		}
	}
	const char *getErrorMessage(am::base::ReturnCode code)
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
		case ITEM_NOT_IN_INVENTORY:	return "given item was not found in expected inventory.";
			// Store
		case NOT_ENOUGH_COIN:		return "buyer does not have enough coin to do purchase.";
		case NO_STORE_OWNER:		return "this store does not have an owser.";
		case NO_INVENTORIES:		return "store does not have any inventories.";
			// Default
		default:
			return "unknown return code.";
		}
	}

}
}
