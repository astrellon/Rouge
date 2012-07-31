#pragma once

namespace am {
namespace game {

	class ItemCommon {
	public:
		enum ItemType {
			HELM, BODY, SWORD, AXE, BOW, RING, AMULET, LEGGINGS, BOOTS,
			SHIELD, POTION, GOLD, UNKNOWN
		};

		static const char *ItemTypeNames[];
		static const char *ItemTypeNameIds[];

		static ItemType getItemTypeFromName(const char *name);
	};

}
}
