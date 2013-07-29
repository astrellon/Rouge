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
		BODY_PART_TYPE_MISMATCH =	-0x0100,
		NOT_ENOUGH_BODY_PARTS =		-0x0101,
		OUT_OF_RANGE =				-0x0102,
		NO_WEAPON_PART =			-0x0103,

		// Body Parts related
		BODY_PART_FOUND =			0x0200,
		BODY_PART_NOT_FOUND =		-0x0200,
		BODY_PART_TYPE_UNKNOWN =	-0x0201
	};

	const char *getErrorMessage(ReturnCode code);

}
}
