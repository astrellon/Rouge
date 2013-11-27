#pragma once

namespace am {
namespace base {

	enum ReturnCode 
	{
		// General
		SUCCESS =					0x0001,
		NULL_PARAMETER =			0x0000,
		INTERNAL_ERROR =			-0x0001,
		FILE_NOT_FOUND =			-0x0002,

		// Character related.
		ABLE_TO_EQUIP =				0x0100,
		CAN_EQUIP =					0x0101,
		BODY_PART_TYPE_MISMATCH =	-0x0100,
		NOT_ENOUGH_BODY_PARTS =		-0x0101,
		OUT_OF_RANGE =				-0x0102,
		NO_WEAPON_PART =			-0x0103,
		OFF_THE_MAP =				-0x0104,
		INVALID_LOCATION =			-0x0105,
		
		// Body Parts related
		BODY_PART_FOUND =			0x0200,
		BODY_PART_NOT_FOUND =		-0x0200,
		BODY_PART_TYPE_UNKNOWN =	-0x0201,

		// Texture related
		TEXTURE_LOAD_FAILED =		-0x0300,

		// System related
		NO_LINKED_SYSTEM =			-0x0400,

		// Inventory related
		NOT_ENOUGH_INVENTORY_SPACE =	-0x0500,
		ITEM_NOT_IN_INVENTORY =		-0x0501,

		// Store related
		NOT_ENOUGH_COIN	=			-0x0600,
		NO_STORE_OWNER =			-0x0601,
		NO_INVENTORIES =			-0x0602
	};

	const char *getErrorName(base::ReturnCode code);
	const char *getErrorMessage(base::ReturnCode code);

}
}
