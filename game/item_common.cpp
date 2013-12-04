#include "item_common.h"

#include <util/utils.h>
#include <string.h>

namespace am {
namespace game {

	const char *ItemCommon::ItemTypeNames[] = 
	{ 
		"Helm", "Body", "Sword", "Axe", 
		"Bow", "Ring", "Amulet", "Leggings", "Boots", "Shield", "Potion",
		"Gold", "Message Scroll", "Spell Scroll", "Unknown" 
	};

	const char *ItemCommon::ItemTypeNameIds[] = 
	{ 
		"helm", "body", "sword", "axe", 
		"bow", "ring", "amulet", "leggings", "boots", "shield", "potion",
		"gold", "message_scroll", "spell_scroll", "unknown" 
	};

	ItemCommon::ItemType ItemCommon::getItemType(const char *name)
	{
		if (name == nullptr)
		{
			return ItemCommon::UNKNOWN;
		}
		std::string lower = util::Utils::toLowerCase(name);
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
	ItemCommon::ItemType ItemCommon::getItemType(int typeValue)
	{
		if (typeValue < 0 || typeValue >= UNKNOWN)
		{
			return UNKNOWN;
		}
		return static_cast<ItemType>(typeValue);
	}

	const char *ItemCommon::getItemTypeName(ItemCommon::ItemType type)
	{
		if (type < 0 || type > UNKNOWN)
		{
			return nullptr;
		}
		return ItemTypeNameIds[type];
	}

	const char *ItemCommon::getItemTypeNameNice(ItemCommon::ItemType type)
	{
		if (type < 0 || type > UNKNOWN)
		{
			return nullptr;
		}
		return ItemTypeNames[type];
	}

}
}
