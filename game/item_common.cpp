#include "item_common.h"

#include <util/utils.h>
using namespace am::util;

namespace am {
namespace game {

	const char *ItemCommon::ItemTypeNames[] = { "Helm", "Body", "Sword", "Axe", 
		"Bow", "Ring", "Amulet", "Leggings", "Boots", "Shield", "Potion",
		"Gold", "Unknown" };

	const char *ItemCommon::ItemTypeNameIds[] = { "helm", "body", "sword", "axe", 
		"bow", "ring", "amulet", "leggings", "boots", "shield", "potion",
		"gold", "unknown" };

	ItemCommon::ItemType ItemCommon::getItemTypeFromName(const char *name)
	{
		if (name == NULL)
		{
			return ItemCommon::UNKNOWN;
		}
		string lower = Utils::toLowerCase(name);
		int i = 0;
		for (i = 0; i < UNKNOWN; i++)
		{
			if (strcmp(ItemTypeNameIds[i], lower.c_str()) == 0)
			{
				break;
			}
		}
		return static_cast<ItemType>(i);
	}

}
}
