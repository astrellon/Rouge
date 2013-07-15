#include "return_codes.h"

namespace am {
namespace base {

	const char *getErrorMessage(ReturnCode code)
	{
		switch (code)
		{
		case SUCCESS:			return "success";
		case NULL_PARAMETER:	return "a parameter was null or an empty string.";
		case INTERNAL_ERROR:	return "there was an unexpected internal error.";
		case ABLE_TO_EQUIP:		return "the character is able to equip the item.";
		case CAN_EQUIP:			return "the character can equip but is currently unable to.";
		case PART_NOT_FOUND:	return "unable to find the body part with the given part name.";
		case BODY_PART_TYPE_MISMATCH:	return "item cannot be equipped because the body part type on the part to be equipped was not found in the list of types on the items.";
		default:
			return "unknown return code.";
		}
	}

}
}
