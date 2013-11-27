#include "inventory_event.h"

namespace am {
namespace ui {

	const char *Inventory::EventTypeName[] = 
	{
		"inventory_add", "inventory_remove", "inventory_before_add", 
        "inventory_before_remove"
	};

}
}

