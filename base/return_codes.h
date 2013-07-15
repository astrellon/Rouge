#pragma once

namespace am {
namespace base {

	enum ReturnCode {
		// General
		SUCCESS =					0x0001,
		NULL_PARAMETER =			0x0000,
		INTERNAL_ERROR =			-0x0001,

		// Character related.
		ABLE_TO_EQUIP =				0x0100,
		CAN_EQUIP =					0x0101,
		PART_NOT_FOUND =			-0x0100,
		BODY_PART_TYPE_MISMATCH =	-0x0101
	};

	const char *getErrorMessage(ReturnCode code);

}
}
