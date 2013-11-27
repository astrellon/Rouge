#pragma once

namespace am {
namespace ui {

	enum InventoryEventType {
		INVENTORY_ADD, INVENTORY_REMOVE, INVENTORY_BEFORE_ADD, INVENTORY_BEFORE_REMOVE
	};

	const char **InventoryEventTypeName;

}
}
