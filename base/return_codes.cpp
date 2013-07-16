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
			// Character
		case ABLE_TO_EQUIP:		return "the character is able to equip the item.";
		case CAN_EQUIP:			return "the character can equip but is currently unable to.";
		case BODY_PART_TYPE_MISMATCH:	return "item cannot be equipped because the body part type on the part to be equipped was not found in the list of types on the items.";
		case NOT_ENOUGH_BODY_PARTS:	return "not enough body parts to equip the item.";
			// Body Parts
		case BODY_PART_FOUND:		return "body part was already found in the parts list.";
		case BODY_PART_NOT_FOUND:	return "unable to find the body part with the given part name.";
			// Default
		default:
			return "unknown return code.";
		}
	}

}
}
